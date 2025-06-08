/*
 * spi_dev.c
 *
 *  Created on: May 22, 2025
 *      Author: stelhs
 */
#include "spi_dev.h"
#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/klist.h"
#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/gpio.h"


static struct list spi_dev_list = LIST_INIT;

static void dev_destructor(void *mem)
{
	UNUSED(mem);
}

struct spi_dev *spi_dev_register(char *name, SPI_HandleTypeDef *hspi,
								 struct gpio *cs)
{
	struct spi_dev *dev;
	dev = kzref_alloc(name, sizeof *dev, dev_destructor);
	dev->hspi = hspi;
	dev->cs = cs;
	dev->lock = FALSE;
	list_append(&spi_dev_list, &dev->le, dev);
	return dev;
}


void spi_send_sleep(struct spi_dev *dev, u8 *data, size_t len)
{
	bool is_finished;
	irq_disable();
	dev->is_finished = FALSE;
	irq_enable();

	thread_lock(dev->lock);
	gpio_down(dev->cs);
	HAL_SPI_Transmit_DMA(dev->hspi, data, len);
	while(1) {
		irq_disable();
		is_finished = dev->is_finished;
		irq_enable();
		if (is_finished)
			break;
//		yield();
	}
	gpio_up(dev->cs);
	thread_unlock(dev->lock);
}

void spi_send_sync(struct spi_dev *dev, u8 *data, size_t len)
{
    thread_lock(dev->lock);
	gpio_down(dev->cs);
	HAL_SPI_Transmit(dev->hspi, data, len, HAL_MAX_DELAY);
	gpio_up(dev->cs);
	thread_unlock(dev->lock);
}

void spi_send_recv_sync(struct spi_dev *dev, u8 *tx_data,
                        u8 *rx_data, size_t len)
{
    thread_lock(dev->lock);
    gpio_down(dev->cs);
    HAL_SPI_TransmitReceive(dev->hspi, tx_data, rx_data, len, HAL_MAX_DELAY);
    gpio_up(dev->cs);
    thread_unlock(dev->lock);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	struct le *le;
	LIST_FOREACH(&spi_dev_list, le) {
		struct spi_dev *dev = (struct spi_dev *)list_ledata(le);
		if (dev->hspi != hspi)
			continue;

		dev->is_finished = TRUE;
	}
}

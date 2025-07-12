/*
 * potentiometer.c
 *
 *  Created on: Jun 5, 2025
 *      Author: stelhs
 */
#include "potentiometer.h"
#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/kref_alloc.h"
#include <stdlib.h>


static void potentiometer_destructor(void *mem)
{
    struct potentiometer *pm = (struct potentiometer *)mem;
    UNUSED(pm);
}


struct potentiometer *
potentiometer_register(char *name, ADC_HandleTypeDef *hadc, int adc_offset)
{
    struct potentiometer *pm;

    pm = kzref_alloc(name, sizeof *pm, potentiometer_destructor);
    pm->name = name;
    pm->hadc = hadc;
    pm->adc_offset = adc_offset;
///    HAL_ADC_Start_IT(hadc);
    // TODO
    return pm;
}

static u16 integrator(u16 *buf, size_t size)
{
    u32 i;
    u16 val;
    u16 sz = (size - 1);

    bool changed = FALSE;
    do {
        changed = FALSE;
        for (i = 0; i < sz - 1; i++) {
            if (buf[i] <= buf[i + 1])
                continue;

            val = buf[i];
            buf[i] = buf[i + 1];
            buf[i + 1] = val;
            changed = TRUE;
        }
    } while (changed);

    val = buf[(size / 2)];
    return val;
}

int potentiometer_val(struct potentiometer *pm)
{
    irq_disable();
    u16 val = integrator(pm->buf, ARRAY_SIZE(pm->buf));
    irq_enable();
    if (val > pm->adc_offset)
        val -= pm->adc_offset;
    else
        val = 0;

    return val * 100 / 4096;
}

bool is_potentiometer_changed(struct potentiometer *pm)
{
    int val = potentiometer_val(pm);
    if (abs(val - pm->last_val) > 1) {
        pm->last_val = val;
        return TRUE;
    }
    return FALSE;
}



/*
 * potentiometer.h
 *
 *  Created on: Jun 5, 2025
 *      Author: stelhs
 */

#ifndef POTENTIOMETER_H_
#define POTENTIOMETER_H_

#include "adc.h"
#include "stm32_lib/types.h"

struct potentiometer {
    char *name;
    ADC_HandleTypeDef *hadc;
    int adc_offset;
    u16 buf[32];
    int buf_index;
    int last_val;
};

struct potentiometer *
potentiometer_register(char *name, ADC_HandleTypeDef *hadc,
                       int adc_offset);

int potentiometer_val(struct potentiometer *pm);
bool is_potentiometer_changed(struct potentiometer *pm);

static inline void potentiometer_irq(struct potentiometer *pm)
{
    pm->buf[pm->buf_index] = HAL_ADC_GetValue(pm->hadc);
    pm->buf_index ++;
    if (pm->buf_index >= ARRAY_SIZE(pm->buf))
        pm->buf_index = 0;
}


#endif /* POTENTIOMETER_H_ */

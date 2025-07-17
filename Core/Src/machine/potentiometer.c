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
    return pm;
}

void potentiometer_handler(struct potentiometer *pm)
{
    int val;
    if ((now() - pm->start_time) < 10)
        return;

    pm->start_time = now();
    HAL_ADC_Start(&hadc1);

    val = HAL_ADC_GetValue(&hadc1);
    if (val > pm->adc_offset)
        val -= pm->adc_offset;
    else
        val = 0;

    if (pm->val != val) {
        pm->val = val;
        pm->is_changed = TRUE;
    }
}

int potentiometer_val(struct potentiometer *pm)
{
    return pm->val * 100 / 4096;
}

bool is_potentiometer_changed(struct potentiometer *pm)
{
    return pm->is_changed;
}



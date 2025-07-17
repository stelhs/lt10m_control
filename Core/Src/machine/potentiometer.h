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
    int val;
    int start_time;
    bool is_changed;
};

struct potentiometer *
potentiometer_register(char *name, ADC_HandleTypeDef *hadc,
                       int adc_offset);

int potentiometer_val(struct potentiometer *pm);
bool is_potentiometer_changed(struct potentiometer *pm);
void potentiometer_handler(struct potentiometer *pm);


#endif /* POTENTIOMETER_H_ */

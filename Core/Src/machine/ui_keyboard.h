/*
 * disp_keyboard.h
 *
 *  Created on: Jun 7, 2025
 *      Author: stelhs
 */

#ifndef UI_KEYBOARD_H_
#define UI_KEYBOARD_H_

#include "stm32_lib/types.h"

int ui_keyboard_run(char *field_name, int *val,
                    int min, int max, int step, bool is_integer);

#endif /* UI_KEYBOARD_H_ */

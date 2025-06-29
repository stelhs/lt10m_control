/*
 * periphery.h
 *
 */

#ifndef PERIPHERY_H_
#define PERIPHERY_H_
#include "stm32_lib/types.h"

#define MOVE_LEFT 1
#define MOVE_RIGHT 0
#define MOVE_UP 0
#define MOVE_DOWN 1
#define NO_AUTO_STOP 0x7FFFFFFF

void periphery_init(void);
void beep_blink_start(int beep_duration, int period, int count);
void beep_blink_stop(void);
int panel_encoder_val(void);
void panel_encoder_reset(void);

#endif /* PERIPHERY_H_ */

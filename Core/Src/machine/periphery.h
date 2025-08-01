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

#define SPINDLE_ENC_RESOLUTION 3000

void periphery_init(void);

void beep_blink_start(int beep_duration, int period, int count);
void beep_blink_stop(void);

int panel_encoder_val(void);
void panel_encoder_reset(void);

u32 spindle_angle(void);
u32 spindle_raw_angle(void);
u32 spindle_raw_to_angle(u32 raw);
u32 spindle_speed(void);

void spindle_power_on(void);
void spindle_power_off(void);


#endif /* PERIPHERY_H_ */

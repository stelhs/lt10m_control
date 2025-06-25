/*
 * periphery.h
 *
 */

#ifndef PERIPHERY_H_
#define PERIPHERY_H_
#include "stm32_lib/types.h"

void periphery_init(void);
void lde_d2_on(void);
void lde_d2_off(void);

#define MOVE_LEFT 1
#define MOVE_RIGHT 0
#define MOVE_UP 0
#define MOVE_DOWN 1
#define NO_AUTO_STOP 0x7FFFFFFF

#endif /* PERIPHERY_H_ */

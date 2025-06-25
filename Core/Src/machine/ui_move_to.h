/*
 * ui_move_to.h
 *
 *  Created on: Jun 21, 2025
 *      Author: stelhs
 */

#ifndef UI_MOVE_TO_H_
#define UI_MOVE_TO_H_

void ui_move_to_run(void);

int ui_move_to_step(void);
void ui_moveto_blink_left_arrow(void);
void ui_moveto_blink_right_arrow(void);
void ui_moveto_blink_up_arrow(void);
void ui_moveto_blink_down_arrow(void);
void ui_moveto_blink_stop(void);
void ui_moveto_blink_up_down_arrow(void);
void ui_moveto_blink_left_right_arrow(void);

void ui_move_to_lock_moveto(void);
void ui_move_to_unlock_moveto(void);

#endif /* UI_MOVE_TO_H_ */

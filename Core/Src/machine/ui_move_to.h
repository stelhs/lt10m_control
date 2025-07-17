/*
 * ui_move_to.h
 *
 *  Created on: Jun 21, 2025
 *      Author: stelhs
 */

#ifndef UI_MOVE_TO_H_
#define UI_MOVE_TO_H_

struct ui_move_to {
    struct ui_scope *ui_scope;
    struct ui_item *left_arrow;
    struct ui_item *right_arrow;
    struct ui_item *up_arrow;
    struct ui_item *down_arrow;
    struct ui_item *up_down_arrow;
    struct ui_item *left_right_arrow;
    struct ui_item *move_to_cross;
    struct ui_item *move_to_longitudal;
    struct disp *disp_info;
    struct disp *disp_touch;
    int move_to_cross_pos;
    int move_to_longitudal_pos;
    int move_step;
};

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

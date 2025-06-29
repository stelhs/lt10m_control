/*
 * disp_main_win.h
 *
 *  Created on: Jun 8, 2025
 *      Author: stelhs
 */

#ifndef UI_MAIN_H_
#define UI_MAIN_H_

#include "ui_sel_prog.h"
#include "stm32_lib/types.h"

enum cut_return_modes {
    CUT_MODE_RETURN_DOWN,
    CUT_MODE_MOVE_DOWN,
    CUT_MODE_MOVE_UP,
    CUT_MODE_RETURN_UP,
    CUT_MODE_NO_RETURN,

    CUT_MODE_ITEM_LAST
};

enum msg_type {
    MSG_ERR,
    MSG_WARN,
    MSG_NORM
};

struct ui_main {
    struct disp *disp;
    struct ui_main_items *ui_items;
    enum progs prog;
    int longitudial_limit;
    int target_diameter;
    int cross_limit;
    int step;
    bool feed_or_diameter;
    enum cut_return_modes crm;
    int pass_repeate_cnt;
};

extern struct ui_main *ui_main;

void ui_main_start(void);
void ui_message_show(char *msg, enum msg_type mt);
void ui_message_hide(void);

#endif /* UI_MAIN_H_ */

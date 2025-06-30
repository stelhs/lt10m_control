/*
 * disp_main_win.h
 *
 *  Created on: Jun 8, 2025
 *      Author: stelhs
 */

#ifndef UI_MAIN_H_
#define UI_MAIN_H_

#include "stm32_lib/types.h"

enum msg_type {
    MSG_ERR,
    MSG_WARN,
    MSG_NORM
};

struct ui_main {
    struct disp *disp;
    struct ui_main_items *ui_items;
    struct mode_cut *mc;
};

void ui_main_start(void);
void ui_message_show(char *msg, enum msg_type mt);
void ui_message_hide(void);

#endif /* UI_MAIN_H_ */

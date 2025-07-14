/*
 * disp_main_win.h
 *
 *  Created on: Jun 8, 2025
 *      Author: stelhs
 */

#ifndef UI_MAIN_H_
#define UI_MAIN_H_

#include "stm32_lib/types.h"
#include "stm32_lib/klist.h"

enum msg_type {
    MSG_ERR,
    MSG_WARN,
    MSG_NORM
};

struct ui_main {
    struct disp *disp;
    struct mode_cut *mc;
    struct mode_thread *mt;
    struct list *ui_scope;
    struct ui_item *thread_moveto;
    struct ui_item *thread_size;
    struct ui_item *thread_standard_m;
    struct ui_item *feed_return_mode;
    struct ui_item *feed_repeate;
    struct ui_item *thread_arrow;
    struct ui_item *thread_repeate;
    struct ui_item *thread_type;
};

void ui_main_start(void);
void ui_message_show(char *msg, enum msg_type mt);
void ui_message_hide(void);
char *interval_to_str(int time_sec);

#endif /* UI_MAIN_H_ */

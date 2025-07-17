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
    struct ui_scope *ui_scope;
    struct ui_item *thread_moveto_start;
    struct ui_item *thread_size;
    struct ui_item *thread_standard_m;
    struct ui_item *thread_depth;
    struct ui_item *feed_return_mode;
    struct ui_item *feed_repeate;
    struct ui_item *feed_limit;
    struct ui_item *cross_or_diameter_val;
    struct ui_item *feed_step;
    struct ui_item *thread_arrow;
    struct ui_item *thread_repeate;
    struct ui_item *thread_type;
    struct ui_item *thread_info;
    struct ui_item *thread_offset;
};

void ui_main_start(void);
void ui_message_show(char *msg, enum msg_type mt);
void ui_message_hide(void);
char *interval_to_str(int time_sec);
void program_finished_show(void);
void ui_main_unlock(void);
void ui_main_lock(void);

#endif /* UI_MAIN_H_ */

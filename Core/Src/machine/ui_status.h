/*
 * ui_status.h
 *
 *  Created on: Jun 17, 2025
 *      Author: stelhs
 */

#ifndef UI_STATUS_H_
#define UI_STATUS_H_
#include "stm32_lib/types.h"

struct ui_status {
    int curr_tool_num;
    int cross_pos;
    int longitudal_pos;
    bool is_longitudal_inc_right;
    bool is_cross_inc_up;
};

void ui_status_show(void);
void ui_status_cross_update(bool is_erase);
void ui_status_longitudal_update(bool is_erase);
void ui_status_tool_update(void);
void ui_status_cross_dir_update(void);
void ui_status_longitudal_dir_update(void);

#endif /* UI_STATUS_H_ */

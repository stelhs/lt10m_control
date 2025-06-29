/*
 * ui_status.h
 *
 *  Created on: Jun 17, 2025
 *      Author: stelhs
 */

#ifndef UI_STATUS_H_
#define UI_STATUS_H_

#include "ui_item.h"

struct ui_status {
    struct ui_item *cross_pos_dir;
    struct ui_item *cross_pos;
    struct ui_item *longitudal_pos_dir;
    struct ui_item *longitudal_pos;
    struct ui_item *spindle_icon;
    struct ui_item *spindle_speed;
    struct ui_item *tool_num;
    struct ui_item *cross_speed_icon;
    struct ui_item *cross_speed;
    struct ui_item *longitudal_speed_icon;
    struct ui_item *longitudal_speed;
    struct ui_item *feed_rate_icon;
    struct ui_item *feed_rate;
};

void ui_status_init(void);

#endif /* UI_STATUS_H_ */

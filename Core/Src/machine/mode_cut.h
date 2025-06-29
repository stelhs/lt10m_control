/*
 * mode_cut.h
 *
 *  Created on: Jun 26, 2025
 *      Author: stelhs
 */

#ifndef MODE_CUT_H_
#define MODE_CUT_H_

#include "ui_item.h"

struct mode_cut_ui_items {
    struct ui_item *uptime;
    struct ui_item *number_passes;
    struct ui_item *cutting_speed;
    struct ui_item *feed_speed;
    struct ui_item *cut_depth;
    struct ui_item *cross_miss_up;
    struct ui_item *cross_miss_down;
    struct ui_item *longitudal_miss_left;
    struct ui_item *longitudal_miss_right;
};

struct mode_cut {
    struct disp *disp;
    struct mode_cut_ui_items *ui_items;
    int start_time;
    int calc_time;
    int pass_number;
    int cut_depth;

    int cross_miss_up;
    int cross_miss_down;
    int longitudal_miss_left;
    int longitudal_miss_right;
};

void mode_cut_run(void);

#endif /* MODE_CUT_H_ */

/*
 * mode_cut.h
 *
 *  Created on: Jun 26, 2025
 *      Author: stelhs
 */

#ifndef MODE_CUT_H_
#define MODE_CUT_H_

#include "ui_item.h"
#include "ui_main.h"

enum cut_longitudal_return_modes {
    CUT_LONGITUDAL_RETURN_DOWN,
    CUT_LONGITUDAL_MOVE_DOWN,
    CUT_LONGITUDAL_MOVE_UP,
    CUT_LONGITUDAL_RETURN_UP,
    CUT_LONGITUDAL_NO_RETURN,
};

enum cut_cross_return_modes {
    CUT_CROSS_RETURN_RIGHT,
    CUT_CROSS_MOVE_RIGHT,
    CUT_CROSS_MOVE_LEFT,
    CUT_CROSS_RETURN_LEFT,
    CUT_CROSS_NO_RETURN,
};


#define CROSS_RETRACTION 500
#define LONGITUDAL_RETRACTION 500

struct mode_cut_stat {
    int cross_miss_up;
    int cross_miss_down;
    int longitudal_miss_left;
    int longitudal_miss_right;
    int start_time;
    int calc_time;
};

struct mode_cut_settings {
    int feed_rate;
    int longitudal_distance;
    int cross_distance;
    int target_diameter;
    int cut_depth_step;
    enum cut_longitudal_return_modes longitudal_ret_mode;
    enum cut_cross_return_modes cross_ret_mode;
    int last_repeate_number;
};

struct mode_cut {
    struct ui_scope *ui_status_scope;
    struct ui_item *status_text;
    struct mode_cut_settings settings;
    struct mode_cut_stat stat;
    int start_longitudal_pos;
    int start_cross_pos;
    bool cross_pass_dir;
    int end_longitudal_pos;
    int end_cross_pos;
    int cross_pos;
    int longitudal_pos;
    int cut_depth;
    int cut_pass_cnt;
    int cut_pass_rest;
    int cut_pass_last_cnt;
    int cross_distance;
    int longitudal_distance;
    bool longitudal_pass_dir;
};

void mode_cut_run(void);
void mode_cut_settings_validate(void);

#endif /* MODE_CUT_H_ */

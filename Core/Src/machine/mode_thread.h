/*
 * mode_thread.h
 *
 *  Created on: Jul 8, 2025
 *      Author: stelhs
 */

#ifndef MODE_THREAD_H_
#define MODE_THREAD_H_

struct mode_thread_settings {
    int thread_size;
    int cut_depth;
    bool is_internal;
    int length;
    bool is_type_inch;
    int last_repeate_number;
};

struct mode_thread {
    struct mode_thread_settings settings;
    bool cross_dir;
    bool longitudal_dir;
    int step_size;
    int cut_depth;
    int curr_depth;
    int max_depth;
    int cut_offset;
    int krpm_min;
    int krpm_max;
    int start_longitudal_pos;
    int end_longitudal_pos;
    int start_angle;
    int start_cross_pos;
    int end_cross_pos;
    int repeate_number;
};

int mode_thread_run(void);

#endif /* MODE_THREAD_H_ */

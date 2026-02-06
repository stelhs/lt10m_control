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
    int cut_depth_step;
    int max_cut_depth;
    bool is_internal;
    int length;
    bool is_type_inch;
    int last_repeate_number;
    int standart_diameter;
    const struct thread_metric *tm;
    int spindle_start;
    int longitudal_start;
};

struct mode_thread {
    struct mode_thread_settings settings;
    struct ui_scope *ui_status_scope;
    struct ui_item *status_text;
    bool cross_dir;
    bool longitudal_dir;
    int step_size;
    int curr_depth;
    int max_depth;
    int cut_offset;
    int krpm_min;
    int krpm_max;
    int start_longitudal_pos;
    int end_longitudal_pos;
    int start_cross_pos;
    int end_cross_pos;
    int repeate_number;
    int calc_time;
    int start_time;
    int calc_cut_pass_num;
    int cut_pass_cnt;
    int cut_pass_rest;
    int curr_depth_step;
};

struct thread_metric_info {
    int diameter_max;
    int diameter_min;
    int diameter;
    int depth_max;
    int depth_min;
    int depth;
    int end_diameter;
    int end_diameter_max;
    int end_diameter_min;
};


void mode_thread_run(void);
int thread_standart_steps_list(int diameter, u32 *list);
void thread_state_init(void);
void thread_standart_info(const struct thread_metric *tm, bool is_internal,
                          int diameter, struct thread_metric_info *tmi);
int thread_calc_work_time(int length, int pass_number, int sm_freq);
void thread_calibrate_entry_point(void);

#endif /* MODE_THREAD_H_ */

/*
 * ui_status.c
 *
 *  Created on: Jun 17, 2025
 *      Author: stelhs
 */

#include "ui_status.h"
#include "stm32_lib/kref_alloc.h"
#include "abs_position.h"
#include "images.h"
#include "machine.h"
#include "periphery.h"


static void ui_tool_num_show(struct ui_item *ut)
{
    struct machine *m = &machine;
    char str_num[2];
    struct text_style ts = {
            .bg_color = BLACK,
            .color = EMERALD,
            .font = font_rus,
            .fontsize = 3,
    };
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 2, EMERALD);

    sprintf(str_num, "%d", abs_pos_tool(m->ap) + 1);
    disp_text(ut->disp, str_num, ut->x + 9, ut->y + 5, &ts);
}

static void ui_cross_pos_dir_show(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct abs_position *ap = m->ap;
    struct img *img;

    if (ap->is_cross_inc_down)
        img = img_cross_arrow_down1();
    else
        img = img_cross_arrow_up1();
    disp_fill_img(ut->disp, ut->x, ut->y, img);
    kmem_deref(&img);
}

static void ui_cross_pos_getter(struct ui_item *ut, char *str, size_t size)
{
    struct machine *m = &machine;
    struct abs_position *ap = m->ap;
    int pos = abs_cross_curr_tool(ap) * 2;
    snprintf(str, size, "%.3f", (float)pos / 1000);
}

static void ui_longitudal_pos_dir_show(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct abs_position *ap = m->ap;
    struct img *img;

    if (ap->is_longitudal_inc_left)
        img = img_longitudal_arrow_left1();
    else
        img = img_longitudal_arrow_right1();
    disp_fill_img(ut->disp, ut->x, ut->y, img);
    kmem_deref(&img);
}

static void ui_longitudal_pos_getter(struct ui_item *ut, char *str, size_t size)
{
    struct machine *m = &machine;
    struct abs_position *ap = m->ap;
    int pos = abs_longitudal_curr_tool(ap);
    snprintf(str, size, "%.3f", (float)pos / 1000);
}

static void ui_spindle_icon_show(struct ui_item *ut)
{
    struct img *img;
    if (spindle_speed() < 10000)
        img = img_spindle_angle();
    else
        img = img_spindle_speed();
    disp_fill_img(ut->disp, ut->x, ut->y, img);
    kmem_deref(&img);
}

static void ui_spindle_speed_getter(struct ui_item *ut, char *str, size_t size)
{
    struct machine *m = &machine;
    struct ui_status *us = &m->ui_stat;

    u32 val = spindle_speed();
    static u32 last_mode = 0;
    if (val < 10000) {
        val = spindle_angle();
        snprintf(str, size, "%.1f", (float)val / 1000);
        if (!last_mode)
            ui_item_show(us->spindle_icon);
        last_mode = 1;
        return;
    }
    snprintf(str, size, "%lu", val / 1000);
    if (last_mode)
        ui_item_show(us->spindle_icon);
    last_mode = 0;
}

static void ui_cross_speed_icon_show(struct ui_item *ut)
{
    struct img *img;
    img = img_cross_speed();
    disp_fill_img(ut->disp, ut->x, ut->y, img);
    kmem_deref(&img);
}

static void ui_cross_speed_getter(struct ui_item *ut, char *str, size_t size)
{
    struct machine *m = &machine;

    snprintf(str, size, "%.3f", (float)m->ap->cross_speed / 1000);
}

static void ui_longitudal_speed_icon_show(struct ui_item *ut)
{
    struct img *img;
    img = img_longitudal_speed();
    disp_fill_img(ut->disp, ut->x, ut->y, img);
    kmem_deref(&img);
}

static void ui_longitudal_speed_getter(struct ui_item *ut,
                                       char *str, size_t size)
{
    struct machine *m = &machine;
    snprintf(str, size, "%.3f", (float)m->ap->longitudal_speed / 1000);
}

static void ui_feed_rate_icon_show(struct ui_item *ut)
{
    struct img *img;
    img = img_feed_rate();
    disp_fill_img(ut->disp, ut->x, ut->y, img);
    kmem_deref(&img);
}

static void ui_feed_rate_getter(struct ui_item *ut,
                                char *str, size_t size)
{
    struct machine *m = &machine;
    struct mode_cut *mc = &m->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;
    snprintf(str, size, "%.3f", (float)mc_settings->feed_rate / 1000);
}

static void ui_cut_speed_icon_show(struct ui_item *ut)
{
    struct img *img;
    img = img_cut_speed();
    disp_fill_img(ut->disp, ut->x, ut->y, img);
    kmem_deref(&img);
}

static void ui_cut_speed_getter(struct ui_item *ut,
                                char *str, size_t size)
{
    struct machine *m = &machine;
    int diameter = abs_cross_curr_tool(m->ap) * 2;
    int circumference = (3141 * diameter) / 1000; // 3141 это Pi 3.141*1000
    int rpm = spindle_speed() / 1000;
    int val = (circumference * rpm) / 1000;
    snprintf(str, size, "%.1f", (float)val / 1000);
}

void ui_status_init(void)
{
    struct machine *m = &machine;
    struct disp *disp = m->disp2;
    struct ui_status *us = &m->ui_stat;
    struct text_style cross_ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3,
    };
    struct text_style longitudal_ts = {
            .bg_color = BLACK,
            .color = BLUE,
            .font = font_rus,
            .fontsize = 3,
    };
    struct text_style spindle_ts = {
            .bg_color = BLACK,
            .color = {241, 255, 146},
            .font = font_rus,
            .fontsize = 3,
    };
    struct text_style feed_rate_ts = {
            .bg_color = BLACK,
            .color = {255, 128, 0},
            .font = font_rus,
            .fontsize = 3,
    };
    struct text_style cut_speed_ts = {
            .bg_color = BLACK,
            .color = {255, 128, 128},
            .font = font_rus,
            .fontsize = 3,
    };

    disp_fill(disp, 0,0, 480, 111, BLACK); // Clear status area
    disp_fill(disp, 2, 111, 480 - 4, 3, GREEN); // Green line

    us->cross_pos_dir =
            ui_item_register("ui_cross_pos_dir", NULL, disp,
                                 0, 2, 16, 28,
                                 ui_cross_pos_dir_show, NULL, NULL, 0);

    us->cross_pos =
            ui_item_text_register("ui_cross_pos", NULL, disp,
                                  23, 5, 8, &cross_ts,
                                  ui_cross_pos_getter, NULL, NULL);

    us->longitudal_pos_dir =
            ui_item_register("ui_longitudal_pos_dir", NULL, disp,
                                 173, 7, 31, 18,
                                 ui_longitudal_pos_dir_show, NULL, NULL, 0);

    us->longitudal_pos =
            ui_item_text_register("ui_longitudal_pos", NULL, disp,
                                  208, 5, 8, &longitudal_ts,
                                  ui_longitudal_pos_getter, NULL, NULL);

    us->spindle_icon =
            ui_item_register("ui_spindle_icon", NULL, disp,
                                 358, 4, 26, 29,
                                 ui_spindle_icon_show, NULL, NULL, 0);

    us->spindle_speed =
            ui_item_text_register("ui_spindle_speed", NULL, disp,
                                  390, 9, 5, &spindle_ts,
                                  ui_spindle_speed_getter, NULL, NULL);

    us->tool_num =
            ui_item_register("ui_tool_num", NULL, disp,
                                 1, 39, 32, 32,
                                 ui_tool_num_show, NULL, NULL, 0);

    us->cross_speed_icon =
            ui_item_register("ui_cross_speed_icon", NULL, disp,
                                 39, 36, 33, 36,
                                 ui_cross_speed_icon_show, NULL, NULL, 0);

    us->cross_speed =
            ui_item_text_register("ui_cross_speed", NULL, disp,
                                  77, 48, 6, &cross_ts,
                                  ui_cross_speed_getter, NULL, NULL);

    us->longitudal_speed_icon =
            ui_item_register("ui_longitudal_speed_icon", NULL, disp,
                                 186, 36, 33, 36,
                                 ui_longitudal_speed_icon_show, NULL, NULL, 0);

    us->longitudal_speed =
            ui_item_text_register("ui_longitudal_speed", NULL, disp,
                                  223, 48, 6, &longitudal_ts,
                                  ui_longitudal_speed_getter, NULL, NULL);

    us->feed_rate_icon =
            ui_item_register("ui_feed_rate_icon", NULL, disp,
                                 334, 36, 33, 36,
                                 ui_feed_rate_icon_show, NULL, NULL, 0);

    us->feed_rate =
            ui_item_text_register("ui_feed_rate", NULL, disp,
                                  372, 48, 6, &feed_rate_ts,
                                  ui_feed_rate_getter, NULL, NULL);

    us->cut_speed_icon =
            ui_item_register("ui_cut_speed_icon", NULL, disp,
                                 300, 74, 33, 36,
                                 ui_cut_speed_icon_show, NULL, NULL, 0);

    us->cut_speed =
            ui_item_text_register("ui_cut_speed", NULL, disp,
                                  338, 86, 7, &cut_speed_ts,
                                  ui_cut_speed_getter, NULL, NULL);
}


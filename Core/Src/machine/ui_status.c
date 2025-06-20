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


void ui_status_show(void)
{
    struct machine *m = &machine;
    struct disp *disp = m->disp2;

    disp_fill(disp, 0,0, 480, 43, BLACK);
    disp_fill(disp, 2, 40, 480 - 4, 3, GREEN);
    disp_rect(disp, 5, 0, 35, 32, 2, EMERALD);

    ui_status_cross_dir_update();
    ui_status_tool_update();
    ui_status_cross_update(FALSE);
    ui_status_longitudal_dir_update();
    ui_status_longitudal_update(FALSE);
}

void ui_status_cross_update(bool is_erase)
{
    struct machine *m = &machine;
    struct disp *disp = m->disp2;
    char str_num[10];
    struct abs_position *ap = m->ap;
    int pos, len;
    int width, height;
    struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3,
    };

    pos = abs_cross(ap, m->curr_tool_num);
    sprintf(str_num, "%.3f", (float)pos / 1000);
    if (is_erase) {
        len = strlen(str_num);
        width = disp_text_width(&ts, sizeof str_num);
        height = disp_text_height(&ts, len);
        disp_fill(disp, 78, 9, width, height, BLACK);
    }
    disp_text(disp, str_num, 78, 9, &ts);
}

void ui_status_longitudal_update(bool is_erase)
{
    struct machine *m = &machine;
    struct disp *disp = m->disp2;
    char str_num[10];
    struct abs_position *ap = m->ap;
    int pos, len;
    int width, height;
    struct text_style ts = {
            .bg_color = BLACK,
            .color = BLUE,
            .font = font_rus,
            .fontsize = 3,
    };

    pos = abs_longitudal(ap, m->curr_tool_num);
    sprintf(str_num, "%.3f", (float)pos / 1000);
    if (is_erase) {
        len = strlen(str_num);
        width = disp_text_width(&ts, sizeof str_num);
        height = disp_text_height(&ts, len);
        disp_fill(disp, 300, 9, width, height, BLACK);
    }
    disp_text(disp, str_num, 300, 9, &ts);
}

void ui_status_tool_update(void)
{
    struct machine *m = &machine;
    struct disp *disp = m->disp2;
    char str_num[2];
    int width, height, len;
    struct text_style ts = {
            .bg_color = BLACK,
            .color = EMERALD,
            .font = font_rus,
            .fontsize = 3,
    };

    sprintf(str_num, "%d", m->curr_tool_num + 1);
    len = strlen(str_num);
    width = disp_text_height(&ts, len);
    height = disp_text_width(&ts, len);
    disp_fill(disp, 15, 7, width, height, BLACK);
    disp_text(disp, str_num, 15, 7, &ts);
}

void ui_status_cross_dir_update(void)
{
    struct machine *m = &machine;
    struct disp *disp = m->disp2;
    struct abs_position *ap = m->ap;
    struct img *img;

    if (ap->is_cross_inc_up)
        img = img_cross_arrow_up1();
    else
        img = img_cross_arrow_down1();
    disp_fill_img(disp, 53, 4, img);
    kmem_deref(&img);
}

void ui_status_longitudal_dir_update(void)
{
    struct machine *m = &machine;
    struct disp *disp = m->disp2;
    struct abs_position *ap = m->ap;
    struct img *img;

    if (ap->is_longitudal_inc_right)
        img = img_longitudal_arrow_right1();
    else
        img = img_longitudal_arrow_left1();
    disp_fill_img(disp, 260, 10, img);
    kmem_deref(&img);
}

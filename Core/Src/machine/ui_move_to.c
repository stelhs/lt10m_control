/*
 * ui_move_to.c
 *
 *  Created on: Jun 21, 2025
 *      Author: stelhs
 */
#include "machine.h"
#include "ui_move_to.h"
#include "ui_set_xy.h"
#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/buttons.h"
#include "disp_mipi_dcs.h"
#include "disp_button.h"
#include "images.h"
#include "ui_keyboard.h"


struct ui_move_to_buttons {
    struct disp_button *set_xy;
    struct disp_button *set_step;
    struct disp_button *set_cross_pos;
    struct disp_button *set_longitudal_pos;
    struct disp_button *move_to;
};

struct ui_move_to {
    struct ui_move_to_buttons *buttons;
    struct disp *disp_info;
    struct disp *disp_touch;
    int move_step;
    int move_to_cross_pos;
    int move_to_longitudal_pos;
};

static struct ui_move_to *ui_move_to = NULL;

static void show(void);
static void hide(void)
{
    struct ui_move_to *umt = ui_move_to;
    kmem_deref(&umt->buttons);
}


static void ui_move_to_buttons_destructor(void *mem)
{
    struct ui_move_to_buttons *buttons = (struct ui_move_to_buttons *)mem;
    kmem_deref(&buttons->set_xy);
    kmem_deref(&buttons->set_step);
    kmem_deref(&buttons->set_cross_pos);
    kmem_deref(&buttons->set_longitudal_pos);
    kmem_deref(&buttons->move_to);
}

static void key_set_xy_draw(struct disp_button *db)
{
    struct img *img;
    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, GRAY);

    img = img_set_xy();
    disp_fill_img(db->disp, db->x + 20, db->y + 4, img);
    kmem_deref(&img);
}

static void onclick_set_xy(void *priv)
{
    hide();
    ui_set_xy_run();
    show();
}

static void key_set_step_draw(struct disp_button *db)
{
    struct ui_move_to *umt = (struct ui_move_to *)db->priv;
    char *string;
    int x, width;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = YELLOW,
            .font = font_rus,
            .fontsize = 3
    };

  //  disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, GRAY);

    string = kref_sprintf("%.3f", (float)umt->move_step / 1000);
    width = disp_text_width(&ts, strlen(string));
    x = db->width / 2 - width / 2;
    disp_text(db->disp, string, db->x + x, db->y + 17, &ts);
    kmem_deref(&string);
}

static void key_set_cross_pos_draw(struct disp_button *db)
{
    struct ui_move_to *umt = (struct ui_move_to *)db->priv;
    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

//    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, GRAY);
    string = kref_sprintf("%.3f", (float)umt->move_to_cross_pos / 1000);
    disp_text(db->disp, string, db->x + 5, db->y + 17, &ts);
    kmem_deref(&string);
}

static void key_set_longitudal_pos_draw(struct disp_button *db)
{
    struct ui_move_to *umt = (struct ui_move_to *)db->priv;
    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = BLUE,
            .font = font_rus,
            .fontsize = 3
    };

//    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, GRAY);

    string = kref_sprintf("%.3f", (float)umt->move_to_longitudal_pos / 1000);
    disp_text(db->disp, string, db->x + 5, db->y + 17, &ts);
    kmem_deref(&string);
}

static void key_move_to_draw(struct disp_button *db)
{
    struct ui_move_to *umt = (struct ui_move_to *)db->priv;
    struct img *img;
    disp_rect(db->disp, db->x, db->y, db->width, db->height, 2, RED);
    img = img_moveto();
    disp_fill_img(db->disp, db->x + 8, db->y + 18, img);
    kmem_deref(&img);
}



void onclick_set_step(void *priv)
{
    struct ui_move_to *umt = (struct ui_move_to *)priv;
    float pos = (float)umt->move_step / 1000;
    hide();
    if (!ui_keyboard_run("move step: ", &pos)) {
        umt->move_step = (int)(pos * 1000);
    }
    show();
}

void onclick_set_cross_pos(void *priv)
{
    struct ui_move_to *umt = (struct ui_move_to *)priv;
    float pos = (float)umt->move_to_cross_pos / 1000;
    hide();
    if (!ui_keyboard_run("cross position: ", &pos)) {
        umt->move_to_cross_pos = (int)(pos * 1000);
    }
    show();
}

void onclick_set_longitudal_pos(void *priv)
{
    struct ui_move_to *umt = (struct ui_move_to *)priv;
    float pos = (float)umt->move_to_longitudal_pos / 1000;
    hide();
    if (!ui_keyboard_run("longitudal position: ", &pos)) {
        umt->move_to_longitudal_pos = (int)(pos * 1000);
    }
    show();
}

void onclick_move_to(void *priv)
{
    struct ui_move_to *umt = (struct ui_move_to *)priv;
    printf("call moveto\r\n");
}




static void show(void)
{
    struct ui_move_to *umt = ui_move_to;
    struct img *img;

    struct ui_move_to_buttons *buttons;
    buttons = kzref_alloc("ui_move_to_buttons", sizeof *buttons,
                          ui_move_to_buttons_destructor);
    umt->buttons = buttons;

    disp_clear(umt->disp_touch);

    buttons->set_xy =
            disp_button_register("key2_set_xy", umt->disp_touch,
                                 190, 10, 100, 55, umt,
                                 key_set_xy_draw, onclick_set_xy);

    buttons->set_step =
            disp_button_register("key_set_step", umt->disp_touch,
                                 87, 125, 145, 55, umt,
                                 key_set_step_draw, onclick_set_step);

    buttons->set_cross_pos =
            disp_button_register("key_set_cross_pos", umt->disp_touch,
                                 160, 320, 160, 55, umt,
                                 key_set_cross_pos_draw,
                                 onclick_set_cross_pos);

    buttons->set_longitudal_pos =
            disp_button_register("key_set_longitudal_pos", umt->disp_touch,
                                 160, 390, 160, 55, umt,
                                 key_set_longitudal_pos_draw,
                                 onclick_set_longitudal_pos);

    buttons->move_to =
            disp_button_register("key_move_to", umt->disp_touch,
                                 5, 325, 90, 110, umt,
                                 key_move_to_draw,
                                 onclick_move_to);


    img = img_cross_arrow_up1();
    disp_fill_img(umt->disp_touch, 150, 90, img);
    kmem_deref(&img);

    img = img_cross_arrow_down1();
    disp_fill_img(umt->disp_touch, 150, 190, img);
    kmem_deref(&img);

    img = img_longitudal_arrow_left1();
    disp_fill_img(umt->disp_touch, 50, 144, img);
    kmem_deref(&img);

    img = img_longitudal_arrow_right1();
    disp_fill_img(umt->disp_touch, 235, 144, img);
    kmem_deref(&img);

    img = img_cross_arrow_up_down();
    disp_fill_img(umt->disp_touch, 120, 326, img);
    kmem_deref(&img);

    img = img_longitudal_arrow_left_right();
    disp_fill_img(umt->disp_touch, 105, 410, img);
    kmem_deref(&img);

}

static void ui_move_to_destructor(void *mem)
{
    struct ui_move_to *umt = (struct ui_move_to *)mem;
    kmem_deref(&umt->disp_info);
    kmem_deref(&umt->disp_touch);
    kmem_deref(&umt->buttons);
}


void ui_move_to_run(void)
{
    struct machine *m = &machine;
    struct ui_move_to *umt;

    umt = kzref_alloc("ui_move_to", sizeof *umt, ui_move_to_destructor);
    ui_move_to = umt;
    umt->disp_touch = kmem_ref(m->disp1);
    umt->disp_info = kmem_ref(m->disp2);

    show();

    while (1) {
        yield();

        disp_button_handler();

        if (!is_switch_on(m->switch_move_to)) {
            kmem_deref(&umt);
            return;
        }
    }
}



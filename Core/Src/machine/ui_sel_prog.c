/*
 * disp_sel_prog.c
 *
 *  Created on: Jun 9, 2025
 *      Author: stelhs
 */
#include "ui_sel_prog.h"

#include "stm32_lib/kref_alloc.h"
#include "disp_mipi_dcs.h"
#include "images.h"
#include "machine.h"
#include "ui_button.h"


struct ui_sel_prog {
    struct disp *disp;
    struct ui_button *progs[14];
};

static struct ui_sel_prog *ui_sel_prog = NULL;

struct img *img_prog_by_num(enum progs prog)
{
    struct img *img;
    switch (prog) {
    case PROG_FEED_LEFT:
        img = img_feed_left(); break;
    case PROG_FEED_RIGHT:
        img = img_feed_right(); break;
    case PROG_FEED_LEFT_UP:
        img = img_feed_left_up(); break;
    case PROG_FEED_RIGHT_UP:
        img = img_feed_right_up(); break;
    case PROG_FEED_LEFT_DOWN:
        img = img_feed_left_down(); break;
    case PROG_FEED_RIGHT_DOWN:
        img = img_feed_right_down(); break;
    case PROG_FEED_UP_LEFT:
        img = img_feed_up_left(); break;
    case PROG_FEED_UP_RIGHT:
        img = img_feed_up_right(); break;
    case PROG_FEED_DOWN_LEFT:
        img = img_feed_down_left(); break;
    case PROG_FEED_DOWN_RIGHT:
        img = img_feed_down_right(); break;
    case PROG_FEED_UP:
        img = img_feed_up(); break;
    case PROG_FEED_DOWN:
        img = img_feed_down(); break;
    case PROG_THREAD_LEFT:
        img = img_thread_left(); break;
    case PROG_THREAD_RIGHT:
        img = img_thread_right(); break;
    default:
        panic("incorrect program selected");
        return NULL;
    }
    return img;
}

static void key_prog_show(struct ui_item *ut)
{
    struct ui_button *ub = (struct ui_button *)ut->priv;
    enum progs prog = (enum progs)ub->priv;
    struct img *img;
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, GRAY);
    img = img_prog_by_num(prog);
    disp_fill_img(ut->disp, ut->x + 2, ut->y + 3, img);
    kmem_deref(&img);
}


static void show(struct ui_sel_prog *usp)
{
    const int left_ident = 35;
    const int right_butt_ident = 180;
    const int top_ident = 2;
    const int key_ident = 15;
    const int key_width = 100;
    const int key_height = 55;
    int i;
    disp_clear(usp->disp);

    for (i = 0; i < 7; i++) {
        char *left, *right;
        switch(i) {
        case 0:
            left = "key_prog_feed_left";
            right = "key_prog_feed_right";
            break;
        case 1:
            left = "key_prog_feed_left_up";
            right = "key_prog_feed_right_up";
            break;
        case 2:
            left = "key_prog_feed_left_down";
            right = "key_prog_feed_right_down";
            break;
        case 3:
            left = "key_prog_feed_up_left";
            right = "key_prog_feed_up_right";
            break;
        case 4:
            left = "key_prog_feed_down_left";
            right = "key_prog_feed_down_right";
            break;
        case 5:
            left = "key_prog_feed_up";
            right = "key_prog_feed_down";
            break;
        case 6:
            left = "key_prog_thread_left";
            right = "key_prog_thread_right";
            break;
        }

        usp->progs[i * 2] =
                ui_button_register(left, usp->disp,
                                     left_ident,
                                     top_ident + i * (key_height + key_ident),
                                     key_width, key_height,
                                     key_prog_show, NULL, (void *)(i * 2));

        usp->progs[i * 2 + 1] =
                ui_button_register(right, usp->disp,
                                     right_butt_ident,
                                     top_ident + i * (key_height + key_ident),
                                     key_width, key_height,
                                     key_prog_show, NULL, (void *)(i * 2 + 1));
    }

}

static void ui_sel_prog_destructor(void *mem)
{
    struct ui_sel_prog *usp = (struct ui_sel_prog *)mem;
    int i;

    kmem_deref(&usp->disp);
    for (i = 0; i < ARRAY_SIZE(usp->progs); i++)
        kmem_deref(usp->progs + i);
}


int ui_sel_prog_run(void)
{
    struct machine *m = &machine;
    struct ui_sel_prog *usp = ui_sel_prog;

    usp = kzref_alloc("ui_sel_prog", sizeof *usp, ui_sel_prog_destructor);
    usp->disp = kmem_ref(m->disp1);

    show(usp);

    while (1) {
        int i;
        yield();

        for (i = 0; i < ARRAY_SIZE(usp->progs); i++) {
            struct ui_button *db = usp->progs[i];
            if (is_ui_button_touched(db)) {
                kmem_deref(&usp);
                return i;
            }
        }
    }

    return -1;
}

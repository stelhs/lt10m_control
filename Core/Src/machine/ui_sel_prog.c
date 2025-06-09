/*
 * disp_sel_prog.c
 *
 *  Created on: Jun 9, 2025
 *      Author: stelhs
 */
#include "ui_sel_prog.h"

#include "stm32_lib/kref_alloc.h"
#include "disp_mipi_dcs.h"
#include "disp_button.h"
#include "images.h"
#include "machine.h"


struct disp_sel_prog {
    struct disp *disp;
    struct disp_button *progs[14];
};

static struct disp_sel_prog *disp_sel_prog = NULL;

struct key_prog {
    enum progs prog;
};

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

static void key_prog_draw(struct disp_button *db)
{
    struct key_prog *kp = (struct key_prog *)db->priv;
    struct img *img;
    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, GRAY);
    img = img_prog_by_num(kp->prog);
    disp_fill_img(db->disp, db->x + 2, db->y + 3, img);
    kmem_deref(&img);
}


static void show(struct disp_sel_prog *dsp)
{
    struct disp_button *db;
    struct key_prog *kp;
    const int left_ident = 35;
    const int right_butt_ident = 180;
    const int top_ident = 2;
    const int key_ident = 15;
    const int key_width = 100;
    const int key_height = 55;
    int i;
    disp_clear(dsp->disp);

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
        db = disp_button_create(left, dsp->disp, key_width, key_height,
                                sizeof *kp);
        kp = (struct key_prog *)db->priv;
        kp->prog = (enum progs)i * 2;
        disp_button_show(db, left_ident,
                         top_ident + i * (key_height + key_ident),
                         key_prog_draw);
        dsp->progs[kp->prog] = db;

        db = disp_button_create(right, dsp->disp, key_width, key_height,
                                sizeof *kp);
        kp = (struct key_prog *)db->priv;
        kp->prog = (enum progs)i * 2 + 1;
        disp_button_show(db, right_butt_ident,
                         top_ident + i * (key_height + key_ident),
                         key_prog_draw);
        dsp->progs[kp->prog] = db;
    }

}

static void disp_sel_prog_destructor(void *mem)
{
    struct disp_sel_prog *dsp = (struct disp_sel_prog *)mem;
    int i;

    kmem_deref(&dsp->disp);
    for (i = 0; i < ARRAY_SIZE(dsp->progs); i++)
        kmem_deref(dsp->progs + i);
}


int ui_sel_prog_run(void)
{
    struct machine *m = &machine;
    struct disp_sel_prog *dsp = disp_sel_prog;

    dsp = kzref_alloc("disp_sel_prog", sizeof *dsp, disp_sel_prog_destructor);
    dsp->disp = kmem_ref(m->disp1);

    show(dsp);

    while (1) {
        int i;
        yield();

        for (i = 0; i < ARRAY_SIZE(dsp->progs); i++) {
            struct disp_button *db = dsp->progs[i];
            if (is_disp_button_touched(db)) {
                kmem_deref(&dsp);
                return i;
            }
        }
    }

    return -1;
}

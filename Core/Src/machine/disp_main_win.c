/*
 * disp_main_win.c
 *
 *  Created on: Jun 8, 2025
 *      Author: stelhs
 */

#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/kref_alloc.h"
#include "machine.h"
#include "disp_main_win.h"
#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"
#include "images.h"
#include "disp_button.h"
#include "disp_sel_prog.h"


struct main_win_items {
    struct disp_button *sel_prog;
    struct disp_button *sel_automatic;
    struct disp_button *sel_feed_limit;
};

struct disp_main_win {
    struct disp *disp;
    struct main_win_items *mwi;
    enum progs prog;
};
static struct disp_main_win *disp_main_win;


static void key_prog_sel_draw(struct disp_button *db)
{
    struct disp_main_win *dmw = (struct disp_main_win *)db->priv;
    struct img *img;
    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, GRAY);

    img = img_prog_by_num(dmw->prog);
    disp_fill_img(db->disp, db->x + 2, db->y + 3, img);
    kmem_deref(&img);
}

static void main_win_items_destructor(void *mem)
{
    struct main_win_items *mwi = (struct main_win_items *)mem;
    kmem_deref(&mwi->sel_prog);
    kmem_deref(&mwi->sel_automatic);
    kmem_deref(&mwi->sel_feed_limit);
}

static void show(struct disp_main_win *dmw)
{
    struct main_win_items *mwi;
    struct disp_button *db;
    mwi = kzref_alloc("main_win_items", sizeof *mwi,
                      main_win_items_destructor);
    dmw->mwi = mwi;

    disp_clear(dmw->disp);

    db = disp_button_create("key_prog_sel", dmw->disp, 100, 55, sizeof (void *));
    db->priv = dmw;
    disp_button_show(db, 30, 10, key_prog_sel_draw);
    mwi->sel_prog = db;
}

static void hide(struct disp_main_win *dmw)
{
    kmem_deref(&dmw->mwi);
}

static void disp_main_win_destructor(void *mem)
{
    struct disp_main_win *dmw = (struct disp_main_win *)mem;
    kmem_deref(&dmw->disp);
    kmem_deref(&dmw->mwi);
}

int disp_main_run(void)
{
    struct machine *m = &machine;
    struct disp_main_win *dmw = disp_main_win;
    struct main_win_items *mwi;

    dmw = kzref_alloc("disp_main_win", sizeof *dmw, disp_main_win_destructor);
    dmw->disp = kmem_ref(m->disp1);
    show(dmw);
    mwi = dmw->mwi;

    while (1) {
        yield();

        if (is_disp_button_touched(mwi->sel_prog)) {
            hide(dmw);
            dmw->prog = disp_sel_prog_run();
            show(dmw);
        }

    }
    return 0;
}

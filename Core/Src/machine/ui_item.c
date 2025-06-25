/*
 * ui_item_blink.c
 *
 *  Created on: Jun 24, 2025
 *      Author: stelhs
 */

#include "ui_item.h"
#include "stm32_lib/kref_alloc.h"
#include "machine.h"

static void hide_default(struct ui_item *ut)
{
    disp_fill(ut->disp, ut->x, ut->y, ut->width, ut->height, BLACK);
}

static void ui_item_destructor(void *mem)
{
    struct ui_item *ut = (struct ui_item *)mem;
    kmem_deref(&ut->tw);
}

struct ui_item *
ui_item_register(char *name, struct disp *disp,
                 int x, int y, int width, int height,
                 void (*show)(struct ui_item *),
                 void (*hide)(struct ui_item *), void *priv)
{
    struct ui_item *ut;

    ut = kzref_alloc(name, sizeof *ut, ui_item_destructor);
    ut->disp = disp;
    ut->x = x;
    ut->y = y;
    ut->width = width;
    ut->height = height;
    ut->show = show;
    ut->hide = hide;
    if (!hide)
        ut->hide = hide_default;
    ut->priv = priv;
    ut->show(ut);
    return ut;
}

static void on_periodic(void *priv)
{
    struct ui_item *ut = (struct ui_item *)priv;

    if (ut->is_blink_hide) {
        ut->show(ut);
        ut->is_blink_hide = FALSE;
        return;
    }
    ut->hide(ut);
    ut->is_blink_hide = TRUE;
}

void ui_item_blink(struct ui_item *ut, int interval)
{
    if (ut->tw)
        return;
    ut->tw = set_periodic("ui_item_periodic", interval,
                          on_periodic, ut, sizeof *ut);
}

void ui_item_blink_stop(struct ui_item *ut)
{
    if (!ut->tw)
        return;
    kmem_deref(&ut->tw);
    ut->show(ut);
}

void ui_item_hide(struct ui_item *ut)
{
    ut->is_show = FALSE;
    ut->hide(ut);
}

void ui_item_show(struct ui_item *ut)
{
    ut->is_show = TRUE;
    ut->show(ut);
}

bool ui_item_is_show(struct ui_item *ut)
{
    return ut->is_show;
}


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
                 void (*hide)(struct ui_item *), void *priv, size_t data_size)
{
    struct ui_item *ut;

    ut = kzref_alloc(name, (sizeof *ut) + data_size, ui_item_destructor);
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
    ut->data = (void *)(ut + 1);
    ut->show(ut);
    return ut;
}

struct ui_item_text {
    char *str;
    char *str_prev;
    size_t text_len;
    void (*getter)(struct ui_item *, char *, size_t);
    struct text_style ts;
};

static void text_show(struct ui_item *ut)
{
    struct ui_item_text *uit = ut->data;
    if (!uit->getter)
        return;

    uit->getter(ut, uit->str, uit->text_len + 1);
    if (strcmp(uit->str, uit->str_prev) == 0)
        return;
    if (strlen(uit->str) != strlen(uit->str_prev))
        ut->hide(ut);
    memcpy(uit->str_prev, uit->str, uit->text_len + 1);
    disp_text(ut->disp, uit->str, ut->x, ut->y, &uit->ts);
}

struct ui_item *
ui_item_text_register(char *name, struct disp *disp,
                      int x, int y, int text_len, struct text_style *ts,
                      void (*getter)(struct ui_item *, char *, size_t size),
                      void (*hide)(struct ui_item *), void *priv)
{
    struct ui_item *ut;
    struct ui_item_text uit;
    ut = ui_item_register(name, disp, x, y,
                          disp_text_width(ts, text_len),
                          disp_text_height(ts),
                          text_show, hide,
                          priv, (sizeof uit) + (text_len + 1) * 2);
    uit.getter = getter;
    uit.text_len = text_len;
    uit.ts = *ts;
    uit.str = (char *)((struct ui_item_text *)ut->data + 1);
    uit.str_prev = (char *)((struct ui_item_text *)ut->data + 1) + (text_len + 1);
    memcpy(ut->data, &uit, sizeof uit);
    ui_item_show(ut);
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

void ui_item_update(struct ui_item *ut)
{
    ui_item_hide(ut);
    ui_item_show(ut);
}

bool ui_item_is_show(struct ui_item *ut)
{
    return ut->is_show;
}


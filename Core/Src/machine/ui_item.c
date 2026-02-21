/*
 * ui_item_blink.c
 *
 *  Created on: Jun 24, 2025
 *      Author: stelhs
 */

#include "ui_item.h"
#include "stm32_lib/kref_alloc.h"
#include "machine.h"

void ui_scope_set_onchange_handler(struct ui_scope *us,
                                   void (*onchange)(struct ui_item *))
{
    us->onchange = onchange;
}

static void ui_scope_destructor(void *mem)
{
    struct ui_scope *us = (struct ui_scope *)mem;
    struct le *le, *le2;
    LIST_FOREACH_SAFE(&us->items, le, le2) {
        struct ui_item *ut = (struct ui_item *)list_ledata(le);
        kmem_deref(&ut);
    }
    list_clear(&us->items);
}

struct ui_scope *ui_scope_create(char *name)
{
    return kzref_alloc(name, sizeof (struct ui_scope), ui_scope_destructor);
}

void ui_item_hide_default_cb(struct ui_item *ut)
{
    disp_fill(ut->disp, ut->x, ut->y, ut->width, ut->height, BLACK);
}

static void ui_item_destructor(void *mem)
{
    struct ui_item *ut = (struct ui_item *)mem;
    list_unlink(&ut->scope_le);

    if (ut->data_destructor) {
        ut->data_destructor(ut);
        ut->data_destructor = NULL;
    }
    kmem_deref(&ut->tw);
}

struct ui_item *
ui_item_register(char *name, struct ui_scope *ui_scope, struct disp *disp,
                 int x, int y, int width, int height,
                 void (*show)(struct ui_item *),
                 void (*hide)(struct ui_item *), void *priv, size_t data_size)
{
    struct ui_item *ut;

    ut = kzref_alloc(name, (sizeof *ut) + data_size, ui_item_destructor);
    ut->name = name;
    ut->disp = disp;
    ut->x = x;
    ut->y = y;
    ut->width = width;
    ut->height = height;
    ut->show = show;
    ut->hide = hide;
    if (!hide)
        ut->hide = ui_item_hide_default_cb;
    ut->priv = priv;
    ut->data = (void *)(ut + 1);
    ut->is_redraw_needed = TRUE;
    if (ui_scope) {
        ut->ui_scope = ui_scope;
        list_append(&ui_scope->items, &ut->scope_le, ut);
    }
    if (!data_size)
        ut->show(ut);
    return ut;
}

void ui_item_set_data_destructor(struct ui_item *ut,
                                 void (*data_destructor)(void *))
{
    ut->data_destructor = data_destructor;
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
    if (!ut->is_redraw_needed) {
        if (strcmp(uit->str, uit->str_prev) == 0)
            return;
        if (strlen(uit->str) != strlen(uit->str_prev))
            ut->hide(ut);
    }
    memcpy(uit->str_prev, uit->str, uit->text_len + 1);
    disp_text(ut->disp, uit->str, ut->x, ut->y, &uit->ts);
    ut->is_redraw_needed = FALSE;
}

struct ui_item *
ui_item_text_register(char *name, struct ui_scope *ui_scope, struct disp *disp,
                      int x, int y, int text_len, struct text_style *ts,
                      void (*getter)(struct ui_item *, char *, size_t size),
                      void (*hide)(struct ui_item *), void *priv)
{
    struct ui_item *ut;
    struct ui_item_text uit;
    ut = ui_item_register(name, ui_scope, disp, x, y,
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
    ut->is_blink_hide = TRUE;
    ut->hide(ut);
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
    ut->is_redraw_needed = TRUE;
    ut->hide(ut);
}

void ui_item_show(struct ui_item *ut)
{
    ut->show(ut);
    ut->is_show = TRUE;
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

void ui_scope_show(struct ui_scope *ui_scope)
{
    struct le *le;
    LIST_FOREACH(&ui_scope->items, le) {
        struct ui_item *ut = (struct ui_item *)list_ledata(le);
        ui_item_show(ut);
    }
}

void ui_scope_print_items(struct ui_scope *ui_scope)
{
    struct le *le;
    printf("------\r\n");
    LIST_FOREACH(&ui_scope->items, le) {
        struct ui_item *ut = (struct ui_item *)list_ledata(le);
        printf("item: %s\r\n", ut->name);
    }
}

struct ui_item *ui_scope_item_by_name(struct ui_scope *ui_scope, char *name)
{
    struct le *le;
    LIST_FOREACH(&ui_scope->items, le) {
        struct ui_item *ut = (struct ui_item *)list_ledata(le);
        if (strcmp(ut->name, name) == 0)
            return ut;
    }
}

void ui_scope_hide(struct ui_scope *ui_scope)
{
    struct le *le;
    LIST_FOREACH(&ui_scope->items, le) {
        struct ui_item *ut = (struct ui_item *)list_ledata(le);
        ui_item_hide(ut);
    }
}

void ui_scope_update(struct ui_scope *ui_scope)
{
    struct le *le;
    LIST_FOREACH(&ui_scope->items, le) {
        struct ui_item *ut = (struct ui_item *)list_ledata(le);
        ui_item_update(ut);
    }
}

struct ui_item *ui_item_by_name(struct ui_scope *ui_scope, char *name)
{
    struct le *le;
    LIST_FOREACH(&ui_scope->items, le) {
        struct ui_item *ut = (struct ui_item *)list_ledata(le);
        if (strcmp(ut->name, name) == 0)
            return ut;
    }
    panic("ui_item_by_name(): item not found");
    return NULL;
}

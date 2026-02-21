/*
 * disp_button.c
 *
 *  Created on: Jun 7, 2025
 *      Author: stelhs
 */
#include "ui_button.h"

#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/buttons.h"
#include "ui_item.h"
#include "machine.h"

static struct list disp_buttons_list = LIST_INIT;

struct ui_button {
    char *name;
    struct le le;
    struct touch_area *ta;
    int (*on_click)(struct ui_item *);
    struct ui_item *ut;
    void *data;
    void (*show)(struct ui_item *);
    bool is_lock;
};

static struct ui_item *action_confirmation_ut = NULL;
struct ui_button_confirmation_args {
    void (*onclick)(struct ui_item *);
};


static void ui_button_confirmation_handler(void)
{
    struct machine *m = &machine;
    struct ui_button_confirmation_args *args;
    if (!action_confirmation_ut)
        return;

    if (!is_button_clicked(m->btn_ok))
        return;

    args = (struct ui_button_confirmation_args *)action_confirmation_ut->data;
    args->onclick(action_confirmation_ut);
    ui_item_blink_stop(action_confirmation_ut);
    action_confirmation_ut = NULL;
}

static int onclick_action_confirmation(struct ui_item *ut)
{
    struct machine *m = &machine;
    if (ut == action_confirmation_ut) {
        ui_item_blink_stop(ut);
        action_confirmation_ut = NULL;
        return FALSE;
    }

    if (action_confirmation_ut)
        ui_item_blink_stop(action_confirmation_ut);

    button_reset(m->btn_ok);
    action_confirmation_ut = ut;
    ui_item_blink(ut, 300);
    return FALSE;
}


static void ui_button_destructor(void *mem)
{
    struct ui_item *ut = (struct ui_item *)mem;
    struct ui_button *ub = (struct ui_button *)ut->data;
    if (ut == action_confirmation_ut)
        action_confirmation_ut = NULL;

    kmem_deref(&ub->ta);
    list_unlink(&ub->le);
}

static void button_show(struct ui_item *ut)
{
    struct ui_button *ub = (struct ui_button *)ut->data;
    if (ub->ta)
        ub->ta->is_lock = FALSE;
    if (ub->show)
        ub->show(ut);
}

static void button_hide(struct ui_item *ut)
{
    struct ui_button *ub = (struct ui_button *)ut->data;
    ub->ta->is_lock = TRUE;
    ui_item_hide_default_cb(ut);
}

void ui_button_lock(struct ui_item *ut)
{
    struct ui_button *ub = (struct ui_button *)ut->data;
    ub->is_lock = TRUE;
}

void ui_button_unlock(struct ui_item *ut)
{
    struct ui_button *ub = (struct ui_button *)ut->data;
    ub->is_lock = FALSE;
}

void ui_buttons_lock(struct ui_scope *ui_scope)
{
    struct le *le;
    LIST_FOREACH(&ui_scope->items, le) {
        struct ui_item *ut = (struct ui_item *)list_ledata(le);
        if (ut->is_button)
            ui_button_lock(ut);
    }
}

void ui_buttons_unlock(struct ui_scope *ui_scope)
{
    struct le *le;
    LIST_FOREACH(&ui_scope->items, le) {
        struct ui_item *ut = (struct ui_item *)list_ledata(le);
        if (ut->is_button)
            ui_button_unlock(ut);
    }
}


struct ui_item *
ui_button_register(char *name, struct ui_scope *ui_scope,
                   int x, int y, int width, int height,
                   void (*show)(struct ui_item *),
                   int (*on_click)(struct ui_item *), void *priv,
                   size_t data_size)
{
    struct machine *m = &machine;
    struct ui_item *ut;
    struct ui_button *ub;

    ut = ui_item_register(name, ui_scope, m->disp1,
                              x, y, width, height,
                              button_show, button_hide, priv,
                              (sizeof *ub) + data_size);
    ut->is_button = TRUE;
    ub = (struct ui_button *)ut->data;
    ub->data = (void *)(ub + 1);
    ub->ta = touch_area_register(m->disp1->touch, "disp_butt_touch",
                                 x, y, x + width, y + height);
    ub->on_click = on_click;
    ub->ut = ut;
    ub->show = show;
    ub->name = name;
    ui_item_set_data_destructor(ut, ui_button_destructor);
    list_append(&disp_buttons_list, &ub->le, ub);
    if (!data_size)
        ui_item_show(ut);
    return ut;
}

struct ui_item *
ui_button_confirmation_register(char *name, struct ui_scope *ui_scope,
                                int x, int y, int width, int height,
                                void (*show)(struct ui_item *),
                                void (*onclick)(struct ui_item *),
                                void *priv)
{
    struct ui_item *ut;
    struct ui_button_confirmation_args *args;
    ut = ui_button_register(name, ui_scope,
                            x, y, width, height, show,
                            onclick_action_confirmation, priv, sizeof *args);
    args = (struct ui_button_confirmation_args *)ut->data;
    args->onclick = onclick;
    ui_item_show(ut);
    return ut;
}

bool is_ui_button_touched(struct ui_item *ut)
{
    struct ui_button *ub = (struct ui_button *)ut->data;
    return is_area_touched(ub->ta);
}


void ui_button_handler(void)
{
    struct le *le, *le2;

    ui_button_confirmation_handler();

    LIST_FOREACH_SAFE(&disp_buttons_list, le, le2) {
        struct ui_button *ub = (struct ui_button *)list_ledata(le);
        if (!ub->on_click)
            continue;
        if (is_area_touched(ub->ta)) {
            if (ui_item_is_show(ub->ut) && (!ub->is_lock)) {
                struct ui_scope *us = ub->ut->ui_scope;
                int rc = ub->on_click(ub->ut);
                if (!rc && us && us->onchange)
                    us->onchange(ub->ut);
            }
            return;
        }
    }
}

void *ui_button_data(struct ui_item *ut)
{
    struct ui_button *ub = (struct ui_button *)ut->data;
    return ub->data;
}

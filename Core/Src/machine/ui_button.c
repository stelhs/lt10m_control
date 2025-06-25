/*
 * disp_button.c
 *
 *  Created on: Jun 7, 2025
 *      Author: stelhs
 */
#include "ui_button.h"

#include "stm32_lib/kref_alloc.h"
#include "ui_item.h"
#include "machine.h"

static struct list disp_buttons_list = LIST_INIT;

static void disp_button_destructor(void *mem)
{
    struct ui_button *ub = (struct ui_button *)mem;
    kmem_deref(&ub->ut);
    list_unlink(&ub->le);
}

struct ui_button *
ui_button_register(char *name,
                   struct disp *disp,
                   int x, int y, int width, int height,
                   void (*show)(struct ui_item *),
                   void (*on_click)(void *), void *priv)
{
    struct ui_button *ub;
    if (!disp->touch)
        panic("Can't create a button on the display without touch");

    ub = kzref_alloc(name, sizeof *ub, disp_button_destructor);
    ub->name = name;
    ub->priv = priv;
    ub->disp = disp;
    ub->on_click = on_click;
    ub->ut = ui_item_register("ui_item_button", disp, x, y, width, height,
                              (void (*)(struct ui_item *))show, NULL, ub);
    ub->ta = touch_area_register(ub->disp->touch, "disp_butt_touch",
                                 x, y, x + width, y + height);
    ui_item_show(ub->ut);
    list_append(&disp_buttons_list, &ub->le, ub);
    return ub;
}

bool is_ui_button_touched(struct ui_button *ub)
{
    if (!ub)
        return FALSE;
    return is_area_touched(ub->ta);
}

void ui_button_hide(struct ui_button *ub)
{
    ui_item_hide(ub->ut);
}

void ui_button_show(struct ui_button *ub)
{
    ui_item_show(ub->ut);
}


void ui_button_handler(void)
{
    struct le *le, *le2;
    LIST_FOREACH_SAFE(&disp_buttons_list, le, le2) {
        struct ui_button *ub = (struct ui_button *)list_ledata(le);
        if (!ub->on_click)
            continue;
        if (is_area_touched(ub->ta)) {
            if (ui_item_is_show(ub->ut))
                ub->on_click(ub->priv);
            return;
        }
    }
}


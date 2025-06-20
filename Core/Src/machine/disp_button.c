/*
 * disp_button.c
 *
 *  Created on: Jun 7, 2025
 *      Author: stelhs
 */
#include "stm32_lib/kref_alloc.h"
#include "disp_button.h"
#include "machine.h"

static struct list disp_buttons_list = LIST_INIT;

static void disp_button_destructor(void *mem)
{
    struct disp_button *db = (struct disp_button *)mem;
    kmem_deref(&db->disp);
    if (db->ta)
        kmem_deref(&db->ta);
    list_unlink(&db->le);
}

struct disp_button *
disp_button_register(char *name,
                     struct disp *disp,
                     int x, int y,
                     int width, int height,
                     void *priv,
                     void (*draw)(struct disp_button *db),
                     void (*on_click)(void *))
{
    struct disp_button *db;
    if (!disp->touch)
        panic("Can't create a button on the display without touch");
    db = kzref_alloc(name, sizeof *db, disp_button_destructor);
    db->name = name;
    db->priv = priv;
    db->x = x;
    db->y = y;
    db->width = width;
    db->height = height;
    db->disp = kmem_ref(disp);
    db->draw = draw;
    db->on_click = on_click;
    db->draw(db);
    db->ta = touch_area_register(db->disp->touch, "disp_butt_touch",
                                 x, y, x + db->width, y + db->height);
    list_append(&disp_buttons_list, &db->le, db);
    return db;
}

void disp_button_redraw(struct disp_button *db)
{
    db->draw(db);
}

bool is_disp_button_touched(struct disp_button *db)
{
    if (!db)
        return FALSE;
    return is_area_touched(db->ta);
}

void disp_button_handler(void)
{
    struct le *le;
    LIST_FOREACH(&disp_buttons_list, le) {
        struct disp_button *db = (struct disp_button *)list_ledata(le);
        if (!db->on_click)
            continue;
        if (is_area_touched(db->ta))
            db->on_click(db->priv);
    }
}

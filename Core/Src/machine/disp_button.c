/*
 * disp_button.c
 *
 *  Created on: Jun 7, 2025
 *      Author: stelhs
 */
#include "stm32_lib/kref_alloc.h"
#include "disp_button.h"
#include "machine.h"

static void disp_button_destructor(void *mem)
{
    struct disp_button *db = (struct disp_button *)mem;
    kmem_deref(&db->disp);
    if (db->ta)
        kmem_deref(&db->ta);
}

struct disp_button *
disp_button_create(char *name,
                   struct disp *disp,
                   int width, int height,
                   size_t priv_size)
{
    struct disp_button *db;
    if (!disp->touch)
        panic("Can't create a button on the display without touch");
    db = kzref_alloc(name, sizeof *db + priv_size, disp_button_destructor);
    db->name = name;
    db->priv = (void *)(db + 1);
    db->width = width;
    db->height = height;
    db->disp = kmem_ref(disp);
    return db;
}


void disp_button_show(struct disp_button *db, int x, int y,
                        void (*draw)(struct disp_button *db))
{
    db->draw = draw;
    db->x = x;
    db->y = y;
    db->draw(db);
    db->ta = touch_area_register(db->disp->touch, "disp_butt_touch",
                                 x, y, x + db->width, y + db->height);
}

void disp_button_redraw(struct disp_button *db)
{
    db->draw(db);
}

bool is_disp_button_touched(struct disp_button *db)
{
    return is_area_touched(db->ta);
}


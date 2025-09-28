#ifndef MENU_DRAW_H
#define MENU_DRAW_H

#include <stdint.h>
#include <stdbool.h>

#include "menu_types.h"

bool menu_draw_update(menu_context_t *ctx, menu_id_t id);
void menu_draw_string_fixed_value_cb(menu_context_t *ctx, menu_id_t id);
void menu_draw_udword_factor_value_cb(menu_context_t *ctx, menu_id_t id);
void menu_draw_ubyte_simple_value_cb(menu_context_t *ctx, menu_id_t id);

#endif /* MENU_DRAW_H */

#ifndef MENU_EDIT_H
#define MENU_EDIT_H

#include <stdint.h>
#include <stdbool.h>

#include "menu_types.h"

void menu_edit_string_fixed_click_cyclic_cb(menu_context_t *ctx, menu_id_t id);
void menu_edit_string_fixed_position_cyclic_cb(menu_context_t *ctx, menu_id_t id, int8_t delta);
void menu_edit_udword_factor_click_cyclic_cb(menu_context_t *ctx, menu_id_t id);
void menu_edit_udword_factor_position_limit_cb(menu_context_t *ctx, menu_id_t id, int8_t delta);
void menu_edit_ubyte_simple_click_cyclic_cb(menu_context_t *ctx, menu_id_t id);
void menu_edit_ubyte_simple_position_limit_cb(menu_context_t *ctx, menu_id_t id, int8_t delta);

#endif /* MENU_EDIT_H */
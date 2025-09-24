#ifndef MENU_EDIT_H
#define MENU_EDIT_H

#include <stdint.h>
#include <stdbool.h>

#include "menu_struct.h"
#include "menu_config.h"

void string_fixed_click_cyclic_cb(menu_id_t id);
void string_fixed_position_cyclic_cb(menu_id_t id, int8_t delta);
void udword_factor_click_cyclic_cb(menu_id_t id);
void udword_factor_position_limit_cb(menu_id_t id, int8_t delta);
void ubyte_simple_click_cyclic_cb(menu_id_t id);
void ubyte_simple_position_limit_cb(menu_id_t id, int8_t delta);

#endif /* MENU_EDIT_H */
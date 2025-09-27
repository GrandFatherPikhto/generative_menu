#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <stdbool.h>

#include "menu_common.h"

const menu_context_t *menu_context_init(void);
menu_id_t menu_engine_get_sibling(menu_context_t *ctx, menu_id_t id, int8_t delta);
void menu_engine_handle_position(menu_context_t *ctx, menu_id_t id, int8_t delta);
void menu_engine_handle_enter(menu_context_t *ctx, menu_id_t id);
void menu_engine_handle_out(menu_context_t *ctx, menu_id_t id);
void menu_engine_go_to(menu_context_t *ctx, menu_id_t id);

#endif // MENU_H
#ifndef MENU_VALUE_H
#define MENU_VALUE_H

#include <stdint.h>
#include <stdbool.h>

#include "menu_type.h"

typedef struct stub_value_t {} stub_value_t;
typedef struct {
    uint8_t idx;
} string_fixed_value_t;
typedef struct {
    void *value_ptr;
} callback_callback_value_t;
typedef struct {
    uint8_t idx;
    uint32_t value;
} udword_factor_value_t;
typedef struct {
    uint8_t value;
} ubyte_simple_value_t;

typedef struct menu_node_value {
    menu_id_t id;
    union {
        stub_value_t stub_value;
        string_fixed_value_t string_fixed;
        callback_callback_value_t callback_callback;
        udword_factor_value_t udword_factor;
        ubyte_simple_value_t ubyte_simple;
    } data;
} menu_node_value_t;

menu_node_value_t *menu_value_get_by_id(menu_context_t *ctx, menu_id_t id);

uint8_t *menu_value_get_string_fixed_idx(menu_context_t *ctx, menu_id_t id);
uint32_t *menu_value_get_udword_factor_value(menu_context_t *ctx, menu_id_t id);
uint8_t *menu_value_get_udword_factor_idx(menu_context_t *ctx, menu_id_t id);
uint8_t *menu_value_get_ubyte_simple_value(menu_context_t *ctx, menu_id_t id);

#endif // MENU_VALUE_H
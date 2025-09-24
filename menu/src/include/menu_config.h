#ifndef MENU_CONFIG_H
#define MENU_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#include "menu_struct.h"
#include "menu_edit.h"

typedef struct stub_config {} stub_config_t;

// 
typedef struct {
    uint8_t count;
    uint8_t default_idx;
    const char *values;
} string_fixed_config_t;
// 
typedef struct {
    uint32_t max;
    uint32_t min;
    uint32_t step;
    uint32_t default_value;
    uint8_t count;
    uint8_t default_idx;
    const uint32_t *factors;
} udword_factor_config_t;
// 
typedef struct {
    uint8_t default_value;
    uint8_t step;
    uint8_t min;
    uint8_t max;
} ubyte_simple_config_t;

typedef struct menu_config {
    menu_id_t id;
    void (*click_cb)(menu_id_t id);
    void (*position_cb)(menu_id_t id, int8_t delta);
    union {        
        stub_config_t stub_config;
        string_fixed_config_t string_fixed;
        udword_factor_config_t udword_factor;
        ubyte_simple_config_t ubyte_simple;
    } data;
} menu_item_config_t;

const menu_id_t get_first_id(void);
const menu_item_config_t *menu_get_config(menu_id_t id);


#endif /* MENU_CONFIG_H */
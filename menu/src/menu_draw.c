#include "menu_draw.h"
#include "menu_context.h"
#include "menu_tree.h"
#include "menu_config.h"
#include "menu_value.h"

#include <string.h>
#include <stdio.h>

bool menu_draw_update(menu_context_t *ctx, menu_id_t id) {
    
    if (id >= MENU_ID_COUNT || ctx->dirty == false)
        return false;

    const menu_node_t *node = menu_tree_get_by_id(ctx, id);
    const menu_node_config_t *config = menu_config_get_by_id(ctx, id);

    char *title_str = menu_context_get_title_str(ctx);
    char *value_str = menu_context_get_value_str(ctx);
    
    memset((void *)title_str, 0, LCD_STRING_LEN);
    memset((void *)value_str, 0, LCD_STRING_LEN);

    strncpy((char *)title_str, node->title, LCD_STRING_LEN);

    if (node->child == MENU_ID_COUNT) {
        if (config->draw_value_cb) {
            config->draw_value_cb(ctx, id);
        }
    } else {
        value_str[0] = '>';
    }

    menu_context_reset_dirty(ctx);
    menu_context_set_update(ctx);
}

void menu_draw_string_fixed_value_cb(menu_context_t *ctx, menu_id_t id) {
    const char *value_str = menu_context_get_value_str(ctx);
    uint8_t *idx  = menu_value_get_string_fixed_idx(ctx, id);
    const char* value = menu_config_get_string_fixed_current(ctx, id, *idx);
    int len = snprintf(NULL, 0, "%u. %s", *idx + 1, value);
    snprintf((char *)value_str, LCD_STRING_LEN, "%u. %s%*c",
        *idx + 1,
        value,
        15 - len > 0 ? 15 - len : 0,
        menu_context_get_state(ctx) == MENU_STATE_EDIT ? '*' : '>'
    );
}


void menu_draw_udword_factor_value_cb(menu_context_t *ctx, menu_id_t id) {
    const char *value_str = menu_context_get_value_str(ctx);
    uint32_t max = menu_config_get_udword_factor_max(ctx, id);
    uint32_t *value = menu_value_get_udword_factor_value(ctx, id);
    uint8_t *idx = menu_value_get_udword_factor_idx(ctx, id);
    uint32_t factor = menu_config_get_udword_factor_current(ctx, id, *idx);

    int len = snprintf(NULL, 0, "%u (x%u)", *value, factor);
    snprintf((char *)value_str, LCD_STRING_LEN, "%u (x%u)%*c",
        *value,
        factor,
        15 - len > 0 ? 15 - len : 0,
        menu_context_get_state(ctx) == MENU_STATE_EDIT ? '*' : '>'
    );
}
void menu_draw_ubyte_simple_value_cb(menu_context_t *ctx, menu_id_t id) {
    const char *value_str = menu_context_get_value_str(ctx);
    uint8_t min = menu_config_get_ubyte_simple_min(ctx, id);
    uint8_t max = menu_config_get_ubyte_simple_max(ctx, id);
    uint8_t *value = menu_value_get_ubyte_simple_value(ctx, id);

    int len = snprintf(NULL, 0, "%u/%u", *value, max);
    snprintf((char *)value_str, LCD_STRING_LEN, "%u/%u%*c",
        *value,
        max,
        15 - len > 0 ? 15 - len : 0,
        menu_context_get_state(ctx) == MENU_STATE_EDIT ? '*' : '>'
    );
}


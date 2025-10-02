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

    const menu_node_t *node = &(ctx->nodes[id]);
    const menu_node_config_t *config = &(ctx->configs[id]);

    char *title_str = ctx->title_buf;
    char *value_str = ctx->value_buf;
    
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

    ctx->dirty = false;
    ctx->invalidate = true;

    return ctx->invalidate;
}

void menu_draw_value_edit_sign(menu_context_t *ctx, menu_id_t id) {
    for (uint8_t i = 0; i < 15; i++) {
        if (ctx->value_buf[i] == 0) {
            ctx->value_buf[i] = 0x20;
        }
    }
    if (ctx->state == MENU_STATE_EDIT) {
        ctx->value_buf[15] = '*';
    } else {
        ctx->value_buf[15] = '>';
    }
}

/**
 * Это автогененируемые функции. Имеет смысл приводить их имена к виду menu_draw_...?
 */
void menu_draw_string_fixed_value_cb(menu_context_t *ctx, menu_id_t id) {
    const menu_node_config_t *config = &(ctx->configs[id]);
    menu_node_value_t *value = &(ctx->values[id]);
    uint8_t idx = value->data.string_fixed.idx;
    const char *value_str = ctx->value_buf;

    snprintf((char *)value_str, LCD_STRING_LEN, "%-15.15s%c", 
        config->data.string_fixed.values[idx], 
        ctx->state == MENU_STATE_EDIT ? '*' : '>');
}

void menu_draw_udword_factor_value_cb(menu_context_t *ctx, menu_id_t id) {
    const menu_node_config_t *config = &(ctx->configs[id]);
    menu_node_value_t *value = &(ctx->values[id]);
    const char *value_str = ctx->value_buf;

    uint8_t idx = value->data.udword_factor.idx;
    uint32_t factor = config->data.udword_factor.factors[idx];
    uint32_t val = value->data.udword_factor.value;
    uint32_t max = config->data.udword_factor.max;

    int len = snprintf(NULL, 0, "%u (x%u)", val, factor);
    snprintf((char *)value_str, LCD_STRING_LEN, "%u (x%u)%*c", 
        val, factor, 15 - len > 0 ? 15 - len : 0,
        ctx->state == MENU_STATE_EDIT ? '*' : '>'
    );
}

void menu_draw_ubyte_simple_value_cb(menu_context_t *ctx, menu_id_t id) {
    const menu_node_config_t *config = &(ctx->configs[id]);
    menu_node_value_t *value = &(ctx->values[id]);
    const char *value_str = ctx->value_buf;

    uint8_t val = value->data.ubyte_simple.value;
    uint8_t max = config->data.ubyte_simple.max;

    int len = snprintf(NULL, 0, "%u/%u", val, max);
    snprintf((char *)value_str, LCD_STRING_LEN, "%u/%u%*c", 
        val, max, 15 - len > 0 ? 15 - len : 0,
        ctx->state == MENU_STATE_EDIT ? '*' : '>'
    );
}

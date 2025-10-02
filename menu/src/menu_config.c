#include "menu_edit.h"
#include "menu_context.h"
#include "menu_config.h"

const menu_node_config_t *menu_config_get_by_id(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return NULL;
    return &(ctx->configs[id]);
}

bool menu_config_handle_position_cb(menu_context_t *ctx, menu_id_t id, int8_t delta) {
    if (id >= MENU_ID_COUNT)
        return false;

    if (ctx->configs[id].position_cb) {
        ctx->configs[id].position_cb(ctx, id, delta);
        return true;
    }

    return false;
}

bool menu_config_handle_click_cb(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return false;

    if (ctx->configs[id].click_cb) {
        ctx->configs[id].click_cb(ctx, id);
        return true;
    }

    return false;
}

bool menu_config_handle_long_click_cb(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return false;

    if (ctx->configs[id].long_click_cb) {
        ctx->configs[id].long_click_cb(ctx, id);
        return true;
    }

    return false;
}

bool menu_config_handle_double_click_cb(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return false;

    if (ctx->configs[id].double_click_cb) {
        ctx->configs[id].double_click_cb(ctx, id);
        return true;
    }

    return false;
}

bool menu_config_handle_draw_value_cb(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return false;

    if (ctx->configs[id].draw_value_cb) {
        ctx->configs[id].draw_value_cb(ctx, id);
        return true;
    }

    return false;
}

bool menu_config_handle_event_cb(menu_context_t *ctx, menu_id_t id, menu_event_t event) {
    if (id >= MENU_ID_COUNT)
        return false;

    if (ctx->configs[id].event_cb) {
        ctx->configs[id].event_cb(ctx, id, event);
        return true;
    }

    return false;
}

uint8_t menu_config_get_string_fixed_count(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return ctx->configs[id].data.string_fixed.count;
}

uint8_t menu_config_get_string_fixed_default_idx(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return ctx->configs[id].data.string_fixed.default_idx;
}

const char* menu_config_get_string_fixed_current(menu_context_t *ctx, menu_id_t id, uint8_t idx) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return ctx->configs[id].data.string_fixed.values[idx];
}

uint8_t menu_config_get_udword_factor_count(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return ctx->configs[id].data.udword_factor.count;
}

uint8_t menu_config_get_udword_factor_default_idx(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return ctx->configs[id].data.udword_factor.default_idx;
}

uint32_t menu_config_get_udword_factor_min(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return ctx->configs[id].data.udword_factor.min;
}

uint32_t menu_config_get_udword_factor_max(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return ctx->configs[id].data.udword_factor.max;
}

uint32_t menu_config_get_udword_factor_current(menu_context_t *ctx, menu_id_t id, uint8_t factor_idx) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return ctx->configs[id].data.udword_factor.factors[factor_idx];
}

uint32_t menu_config_get_udword_factor_default_value(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return ctx->configs[id].data.udword_factor.default_value;
}

uint8_t menu_config_get_ubyte_simple_min(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return ctx->configs[id].data.ubyte_simple.min;
}

uint8_t menu_config_get_ubyte_simple_max(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return ctx->configs[id].data.ubyte_simple.max;
}

uint8_t menu_config_get_ubyte_simple_step(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return ctx->configs[id].data.ubyte_simple.step;
}

uint8_t menu_config_get_ubyte_simple_default_value(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return ctx->configs[id].data.ubyte_simple.default_value;
}


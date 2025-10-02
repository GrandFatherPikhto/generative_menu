#include "menu_value.h"
#include "menu_context.h"

menu_node_value_t *menu_value_get_by_id(menu_context_t *ctx, menu_id_t id) {
    if (ctx == NULL || id >= MENU_ID_COUNT)
        return NULL;

    return &(ctx->values[id]);
}

uint8_t *menu_value_get_string_fixed_idx(menu_context_t *ctx, menu_id_t id) {
    if (ctx == NULL || id >= MENU_ID_COUNT)
        return NULL;
    return &(ctx->values[id].data.string_fixed.idx);
}
uint32_t *menu_value_get_udword_factor_value(menu_context_t *ctx, menu_id_t id) {
    if (ctx == NULL || id >= MENU_ID_COUNT)
        return NULL;
    return &(ctx->values[id].data.udword_factor.value);
}

uint8_t *menu_value_get_udword_factor_idx(menu_context_t *ctx, menu_id_t id) {
    if (ctx == NULL || id >= MENU_ID_COUNT)
        return NULL;
    return &(ctx->values[id].data.udword_factor.idx);
}

uint8_t *menu_value_get_ubyte_simple_value(menu_context_t *ctx, menu_id_t id) {
    if (ctx == NULL || id >= MENU_ID_COUNT)
        return NULL;
    return &(ctx->values[id].data.ubyte_simple.value);
}



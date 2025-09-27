#include "menu_engine.h"
#include "menu_config.h"
#include "menu_tree.h"
#include "menu_values.h"
#include "menu_context.h"
#include "menu_draw.h"

#include <stdio.h>

void menu_engine_handle_position(menu_context_t *ctx, menu_id_t id, int8_t delta) {
    // printf("[%s]\t%s:%d id = %s, delta = %d\n", __FUNCTION__, __FILE__, __LINE__, menu_items[id].name, delta);
    if (ctx->state == MENU_STATE_NAVIGATION) {
        menu_id_t sibling_id = menu_engine_get_sibling(ctx, id, delta);
        // printf("[%s]\t%s:%d sibling = %s, delta = %d\n", __FUNCTION__, __FILE__, __LINE__, menu_items[sibling_id].name, delta);
        menu_engine_go_to(ctx, sibling_id);
    } else if (ctx->state == MENU_STATE_EDIT && ctx->configs[id].position_cb != 0) {
        ctx->configs[id].position_cb(ctx, id, delta);
    }
}

void menu_engine_handle_enter(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return;
    const menu_node_config_t *config = &(ctx->configs[id]);
    const menu_node_t *node = &(ctx->nodes[id]);
    if (config == 0 || node == 0)
        return;
    // printf("[%s]\t%s:%d id = %s/%s, state = %d\n", __FUNCTION__, __FILE__, __LINE__, menu_items[id].name, menu_items[node->child].name, ctx->state);

    if (node->child == MENU_ID_COUNT) {
        if (ctx->state == MENU_STATE_NAVIGATION) {
            ctx->state = MENU_STATE_EDIT;
            ctx->dirty = true;
        } else if (ctx->state == MENU_STATE_EDIT && ctx->configs[id].click_cb != 0) {
            ctx->configs[id].click_cb(ctx, id);
            ctx->dirty = true;
        }
        // printf("[%s]\t%s:%d id = %s/%s, state = %d\n", __FUNCTION__, __FILE__, __LINE__, menu_items[id].name, menu_items[node->child].name, ctx->state);
    } else {
        menu_id_t target_id = node->child;
        if (target_id != MENU_ID_COUNT)
            menu_engine_go_to(ctx, target_id);
        // printf("[%s]\t%s:%d id = %s/%s, state = %d\n", __FUNCTION__, __FILE__, __LINE__, menu_items[id].name, menu_items[target_id], ctx->state);
    }
}

void menu_engine_handle_out(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return;
    const menu_node_config_t *config = &(ctx->configs[id]);
    const menu_node_t *node = &(ctx->nodes[id]);
    if (config == 0 || node == 0)
        return;

    if (node->type == MENU_TREE_TYPE_LEAF) {
        if (ctx->state == MENU_STATE_EDIT) {
            ctx->state = MENU_STATE_NAVIGATION;
            ctx->dirty = true;
        } else if (ctx->state == MENU_STATE_NAVIGATION) {
            menu_id_t target_id = node->parent;
            if (target_id != MENU_ID_COUNT)
                menu_engine_go_to(ctx, target_id);
        }
    }
}

void menu_engine_go_to(menu_context_t *ctx, menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return;
    if (id == ctx->current)
        return;

    ctx->previous = ctx->current;
    ctx->current = id;
    ctx->dirty = true;
}

menu_id_t menu_engine_get_sibling(menu_context_t *ctx, menu_id_t id, int8_t delta) {
    if (delta == 0 || id >= MENU_ID_COUNT) return MENU_ID_COUNT;

    bool is_back = delta > 0;
    int8_t steps = is_back ? delta : -delta;
    
    menu_id_t current_id = id;
    for (int8_t i = 0; i < steps; i++) {
        const menu_node_t *node = &(ctx->nodes[current_id]);
        if (node == 0)
            return MENU_ID_COUNT;

        menu_id_t next_id = is_back ? node->prev : node->next;
        
        if (next_id == MENU_ID_COUNT) {
            return current_id; // Останавливаемся на последнем валидном пункте
        }
        current_id = next_id;
    }

    return current_id;
}


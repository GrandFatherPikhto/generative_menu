#include "menu_context.h"
#include "menu_engine.h"
#include "menu_tree.h"
#include "menu_config.h"
#include "menu_values.h"

static menu_context_t *s_context = 0;

const menu_context_t *menu_context_init(void) {
    static menu_context_t context = {0};

    context.nodes = menu_tree_get_nodes();
    context.configs = menu_config_get_nodes();
    context.values = menu_values_get_nodes();
    context.previous = MENU_ID_COUNT;
    context.current = menu_tree_get_first_id();
    context.dirty = false;
    context.invalidate = false;
    context.state = MENU_STATE_NAVIGATION;

    s_context = &context;

    return s_context;
}

void menu_context_set_dirty(bool dirty) {
    s_context->dirty = dirty;
}

bool menu_context_is_invalidate(void) {
    return s_context->invalidate;
}

void menu_context_reset_invalidate(void) {
    s_context->invalidate = false;
}

void menu_context_set_node_tree(const menu_node_t *tree) {
    s_context->nodes = tree;
}

void menu_context_set_node_configs(const menu_node_config_t *configs) {
    s_context->configs = configs;
}

void menu_context_set_node_values(menu_node_value_t *values) {
    s_context->values = values;
}

menu_context_t *menu_get_context(void) {
    return s_context;
}

char *menu_context_get_title_str(void) {
    return s_context->title_buf;
}

char *menu_context_get_value_str(void) {
    return s_context->value_buf;
}

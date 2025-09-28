#include "menu_context.h"
#include "menu_navigate.h"
#include "menu_tree.h"
#include "menu_config.h"
#include "menu_value.h"

static menu_context_t *s_context = 0;

void menu_context_init(menu_context_t *context) {

    context->nodes = menu_tree_get_nodes();
    context->configs = menu_config_get_nodes();
    context->values = menu_value_get_nodes();
    context->previous = MENU_ID_COUNT;
    context->current = menu_tree_get_first_id();
    context->dirty = false;
    context->invalidate = false;
    context->state = MENU_STATE_NAVIGATION;

    s_context = context;
}

menu_context_t *menu_context_get(void) {
    return s_context;
}

char *menu_context_get_title_str(menu_context_t *context) {
    return context->title_buf;
}

char *menu_context_get_value_str(menu_context_t *context) {
    return context->value_buf;
}

char *menu_context_set_invalidate(menu_context_t *context, bool need_redraw) {
    context->invalidate = need_redraw;
}

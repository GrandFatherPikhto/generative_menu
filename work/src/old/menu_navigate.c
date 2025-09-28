#include "menu_navigate.h"
#include "menu_context.h"
#include "menu_engine.h"
#include "menu_draw.h"

void menu_navigate_update(void) {
    menu_context_t *ctx = menu_get_context();
    if (ctx->dirty) {
        menu_draw_update(ctx, ctx->current);
        ctx->dirty = false;
    }
}

void menu_navigate_enter(void) {
    menu_context_t *ctx = menu_get_context();
    menu_navigate_handle_enter(ctx, ctx->current);
}

void menu_navigate_out(void) {
    menu_context_t *ctx = menu_get_context();
    menu_navigate_handle_back(ctx, ctx->current);
}

void menu_navigate_position(int8_t delta) {
    menu_context_t *ctx = menu_get_context();
    menu_navigate_handle_position(ctx, ctx->current, delta);
}
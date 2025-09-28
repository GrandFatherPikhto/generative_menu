#include "menu_handle.h"
#include "menu_context.h"
#include "menu_navigate.h"
#include "menu_draw.h"

// Инициализация
void menu_init(void) {
    static menu_context_t context = { 0 };
    menu_context_init(&context);
}

// Публичное API для внешнего мира
void menu_handle_position(int8_t delta) {
    menu_context_t *ctx = menu_context_get();
    menu_navigate_handle_position(ctx, ctx->current, delta);
}

void menu_handle_enter(void) {
    menu_context_t *ctx = menu_context_get();
    menu_navigate_handle_enter(ctx, ctx->current);
}

void menu_handle_back(void) {
    menu_context_t *ctx = menu_context_get();
    menu_navigate_handle_back(ctx, ctx->current);
}

void menu_handle_reset(void) {
    // Пока не реализовано. Сброс к начальным значениям
    menu_context_t *ctx = menu_context_get();
}

void menu_update(void) {
    menu_context_t *ctx = menu_context_get();
    if (ctx->dirty) {
        menu_draw_update(ctx, ctx->current);
    }
}

// Для отрисовки (можно вынести в отдельный menu_display.h)
char* menu_get_display_title(void) {
    menu_context_t *ctx = menu_context_get();
    return ctx->title_buf;
}

char* menu_get_display_value(void) {
    menu_context_t *ctx = menu_context_get();
    return ctx->value_buf;
}

bool menu_needs_redraw(void) {
    menu_context_t *ctx = menu_context_get();
    return ctx->invalidate;
}

bool menu_ack_redraw(void) {
    menu_context_t *ctx = menu_context_get();
    bool ack_redraw = ctx->invalidate;
    ctx->invalidate = false;
    return ack_redraw;
}

void menu_set_dirty(void) {
    menu_context_t *ctx = menu_context_get();
    ctx->dirty = true;
}

menu_state_t menu_get_state(void) {
    menu_context_t *ctx = menu_context_get();
    return ctx->state;
}
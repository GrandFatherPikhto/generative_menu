#include "lcd1602_sdl.h"
#include "utils.h"
#include <unistd.h>
#include <signal.h>
#include <execinfo.h>

#include "menu_navigate.h"
#include "menu_engine.h"
#include "menu_context.h"

typedef struct s_rotary_encoder_t {
    int position;
    int back;
    int8_t delta;
} rotary_encoder_t;

static rotary_encoder_t s_rotary_encoder = {
    .position = 0,
    .back = 0,
    .delta = 0
};

bool rotenc_position(int new_pos) {
    if (new_pos % 2) {
        printf("Debounce factor is wrang\n");
        return false;
    }

    s_rotary_encoder.back = s_rotary_encoder.position;
    s_rotary_encoder.position = new_pos / 2;
    s_rotary_encoder.delta = s_rotary_encoder.position - s_rotary_encoder.back;
    return true;
}

// Callback-функции
void on_position_change(int new_pos) {
    if (!rotenc_position(new_pos))
        return;

    printf("Position changed: %d/%d\n", s_rotary_encoder.delta, s_rotary_encoder.position);
    // Здесь обновляем меню или что-то еще
    menu_navigate_position(s_rotary_encoder.delta);
}

void on_push_button() {
    printf("Button pushed\n");
    // Обработка короткого нажатия
    menu_navigate_enter();
}

void on_long_push_button() {
    printf("Long button push\n");
    // Обработка длинного нажатия
    menu_navigate_out();
}

void on_double_click() {
    printf("Double click\n");
    // Обработка двойного нажатия
}

bool menu_is_dirty(void) {
    return true;
}

void menu_draw(sdl_handle_t *lcd) {
    printf("%s\r\n", menu_context_get_title_str());
    printf("%s\r\n", menu_context_get_value_str());
    lcd_sdl_clear(lcd);
    lcd_sdl_set_cursor(lcd, 0, 0);
    lcd_sdl_print_str(lcd, menu_context_get_title_str());
    lcd_sdl_set_cursor(lcd, 0, 1);
    lcd_sdl_print_str(lcd, menu_context_get_value_str());
    menu_context_reset_invalidate();
}

int main() {
    signal(SIGSEGV, signal_handler);
    signal(SIGABRT, signal_handler);

    menu_context_init();

    // Создаем экземпляр симулятора
    sdl_handle_t *lcd = lcd1602_sdl_create("LCD1602 Simulator", 360, 150);
    if (!lcd) {
        fprintf(stderr, "Failed to create LCD simulator\n");
        return 1;
    }

    // Устанавливаем callback-и
    lcd1602_sdl_set_position_cb(lcd, on_position_change);
    lcd1602_sdl_set_push_button_cb(lcd, on_push_button);
    lcd1602_sdl_set_long_push_button_cb(lcd, on_long_push_button);
    lcd1602_sdl_set_double_click_cb(lcd, on_double_click);

    // Устанавливаем дебаунс (опционально)
    lcd1602_sdl_set_debounce(lcd, 2);

    // Главный цикл
    while (lcd1602_sdl_next_tick(lcd)) {
        // Здесь может быть ваша логика, например, обновление меню
        // Но важно не блокировать надолго, чтобы симулятор оставался отзывчивым
        menu_navigate_update();
        // Например, проверяем, нужно ли перерисовать меню
        if (menu_context_is_invalidate()) {
            menu_draw(lcd);
        }

        // Небольшая пауза, чтобы не грузить CPU
        usleep(10000); // 10 мс
    }

    // Освобождаем ресурсы
    lcd1602_sdl_release(lcd);

    return 0;
}
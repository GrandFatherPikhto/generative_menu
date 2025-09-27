#include "lcd1602.h"
#include "lcd1602_sdl.h"
#include "rotary_encoder.h"

#define LCD_BUFFER_SIZE 0x20

static sdl_handle_t *s_sdl_handle = 0;

void lcd1602_init(void)
{
    renc_handle_t *renc_handle;
    renc_init(renc_handle);

    s_sdl_handle = lcd1602_sdl_create("LCD1602", 0, 0);
    lcd1602_sdl_set_position_cb(s_sdl_handle, renc_change_position);
    lcd1602_sdl_set_push_button_cb(s_sdl_handle, renc_push_button);
    lcd1602_sdl_set_long_push_button_cb(s_sdl_handle, renc_long_push_button);
    lcd1602_sdl_set_double_click_cb(s_sdl_handle, renc_double_click_button);    
}

void lcd1602_release(void) {
    if (s_sdl_handle != 0)
        return;
    
    lcd1602_sdl_release(s_sdl_handle);
}

bool lcd1602_tick(void) {
    if (s_sdl_handle == 0)
        return false;
    return lcd1602_sdl_next_tick(s_sdl_handle);
}

void lcd1602_printf(const char *format, ...) {
    char buffer[LCD_BUFFER_SIZE];
    va_list args;
    
    va_start(args, format);
    vsnprintf(buffer, LCD_BUFFER_SIZE, format, args);
    va_end(args);
    
    // printf("%s\n", buffer);
    lcd_sdl_print_str(s_sdl_handle, buffer);
}

void lcd1602_clear()
{
    lcd_sdl_clear(s_sdl_handle);
}

void lcd1602_set_cursor(int x, int y)
{
    // printf("%s:%d x: %d, y: %d\n", __FILE__, __LINE__, x, y);
    lcd_sdl_set_cursor(s_sdl_handle, x, y);
}

void lcd1602_print(const char *str)
{
    lcd_sdl_print_str(s_sdl_handle, str);
    // printf("%s:%d menu: %s\n", __FILE__, __LINE__, str);
}

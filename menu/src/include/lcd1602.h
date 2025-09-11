#ifndef LCD_CONTEXT_H
#define LCD_CONTEXT_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "lcd1602_sdl.h"

void lcd1602_init(void);
void lcd1602_release(void);
bool lcd1602_tick(void);
void lcd1602_clear(void);
void lcd1602_set_cursor(int, int);
void lcd1602_print(const char *);
void lcd1602_printf(const char *format, ...);

#endif // LCD_CONTEXT_H
// menu.h
#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <stdbool.h>

#include "menu_types.h"

// Инициализация
void menu_init(void);

// Публичное API для внешнего мира
void menu_handle_position(int8_t delta);
void menu_handle_enter(void);
void menu_handle_back(void);
void menu_handle_reset(void);
void menu_update(void);

// Для отрисовки (можно вынести в отдельный menu_display.h)
char* menu_get_display_title(void);
char* menu_get_display_value(void);
bool menu_needs_redraw(void);
bool menu_ack_redraw(void);
void menu_set_dirty(void);
menu_state_t menu_get_state(void);

#endif
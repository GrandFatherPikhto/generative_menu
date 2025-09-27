#ifndef MENU_NAVIGATE_H
#define MENU_NAVIGATE_H

#include <stdint.h>
#include <stdbool.h>

#include "menu_common.h"

void menu_navigate_update(void);
void menu_navigate_enter(void);
void menu_navigate_out(void);
void menu_navigate_position(int8_t delta);

#endif /* MENU_NAVIGATE_H */

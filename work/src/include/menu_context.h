#ifndef MENU_CONTEXT_H
#define MENU_CONTEXT_H

#include <stdint.h>
#include <stdbool.h>

#include "menu_common.h"

typedef enum {
    MENU_STATE_NONE = 0,
    MENU_STATE_NAVIGATION = 1,
    MENU_STATE_EDIT = 2,
    MENU_STATE_COUNT = 3
} menu_state_t;

typedef enum {
    MENU_EVENT_NONE = 0,
    MENU_EVENT_CHANGE = 1,
    MENU_EVENT_CHANGE_VALUE = 2,
    MENU_EVENT_FOCUSED = 3,
    MENU_EVENT_UNFOCUSED = 4,
    MENU_EVENT_START_EDIT = 5,
    MENU_EVENT_STOP_EDIT = 6,
    MENU_EVENT_COUNT = 7
} menu_event_t;

typedef struct s_menu_context_t {
    menu_id_t current;
    menu_id_t previous;
    menu_state_t state;
    menu_event_t event; ///< Планируется для оформления функции-коллбэка чтобы оповещать внешний мир о событиях в меню
    bool dirty;
    bool invalidate;

    menu_node_value_t *values; ///< Неконстантный массив в динамической памяти. Изменяемые значения листьев
    const menu_node_config_t *configs; ///< Константный массив свойств (конфигураций) листьев
    const menu_node_t *nodes; ///< Константный массив всех нод -- дерево меню

    // Буферы для отрисовки (можно вынести в draw модуль)
    char title_buf[LCD_STRING_LEN];
    char value_buf[LCD_STRING_LEN];    
} menu_context_t;

menu_context_t *menu_get_context(void);
void menu_context_set_dirty(bool dirty);
void menu_context_reset_invalidate(void);
bool menu_context_is_invalidate(void);
void menu_context_set_node_tree(const menu_node_t *tree);
void menu_context_set_node_configs(const menu_node_config_t *configs);
void menu_context_set_node_values(menu_node_value_t *values);
char *menu_context_get_title_str(void);
char *menu_context_get_value_str(void);

#endif /* MENU_CONTEXT_H */

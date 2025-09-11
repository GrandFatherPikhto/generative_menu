#include "rotenc_menu.h"
#include <string.h>

// Макрос для ограничения значений
#define CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

// Статические массивы для факторов
static const int32_t factors_sound[] = { 1, 10, 100 };

// НЕИЗМЕНЯЕМЫЕ данные конфигурации меню
static const menu_item_t s_menu_config[MENU_ID_COUNT] = {
    [MENU_ID_ROOT] = {
        .id = MENU_ID_ROOT,
        .title = "ROOT",
        .type = MENU_TYPE_ROOT,
        .parent = MENU_ID_COUNT,
        .first_child = MENU_ID_SETTINGS,
        .next_sibling = MENU_ID_COUNT,
        .prev_sibling = MENU_ID_COUNT,
        .config = {0}
    },
    [MENU_ID_SETTINGS] = {
        .id = MENU_ID_SETTINGS,
        .title = "Settings",
        .type = MENU_TYPE_MENU,
        .parent = MENU_ID_ROOT,
        .first_child = MENU_ID_DISPLAY_OPTION,
        .next_sibling = MENU_ID_COUNT,
        .prev_sibling = MENU_ID_COUNT,
        .config = {
            // Пустая структура для других типов
            .action_int = {0}
        }
    },    [MENU_ID_DISPLAY_OPTION] = {
        .id = MENU_ID_DISPLAY_OPTION,
        .title = "Display",
        .type = MENU_TYPE_ACTION_INT,
        .parent = MENU_ID_SETTINGS,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_COUNT,
        .prev_sibling = MENU_ID_COUNT,
        .config = {
            .action_int = {
                .min = 0,
                .max = 100,
                .default_value = 50,
                .step = 1
            }
        }
    },    [MENU_ID_SOUND] = {
        .id = MENU_ID_SOUND,
        .title = "Sound",
        .type = MENU_TYPE_ACTION_INT_FACTOR,
        .parent = MENU_ID_SETTINGS,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_COUNT,
        .prev_sibling = MENU_ID_DISPLAY_OPTION,
        .config = {
            .action_int_factor = {
                .min = 0,
                .max = 10,
                .default_value = 5,
                .factors = factors_sound,
                .factors_count = sizeof(factors_sound) / sizeof(factors_sound[0]),
                .default_factor_idx = 0
            }
        }
    },    [MENU_ID_INFO] = {
        .id = MENU_ID_INFO,
        .title = "Info",
        .type = MENU_TYPE_MENU,
        .parent = MENU_ID_ROOT,
        .first_child = MENU_ID_VERSION,
        .next_sibling = MENU_ID_COUNT,
        .prev_sibling = MENU_ID_SETTINGS,
        .config = {
            // Пустая структура для других типов
            .action_int = {0}
        }
    },    [MENU_ID_VERSION] = {
        .id = MENU_ID_VERSION,
        .title = "Version",
        .type = MENU_TYPE_ACTION_CALLBACK,
        .parent = MENU_ID_INFO,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_COUNT,
        .prev_sibling = MENU_ID_COUNT,
        .config = {
            .action_callback = {
                .callback = NULL
            }
        }
    },    [MENU_ID_START] = {
        .id = MENU_ID_START,
        .title = "Start",
        .type = MENU_TYPE_ACTION_BOOL,
        .parent = MENU_ID_ROOT,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_COUNT,
        .prev_sibling = MENU_ID_INFO,
        .config = {
            .action_bool = {
                .default_value = false
            }
        }
    }};

// ИЗМЕНЯЕМЫЕ данные значений меню
static menu_values_t s_menu_values = {
    .values = {
        [MENU_ID_SETTINGS] = {
            .action_int = {0}
        },        [MENU_ID_DISPLAY_OPTION] = {
            .action_int = {
                .value = 50
            }
        },        [MENU_ID_SOUND] = {
            .action_int_factor = {
                .value = 5,
                .factor_idx = 0
            }
        },        [MENU_ID_INFO] = {
            .action_int = {0}
        },        [MENU_ID_VERSION] = {
            .action_int = {0}
        },        [MENU_ID_START] = {
            .action_bool = {
                .value = false
            }
        }    },
    .current_menu = MENU_ID_SETTINGS,
    .state = MENU_STATE_NAVIGATION
};

// Реализация функций
const menu_item_t *menu_get_config(menu_id_t id) {
    if (id >= MENU_ID_COUNT) return NULL;
    return &s_menu_config[id];
}

menu_values_t *menu_get_values(void) {
    return &s_menu_values;
}

void *menu_get_value_ptr(menu_id_t id) {
    if (id >= MENU_ID_COUNT) return NULL;
    
    const menu_item_t *config = &s_menu_config[id];
    menu_item_values_t *values = &s_menu_values.values[id];
    
    switch (config->type) {
        case MENU_TYPE_ACTION_INT:
            return &values->action_int.value;
        case MENU_TYPE_ACTION_INT_FACTOR:
            return &values->action_int_factor.value;
        case MENU_TYPE_ACTION_BOOL:
            return &values->action_bool.value;
        default:
            return NULL;
    }
}

void handle_change_position(int8_t delta) {
    const menu_item_t *current_config = &s_menu_config[s_menu_values.current_menu];
    menu_item_values_t *current_values = &s_menu_values.values[s_menu_values.current_menu];
    
    if (s_menu_values.state == MENU_STATE_NAVIGATION) {
        // Навигация между элементами
        menu_id_t target = delta > 0 ? current_config->next_sibling : current_config->prev_sibling;
        if (target != MENU_ID_COUNT) {
            s_menu_values.current_menu = target;
        } else {
            // Зацикливание навигации
            if (delta > 0) {
                // Ищем первого sibling
                menu_id_t first_sibling = current_config->parent != MENU_ID_COUNT ? 
                    s_menu_config[current_config->parent].first_child : MENU_ID_COUNT;
                if (first_sibling != MENU_ID_COUNT) {
                    s_menu_values.current_menu = first_sibling;
                }
            } else {
                // Ищем последнего sibling
                menu_id_t last_sibling = current_config->parent != MENU_ID_COUNT ? 
                    s_menu_config[current_config->parent].first_child : MENU_ID_COUNT;
                while (last_sibling != MENU_ID_COUNT && 
                       s_menu_config[last_sibling].next_sibling != MENU_ID_COUNT) {
                    last_sibling = s_menu_config[last_sibling].next_sibling;
                }
                if (last_sibling != MENU_ID_COUNT) {
                    s_menu_values.current_menu = last_sibling;
                }
            }
        }
    } else if (s_menu_values.state == MENU_STATE_EDIT) {
        // Редактирование значения
        switch (current_config->type) {
            case MENU_TYPE_ACTION_INT:
                current_values->action_int.value += delta * current_config->config.action_int.step;
                current_values->action_int.value = CLAMP(current_values->action_int.value,
                    current_config->config.action_int.min, current_config->config.action_int.max);
                break;
                
            case MENU_TYPE_ACTION_INT_FACTOR:
                current_values->action_int_factor.value += delta *
                    current_config->config.action_int_factor.factors[current_values->action_int_factor.factor_idx];
                current_values->action_int_factor.value = CLAMP(current_values->action_int_factor.value,
                    current_config->config.action_int_factor.min, current_config->config.action_int_factor.max);
                break;
                
            // ... другие типы
        }
    }
}

void handle_push_button(void) {
    const menu_item_t *current_config = &s_menu_config[s_menu_values.current_menu];
    menu_item_values_t *current_values = &s_menu_values.values[s_menu_values.current_menu];
    
    if (s_menu_values.state == MENU_STATE_NAVIGATION) {
        if (current_config->first_child != MENU_ID_COUNT) {
            // Переход к ребенку
            s_menu_values.current_menu = current_config->first_child;
        } else if (current_config->type == MENU_TYPE_ACTION_BOOL ||
                   current_config->type == MENU_TYPE_ACTION_INT_FACTOR) {
            // Переход в режим редактирования
            s_menu_values.state = MENU_STATE_EDIT;
        }
    } else if (s_menu_values.state == MENU_STATE_EDIT) {
        // Обработка в режиме редактирования
        switch (current_config->type) {
            case MENU_TYPE_ACTION_BOOL:
                current_values->action_bool.value = !current_values->action_bool.value;
                break;
                
            case MENU_TYPE_ACTION_INT_FACTOR:
                current_values->action_int_factor.factor_idx =
                    (current_values->action_int_factor.factor_idx + 1) %
                    current_config->config.action_int_factor.factors_count;
                break;
        }
    }
}

void handle_long_push_button(void) {
    if (s_menu_values.state == MENU_STATE_EDIT) {
        // Выход из режима редактирования
        s_menu_values.state = MENU_STATE_NAVIGATION;
    } else {
        // Возврат к родителю
        const menu_item_t *current_config = &s_menu_config[s_menu_values.current_menu];
        if (current_config->parent != MENU_ID_COUNT && current_config->parent != MENU_ID_ROOT) {
            s_menu_values.current_menu = current_config->parent;
        }
    }
}

void handle_double_click_button(void) {
    if (s_menu_values.state == MENU_STATE_EDIT) {
        // Сброс к значениям по умолчанию
        const menu_item_t *current_config = &s_menu_config[s_menu_values.current_menu];
        menu_item_values_t *current_values = &s_menu_values.values[s_menu_values.current_menu];
        
        switch (current_config->type) {
            case MENU_TYPE_ACTION_INT:
                current_values->action_int.value = current_config->config.action_int.default_value;
                break;
            case MENU_TYPE_ACTION_INT_FACTOR:
                current_values->action_int_factor.value = current_config->config.action_int_factor.default_value;
                current_values->action_int_factor.factor_idx = current_config->config.action_int_factor.default_factor_idx;
                break;
            case MENU_TYPE_ACTION_BOOL:
                current_values->action_bool.value = current_config->config.action_bool.default_value;
                break;
        }
    } else {
        // Возврат к первому элементу root
        s_menu_values.current_menu = s_menu_config[MENU_ID_ROOT].first_child;
    }
}
#include "rotenc_menu.h"
#include <string.h>

#include "lcd1602.h"
#include "rotenc_callback.h"

// Макрос для ограничения значений
#ifndef CLAMP
#define CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))
#endif

#ifndef SET_BIT
#define SET_BIT(a, pos) ((a) |= (1 << pos))
#endif

#ifndef RESET_BIT
#define RESET_BIT(a, pos)((a) &= ~(1 << pos))
#endif

#ifndef TOGGLE_BIT
#define TOGGLE_BIT(a, pos)((a) ^= (1 << pos))
#endif

#ifndef IS_BIT_SET
#define IS_BIT_SET(a, pos) (((a) & (1 << pos)) == (1 << pos))
#endif 

#ifndef IS_BIT_RESET
#define IS_BIT_RESET(a, pos) (((a) & (1 << pos)) == 0)
#endif

static menu_event_cb_t s_menu_event_cb = collback_menu_event;
static menu_display_str_cb_t s_display_str_cb = collback_display_str;

// Статические массивы для факторов
static const int32_t factors_hi_delay[] = { 1, 10, 100, 1000 };
static const int32_t factors_hi_duration[] = { 1, 10, 100, 1000 };
static const int32_t factors_lo_delay[] = { 1, 10, 100, 1000 };
static const int32_t factors_lo_duration[] = { 1, 10, 100, 1000 };
static const int32_t fixed_int_delimiters[] = { 12, 3, 5, 7, 10 };
static const char *fixed_string_regime[] = { "Normal", "Precision", "Super" };
static const uint8_t fixed_string_ids_regime[] = { 1, 2, 3 };

// НЕИЗМЕНЯЕМЫЕ данные конфигурации меню
static const menu_item_t s_menu_config[MENU_ID_COUNT] = {
    [MENU_ID_ROOT] = {
        .id = MENU_ID_ROOT,
        .title = "ROOT",
        .type = MENU_TYPE_ROOT,
        .parent = MENU_ID_COUNT,
        .first_child = MENU_ID_INFO,
        .next_sibling = MENU_ID_COUNT,
        .prev_sibling = MENU_ID_COUNT,
        .first_sibling = MENU_ID_COUNT,
        .last_sibling = MENU_ID_COUNT,
        .config = {0}
    },
    [MENU_ID_INFO] = {
        .id = MENU_ID_INFO,
        .title = "Info",
        .type = MENU_TYPE_ACTION_MENU,
        .parent = MENU_ID_ROOT,
        .first_child = MENU_ID_VERSION,
        .next_sibling = MENU_ID_START,
        .prev_sibling = MENU_ID_COUNT,
        .first_sibling = MENU_ID_INFO,
        .last_sibling = MENU_ID_SETTINGS,
        .config = {
            // action_menu
            // Пустая структура для других типов
            .action_int = {0}
        }
    },
    [MENU_ID_VERSION] = {
        .id = MENU_ID_VERSION,
        .title = "Version",
        .type = MENU_TYPE_ACTION_CALLBACK,
        .parent = MENU_ID_INFO,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_COUNT,
        .prev_sibling = MENU_ID_COUNT,
        .first_sibling = MENU_ID_VERSION,
        .last_sibling = MENU_ID_VERSION,
        .config = {
            // action_callback
            // Пустая структура для других типов
            .action_int = {0}
        }
    },
    [MENU_ID_START] = {
        .id = MENU_ID_START,
        .title = "Start",
        .type = MENU_TYPE_ACTION_BOOL,
        .parent = MENU_ID_ROOT,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_REGIME,
        .prev_sibling = MENU_ID_INFO,
        .first_sibling = MENU_ID_INFO,
        .last_sibling = MENU_ID_SETTINGS,
        .config = {
            // action_bool
            .action_bool = {
                .default_value = false
            }
        }
    },
    [MENU_ID_REGIME] = {
        .id = MENU_ID_REGIME,
        .title = "Regime",
        .type = MENU_TYPE_ACTION_FIXED_STRING,
        .parent = MENU_ID_ROOT,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_DELIMITERS,
        .prev_sibling = MENU_ID_START,
        .first_sibling = MENU_ID_INFO,
        .last_sibling = MENU_ID_SETTINGS,
        .config = {
            // action_fixed_string
            .action_fixed_string = {
                .ids = fixed_string_ids_regime,
                .massif = fixed_string_regime,
                .counter = 3,
                .default_idx = 0
            }
        }
    },
    [MENU_ID_DELIMITERS] = {
        .id = MENU_ID_DELIMITERS,
        .title = "Delimiters",
        .type = MENU_TYPE_ACTION_FIXED_INT,
        .parent = MENU_ID_ROOT,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_SETTINGS,
        .prev_sibling = MENU_ID_REGIME,
        .first_sibling = MENU_ID_INFO,
        .last_sibling = MENU_ID_SETTINGS,
        .config = {
            // action_fixed_int
            .action_fixed_int = {
                .massif = fixed_int_delimiters,
                .counter = 5,
                .default_idx = 2
            }
        }
    },
    [MENU_ID_SETTINGS] = {
        .id = MENU_ID_SETTINGS,
        .title = "Settings",
        .type = MENU_TYPE_ACTION_MENU,
        .parent = MENU_ID_ROOT,
        .first_child = MENU_ID_PWM_FREQUENCY,
        .next_sibling = MENU_ID_COUNT,
        .prev_sibling = MENU_ID_DELIMITERS,
        .first_sibling = MENU_ID_INFO,
        .last_sibling = MENU_ID_SETTINGS,
        .config = {
            // action_menu
            // Пустая структура для других типов
            .action_int = {0}
        }
    },
    [MENU_ID_PWM_FREQUENCY] = {
        .id = MENU_ID_PWM_FREQUENCY,
        .title = "PWM Frequency",
        .type = MENU_TYPE_ACTION_CALLBACK,
        .parent = MENU_ID_SETTINGS,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_HI_CHANNEL,
        .prev_sibling = MENU_ID_COUNT,
        .first_sibling = MENU_ID_PWM_FREQUENCY,
        .last_sibling = MENU_ID_LO_CHANNEL,
        .config = {
            // action_callback
            // Пустая структура для других типов
            .action_int = {0}
        }
    },
    [MENU_ID_HI_CHANNEL] = {
        .id = MENU_ID_HI_CHANNEL,
        .title = "Hi Channel",
        .type = MENU_TYPE_ACTION_MENU,
        .parent = MENU_ID_SETTINGS,
        .first_child = MENU_ID_HI_DELAY,
        .next_sibling = MENU_ID_LO_CHANNEL,
        .prev_sibling = MENU_ID_PWM_FREQUENCY,
        .first_sibling = MENU_ID_PWM_FREQUENCY,
        .last_sibling = MENU_ID_LO_CHANNEL,
        .config = {
            // action_menu
            // Пустая структура для других типов
            .action_int = {0}
        }
    },
    [MENU_ID_LO_CHANNEL] = {
        .id = MENU_ID_LO_CHANNEL,
        .title = "Lo Channel",
        .type = MENU_TYPE_ACTION_MENU,
        .parent = MENU_ID_SETTINGS,
        .first_child = MENU_ID_LO_DELAY,
        .next_sibling = MENU_ID_COUNT,
        .prev_sibling = MENU_ID_HI_CHANNEL,
        .first_sibling = MENU_ID_PWM_FREQUENCY,
        .last_sibling = MENU_ID_LO_CHANNEL,
        .config = {
            // action_menu
            // Пустая структура для других типов
            .action_int = {0}
        }
    },
    [MENU_ID_HI_DELAY] = {
        .id = MENU_ID_HI_DELAY,
        .title = "Delay",
        .type = MENU_TYPE_ACTION_INT_FACTOR,
        .parent = MENU_ID_HI_CHANNEL,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_HI_DURATION,
        .prev_sibling = MENU_ID_COUNT,
        .first_sibling = MENU_ID_HI_DELAY,
        .last_sibling = MENU_ID_HI_DUTY,
        .config = {
            // action_int_factor
            .action_int_factor = {
                .min = 0,
                .max = 100000,
                .default_value = 100,
                .factor = {
                    .default_idx = 2,
                    .factors = factors_hi_delay,
                    .count = 4
                }
            }
        }
    },
    [MENU_ID_HI_DURATION] = {
        .id = MENU_ID_HI_DURATION,
        .title = "Duration",
        .type = MENU_TYPE_ACTION_INT_FACTOR,
        .parent = MENU_ID_HI_CHANNEL,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_HI_PWM_ON,
        .prev_sibling = MENU_ID_HI_DELAY,
        .first_sibling = MENU_ID_HI_DELAY,
        .last_sibling = MENU_ID_HI_DUTY,
        .config = {
            // action_int_factor
            .action_int_factor = {
                .min = 0,
                .max = 100000,
                .default_value = 100,
                .factor = {
                    .default_idx = 2,
                    .factors = factors_hi_duration,
                    .count = 4
                }
            }
        }
    },
    [MENU_ID_HI_PWM_ON] = {
        .id = MENU_ID_HI_PWM_ON,
        .title = "PWM On/Off",
        .type = MENU_TYPE_ACTION_BOOL,
        .parent = MENU_ID_HI_CHANNEL,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_HI_DUTY,
        .prev_sibling = MENU_ID_HI_DURATION,
        .first_sibling = MENU_ID_HI_DELAY,
        .last_sibling = MENU_ID_HI_DUTY,
        .config = {
            // action_bool
            .action_bool = {
                .default_value = false
            }
        }
    },
    [MENU_ID_HI_DUTY] = {
        .id = MENU_ID_HI_DUTY,
        .title = "PWM Duty",
        .type = MENU_TYPE_ACTION_INT,
        .parent = MENU_ID_HI_CHANNEL,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_COUNT,
        .prev_sibling = MENU_ID_HI_PWM_ON,
        .first_sibling = MENU_ID_HI_DELAY,
        .last_sibling = MENU_ID_HI_DUTY,
        .config = {
            // action_int
            .action_int = {
                .min = 0,
                .max = 100,
                .default_value = 45,
                .step = 1
            }
        }
    },
    [MENU_ID_LO_DELAY] = {
        .id = MENU_ID_LO_DELAY,
        .title = "Delay",
        .type = MENU_TYPE_ACTION_INT_FACTOR,
        .parent = MENU_ID_LO_CHANNEL,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_LO_DURATION,
        .prev_sibling = MENU_ID_COUNT,
        .first_sibling = MENU_ID_LO_DELAY,
        .last_sibling = MENU_ID_LO_DUTY,
        .config = {
            // action_int_factor
            .action_int_factor = {
                .min = 0,
                .max = 100000,
                .default_value = 100,
                .factor = {
                    .default_idx = 2,
                    .factors = factors_lo_delay,
                    .count = 4
                }
            }
        }
    },
    [MENU_ID_LO_DURATION] = {
        .id = MENU_ID_LO_DURATION,
        .title = "Duration",
        .type = MENU_TYPE_ACTION_INT_FACTOR,
        .parent = MENU_ID_LO_CHANNEL,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_LO_PWM_ON,
        .prev_sibling = MENU_ID_LO_DELAY,
        .first_sibling = MENU_ID_LO_DELAY,
        .last_sibling = MENU_ID_LO_DUTY,
        .config = {
            // action_int_factor
            .action_int_factor = {
                .min = 0,
                .max = 100000,
                .default_value = 100,
                .factor = {
                    .default_idx = 2,
                    .factors = factors_lo_duration,
                    .count = 4
                }
            }
        }
    },
    [MENU_ID_LO_PWM_ON] = {
        .id = MENU_ID_LO_PWM_ON,
        .title = "PWM On/Off",
        .type = MENU_TYPE_ACTION_BOOL,
        .parent = MENU_ID_LO_CHANNEL,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_LO_DUTY,
        .prev_sibling = MENU_ID_LO_DURATION,
        .first_sibling = MENU_ID_LO_DELAY,
        .last_sibling = MENU_ID_LO_DUTY,
        .config = {
            // action_bool
            .action_bool = {
                .default_value = false
            }
        }
    },
    [MENU_ID_LO_DUTY] = {
        .id = MENU_ID_LO_DUTY,
        .title = "PWM Duty",
        .type = MENU_TYPE_ACTION_INT,
        .parent = MENU_ID_LO_CHANNEL,
        .first_child = MENU_ID_COUNT,
        .next_sibling = MENU_ID_COUNT,
        .prev_sibling = MENU_ID_LO_PWM_ON,
        .first_sibling = MENU_ID_LO_DELAY,
        .last_sibling = MENU_ID_LO_DUTY,
        .config = {
            // action_int
            .action_int = {
                .min = 0,
                .max = 100,
                .default_value = 45,
                .step = 1
            }
        }
    }
};

// ИЗМЕНЯЕМЫЕ данные значений меню
static menu_values_t s_menu_values = {
    .values = {
        [MENU_ID_INFO] = {
            .action_int = {0}
        },
        [MENU_ID_VERSION] = {
            .action_int = {0}
        },
        [MENU_ID_START] = {
            .action_bool = {
                .value = false
            }
        },
        [MENU_ID_REGIME] = {
            .action_fixed_string = {
                .value_idx = 0
            }
        },
        [MENU_ID_DELIMITERS] = {
            .action_fixed_int = {
                .value_idx = 2
            }
        },
        [MENU_ID_SETTINGS] = {
            .action_int = {0}
        },
        [MENU_ID_PWM_FREQUENCY] = {
            .action_int = {0}
        },
        [MENU_ID_HI_CHANNEL] = {
            .action_int = {0}
        },
        [MENU_ID_LO_CHANNEL] = {
            .action_int = {0}
        },
        [MENU_ID_HI_DELAY] = {
            .action_int_factor = {
                .value = 100,
                .factor_idx = 2
            }
        },
        [MENU_ID_HI_DURATION] = {
            .action_int_factor = {
                .value = 100,
                .factor_idx = 2
            }
        },
        [MENU_ID_HI_PWM_ON] = {
            .action_bool = {
                .value = false
            }
        },
        [MENU_ID_HI_DUTY] = {
            .action_int = {
                .value = 45
            }
        },
        [MENU_ID_LO_DELAY] = {
            .action_int_factor = {
                .value = 100,
                .factor_idx = 2
            }
        },
        [MENU_ID_LO_DURATION] = {
            .action_int_factor = {
                .value = 100,
                .factor_idx = 2
            }
        },
        [MENU_ID_LO_PWM_ON] = {
            .action_bool = {
                .value = false
            }
        },
        [MENU_ID_LO_DUTY] = {
            .action_int = {
                .value = 45
            }
        }
    },
    .state_info = {
        .current = MENU_ID_INFO,
        .previous = MENU_ID_SETTINGS,
        .state = MENU_STATE_NAVIGATION,
        .delta = 0,
        .menu_event = 0,
        .control_event = 0,
        .dirty = false
    }
};

// Статические функции
static void s_navigate_sibling(void);
static void s_set_edit_state(void);
static void s_change_value(void);
static void s_next_value(void);
static void s_set_menu(menu_id_t menu_id);
static void s_menu_set_state(menu_state_t state);
static void s_call_menu_event_cb(void);
static void s_reset_state_info(void);
static void s_exec_callback_action(void);
static int32_t s_get_current_factor(void);
static void s_next_factor_idx (void);

static void s_toggle_bool_value(void);
static void s_set_default_bool_value (void);
static void s_next_int_value(void);
static void s_change_int_value(void);
static void s_set_default_int_value (void);
static void s_change_int_factor_value(void);
static void s_set_default_int_factor_value (void);
static void s_next_fixed_int_value(void);
static void s_change_fixed_int_value(void);
static void s_set_default_fixed_int_value(void);
static void s_next_fixed_string_value(void);
static void s_change_fixed_string_value(void);
static void s_set_default_fixed_string_value(void);

// Реализация функций
const menu_item_t *menu_get_config(menu_id_t id) {
    if (id >= MENU_ID_COUNT) return NULL;
    return &s_menu_config[id];
}

const menu_item_t *menu_get_current_config(void) {
    return menu_get_config(s_menu_values.state_info.current);
}

menu_item_values_t *menu_get_values(menu_id_t id) {
    if (id >= MENU_ID_COUNT) return NULL;
    return &(s_menu_values.values[id]);
}

menu_item_values_t *menu_get_current_values(void) {
    return menu_get_values(s_menu_values.state_info.current);
}

void *menu_get_value_ptr(menu_id_t id) {
    if (id >= MENU_ID_COUNT) return NULL;
    
    const menu_item_t *config = &s_menu_config[id];
    menu_item_values_t *values = &s_menu_values.values[id];
    if (values == 0)
        return NULL;
    
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
    const menu_item_t *config = menu_get_current_config();
    menu_item_values_t *values = menu_get_current_values();

    s_reset_state_info();
    s_menu_values.state_info.control_event |= MENU_EVENT_DELTA_CHANGED;
    s_menu_values.state_info.delta = delta;

    switch(s_menu_values.state_info.state) {
        case MENU_STATE_NAVIGATION: {
            s_navigate_sibling();
            } break;
        case MENU_STATE_EDIT: {
            s_change_value();
        } break;
        default:
            break;
    }

    s_exec_callback_action();
}

void handle_push_button(void) {
    s_reset_state_info();
    const menu_item_t *config = menu_get_current_config();
    menu_item_values_t *values = menu_get_current_values();
    s_menu_values.state_info.control_event |= MENU_EVENT_BUTTON_PRESS;

    switch(s_menu_values.state_info.state) {
        case MENU_STATE_NAVIGATION: {
            if (config->first_child != MENU_ID_COUNT) {
                s_set_menu(config->first_child);
            } else {
                s_set_edit_state();
            }
        } break;
        case MENU_STATE_EDIT: {
            s_next_value ();
        } break;
        default: 
            break;
    }

    s_exec_callback_action();
}

void handle_long_push_button(void) {
    s_reset_state_info();
    const menu_item_t *config = menu_get_current_config();
    menu_item_values_t *values = menu_get_current_values();

    s_menu_values.state_info.control_event |= MENU_EVENT_BUTTON_LONG_PRESS;

    if (s_menu_values.state_info.state == MENU_STATE_EDIT) {
        // Выход из режима редактирования
        s_menu_values.state_info.menu_event |= MENU_EVENT_EXIT_EDIT;
        s_menu_set_state(MENU_STATE_NAVIGATION);
    } else {
        // Возврат к родителю
        if (config->parent != MENU_ID_COUNT && config->parent != MENU_ID_ROOT) {
            s_set_menu(config->parent);
        }
    }

    s_exec_callback_action();
}

void handle_double_click_button(void) {
    s_reset_state_info();
    s_menu_values.state_info.control_event |= MENU_EVENT_BUTTON_DOUBLE_CLICK;
    const menu_item_t *config = menu_get_current_config();
    menu_item_values_t *values = menu_get_current_values();

    if (s_menu_values.state_info.state == MENU_STATE_EDIT) {
        // Сброс к значениям по умолчанию
        switch (config->type) {
            
            case MENU_TYPE_ACTION_BOOL:
                s_set_default_bool_value ();
                break;
                
            case MENU_TYPE_ACTION_INT:
                s_set_default_int_value ();
                break;
                
            case MENU_TYPE_ACTION_INT_FACTOR:
                s_set_default_int_factor_value ();
                break;
            default:
                break;
        }
    } else {
        // Возврат к первому элементу root
        s_set_menu(s_menu_config[MENU_ID_ROOT].first_child);
    }

    s_exec_callback_action();
}

static void s_default_menu_draw(void) {
    const menu_item_t *config = menu_get_current_config();
    menu_item_values_t *values = menu_get_current_values();

    switch (config->type) {
        
        case MENU_TYPE_ACTION_BOOL: {
            lcd1602_print(values->action_bool.value ? "ON " : "OFF");
        } break;
        case MENU_TYPE_ACTION_INT: {
            lcd1602_printf("%d/%d", values->action_int.value, config->config.action_int.max);
        } break;
            
        case MENU_TYPE_ACTION_INT_FACTOR: {
            lcd1602_printf("%d (x%d)", values->action_int_factor.value, s_get_current_factor());
        } break;
        case MENU_TYPE_ACTION_FIXED_INT: {
            lcd1602_printf("%d", config->config.action_fixed_int.massif[values->action_fixed_int.value_idx]);
        } break;
        case MENU_TYPE_ACTION_FIXED_STRING: {
            lcd1602_print(config->config.action_fixed_string.massif[values->action_fixed_string.value_idx]);
        } break;
            
        case MENU_TYPE_ACTION_CALLBACK:
            break;
      
        case MENU_TYPE_ACTION_MENU:
            // Показываем, что это подменю со стрелкой
            lcd1602_print(">");
            break;
            
        default:
            lcd1602_print("");
            break;
    }
}

void menu_draw(void) {
    const menu_item_t *config = menu_get_current_config();
    menu_item_values_t *values = menu_get_current_values();

    lcd1602_clear();
    
    // Первая строка: заголовок текущего меню
    lcd1602_set_cursor(0, 0);
    lcd1602_print(config->title);

    // Вторая строка: значение или навигация
    lcd1602_set_cursor(0, 1);

    const char *str = 0;

    if (s_display_str_cb) {
        str = s_display_str_cb(s_menu_values.state_info.current);
    }

    if (str != 0) {
        lcd1602_print(str);
    } else {
        s_default_menu_draw();
    }
    
    // Добавляем индикатор состояния редактирования
    if (s_menu_values.state_info.state == MENU_STATE_EDIT) {
        lcd1602_set_cursor(15, 1);  // Правый нижний угол
        lcd1602_print("*");
    }

    s_menu_values.state_info.dirty = false;
}

bool menu_is_dirty(void)
{
    return s_menu_values.state_info.dirty;
}

bool menu_is_editable(void) {
    return (
        s_menu_values.state_info.state == MENU_STATE_EDIT 
        && ((s_menu_values.state_info.menu_event & MENU_EVENT_ENTER_EDIT) == 0)
    );
}

void menu_set_dirty(bool dirty) {
    s_menu_values.state_info.dirty = dirty;
}

/**
  * Навигация по соседним меню
  */
static void s_navigate_sibling(void) {
    if (s_menu_values.state_info.state != MENU_STATE_NAVIGATION)
        return;
        
    const menu_item_t *config = menu_get_current_config();
    menu_item_values_t *values = menu_get_current_values();
    
    menu_id_t prev_menu_id = s_menu_values.state_info.current;

    menu_id_t target = s_menu_values.state_info.delta > 0 ? config->next_sibling : config->prev_sibling;

    if (target != MENU_ID_COUNT) {
        s_set_menu(target);
    } else {
        // Зацикливание навигации
        if (s_menu_values.state_info.delta > 0) {
            // Ищем первого sibling
            menu_id_t first_sibling = config->first_sibling;
            if (first_sibling != MENU_ID_COUNT) {
                s_set_menu(first_sibling);
            }
        } else {
            // Ищем последнего sibling
            menu_id_t last_sibling = config->last_sibling;
            if (last_sibling != MENU_ID_COUNT) {
                s_set_menu(last_sibling);
            }
        }
    }
}

static void s_set_edit_state(void) {
    const menu_item_t *config = menu_get_current_config();
    if (config->first_child != MENU_ID_COUNT)
        return; 

    if (config->type != MENU_TYPE_ACTION_MENU) {
        // Переход в режим редактирования
        s_menu_values.state_info.menu_event |= MENU_EVENT_ENTER_EDIT;
        s_menu_set_state(MENU_STATE_EDIT);
    }
}

static void s_change_value(void) {
    if (s_menu_values.state_info.state != MENU_STATE_EDIT)
        return;
    switch(s_menu_values.state_info.current) {
        case MENU_ID_REGIME:
            s_change_fixed_string_value();
        break;
        case MENU_ID_DELIMITERS:
            s_change_fixed_int_value();
        break;
        case MENU_ID_HI_DELAY:
            s_change_int_factor_value();
        break;
        case MENU_ID_HI_DURATION:
            s_change_int_factor_value();
        break;
        case MENU_ID_HI_DUTY:
            s_change_int_value();
        break;
        case MENU_ID_LO_DELAY:
            s_change_int_factor_value();
        break;
        case MENU_ID_LO_DURATION:
            s_change_int_factor_value();
        break;
        case MENU_ID_LO_DUTY:
            s_change_int_value();
        break;
        default:
            break;
    }
}

static void s_next_value(void) {
    if (s_menu_values.state_info.state != MENU_STATE_EDIT)
        return;

    switch(s_menu_values.state_info.current) {
        case MENU_ID_START:
            s_toggle_bool_value();
        break;
        case MENU_ID_HI_DELAY:
            s_next_factor_idx();
        break;
        case MENU_ID_HI_DURATION:
            s_next_factor_idx();
        break;
        case MENU_ID_HI_PWM_ON:
            s_toggle_bool_value();
        break;
        case MENU_ID_LO_DELAY:
            s_next_factor_idx();
        break;
        case MENU_ID_LO_DURATION:
            s_next_factor_idx();
        break;
        case MENU_ID_LO_PWM_ON:
            s_toggle_bool_value();
        break;
        default:
            break;
    }
}


static void s_set_menu(menu_id_t menu_id) {
    if (s_menu_values.state_info.current == menu_id || menu_id == MENU_ID_COUNT || menu_id == MENU_ID_ROOT)
        return;

    s_menu_values.state_info.previous = s_menu_values.state_info.current;
    s_menu_values.state_info.current = menu_id;
    s_menu_values.state_info.menu_event |= MENU_EVENT_ITEM_CHANGED;
    s_call_menu_event_cb ();
    s_menu_values.state_info.dirty = true;
}

static void s_menu_set_state(menu_state_t state) {
    if (s_menu_values.state_info.state == state)
        return;
    s_menu_values.state_info.state = state;
    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}

static void s_next_factor_idx (void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();
    switch(config->type) {
        case MENU_TYPE_ACTION_INT_FACTOR:
            values->action_int_factor.factor_idx =
                (values->action_int_factor.factor_idx + 1) %
                config->config.action_int_factor.factor.count;
            break;
            
        default:
            break;
    }
    s_menu_values.state_info.menu_event |= MENU_EVENT_FACTOR_CHANGED;
    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}

static int32_t s_get_current_factor(void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();

    switch(config->type) {
        
        case MENU_TYPE_ACTION_INT_FACTOR:
            return config->config.action_int_factor.factor.factors[values->action_int_factor.factor_idx];
        default:
            return 0;
    }

    return 0;
}

static void s_toggle_bool_value(void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();
    if (config->type != MENU_TYPE_ACTION_BOOL)
        return; 

    s_menu_values.state_info.menu_event |= MENU_EVENT_VALUE_CHANGED;
    values->action_bool.value = !values->action_bool.value;
    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}

static void s_set_default_bool_value (void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();
    values->action_bool.value = config->config.action_bool.default_value;
    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}

static void s_next_int_value(void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();

    if (config->type != MENU_TYPE_ACTION_INT)
        return; 

    s_menu_values.state_info.menu_event |= MENU_EVENT_VALUE_CHANGED;

    values->action_int.value += config->config.action_int.step;
    if (values->action_int.value > config->config.action_int.max)
        values->action_int.value = config->config.action_int.max;

    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}

static void s_change_int_value(void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();

    if (config->type != MENU_TYPE_ACTION_INT)
        return; 

    s_menu_values.state_info.menu_event |= MENU_EVENT_VALUE_CHANGED;

    values->action_int.value += s_menu_values.state_info.delta * config->config.action_int.step;
    values->action_int.value = CLAMP(values->action_int.value,
        config->config.action_int.min, config->config.action_int.max);
    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}

static void s_set_default_int_value (void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();
    values->action_int.value = config->config.action_int.default_value;
    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}

static void s_change_int_factor_value(void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();

    if (config->type != MENU_TYPE_ACTION_INT_FACTOR)
        return;

    s_menu_values.state_info.menu_event |= MENU_EVENT_VALUE_CHANGED;

    int32_t factor = s_get_current_factor();

    if (!factor)
        return;

    values->action_int.value += s_menu_values.state_info.delta * factor;
    values->action_int.value = CLAMP(values->action_int.value,
        config->config.action_int.min, config->config.action_int.max);
    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}

static void s_set_default_int_factor_value (void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();
    values->action_int_factor.value = config->config.action_int_factor.default_value;
    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}


static void s_next_fixed_int_value(void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();
    values->action_fixed_int.value_idx = 
        (values->action_fixed_int.value_idx + 1) 
            % config->config.action_fixed_int.counter;
    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}

static void s_change_fixed_int_value(void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();
    if (s_menu_values.state_info.delta > 0) {
        values->action_fixed_int.value_idx = 
            (values->action_fixed_int.value_idx + s_menu_values.state_info.delta) 
                % config->config.action_fixed_int.counter;
    } else {
        values->action_fixed_int.value_idx = 
            (values->action_fixed_int.value_idx >= -s_menu_values.state_info.delta) ? 
                (values->action_fixed_int.value_idx + s_menu_values.state_info.delta) : config->config.action_fixed_int.counter - 1;
    }

    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}

static void s_set_default_fixed_int_value(void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();
    values->action_fixed_int.value_idx = config->config.action_fixed_int.default_idx;
    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}
static void s_next_fixed_string_value(void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();

    values->action_fixed_string.value_idx = 
        (values->action_fixed_string.value_idx + 1) %
        config->config.action_fixed_string.counter;
    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}

static void s_change_fixed_string_value(void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();

    if (s_menu_values.state_info.delta > 0) {
        values->action_fixed_string.value_idx = 
            (values->action_fixed_string.value_idx + 1) %
            config->config.action_fixed_string.counter;
    } else {
        values->action_fixed_string.value_idx = 
            values->action_fixed_string.value_idx >= - s_menu_values.state_info.delta ? 
                values->action_fixed_string.value_idx + s_menu_values.state_info.delta : config->config.action_fixed_string.counter - 1;
    }
    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}

static void s_set_default_fixed_string_value(void) {
    menu_item_values_t *values = menu_get_current_values();
    const menu_item_t *config = menu_get_current_config();

    values->action_fixed_string.value_idx = 
        config->config.action_fixed_string.default_idx;

    s_call_menu_event_cb();
    s_menu_values.state_info.dirty = true;
}

static void s_call_menu_event_cb(void) {
    if (s_menu_event_cb) {
        s_menu_event_cb(&s_menu_values.state_info, menu_get_current_values());
    }
    s_reset_state_info();
}

static void s_exec_callback_action(void) {
    const menu_item_t *config = menu_get_current_config();
    if (config->type == MENU_TYPE_ACTION_CALLBACK) {
        s_call_menu_event_cb();
        s_menu_values.state_info.dirty = true;
    }
}

static void s_reset_state_info(void) {
    s_menu_values.state_info.menu_event = 0;
    s_menu_values.state_info.control_event = 0;
    s_menu_values.state_info.delta = 0;
}
#ifndef MENU_TYPES_H
#define MENU_TYPES_H

typedef enum {
    MENU_ID_ROOT,
    MENU_ID_START,
    MENU_ID_REGIMES,
    MENU_ID_VERSION,
    MENU_ID_SETTINGS,
    MENU_ID_PWM_FREQUENCY,
    MENU_ID_HI_CHANNEL,
    MENU_ID_HI_DELAY,
    MENU_ID_HI_DURATION,
    MENU_ID_HI_PWM_ON,
    MENU_ID_HI_DUTY,
    MENU_ID_LO_CHANNEL,
    MENU_ID_LO_DELAY,
    MENU_ID_LO_DURATION,
    MENU_ID_LO_PWM_ON,
    MENU_ID_LO_DUTY,
    MENU_ID_COUNT
} menu_id_t;

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

typedef enum {
    MENU_STATE_NONE = 0,
    MENU_STATE_NAVIGATION = 1,
    MENU_STATE_EDIT = 2,
    MENU_STATE_COUNT = 3
} menu_state_t;

#define LCD_STRING_LEN 0x20
#define LCD_NUM_STRINGS 2

typedef struct {
    menu_id_t id;
    const char name[0x20];
} menu_id_names_t;

// Базовые структуры (только forward declarations)
typedef struct s_menu_context_t menu_context_t;
typedef struct s_menu_node_t menu_node_t;
typedef struct s_menu_node_config_t menu_node_config_t;
typedef struct s_menu_node_value_t menu_node_value_t;
typedef struct s_menu_callback_t menu_callback_t;

const char *menu_types_get_name(menu_id_t id);


#endif /* MENU_TYPES_H */

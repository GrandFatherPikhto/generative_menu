#ifndef MENU_COMMON_H
#define MENU_COMMON_H

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

typedef struct {
    menu_id_t id;
    const char name[0x20];
} menu_id_names_t;

static menu_id_names_t menu_items[] = {
    [MENU_ID_ROOT] = {MENU_ID_ROOT, "MENU_ID_ROOT"},
    [MENU_ID_START] = {MENU_ID_START, "MENU_ID_START"},
    [MENU_ID_REGIMES] = {MENU_ID_REGIMES, "MENU_ID_REGIMES"},
    [MENU_ID_VERSION] = {MENU_ID_VERSION, "MENU_ID_VERSION"},
    [MENU_ID_SETTINGS] = {MENU_ID_SETTINGS, "MENU_ID_SETTINGS"},
    [MENU_ID_PWM_FREQUENCY] = {MENU_ID_PWM_FREQUENCY, "MENU_ID_PWM_FREQUENCY"},
    [MENU_ID_HI_CHANNEL] = {MENU_ID_HI_CHANNEL, "MENU_ID_HI_CHANNEL"},
    [MENU_ID_HI_DELAY] = {MENU_ID_HI_DELAY, "MENU_ID_HI_DELAY"},
    [MENU_ID_HI_DURATION] = {MENU_ID_HI_DURATION, "MENU_ID_HI_DURATION"},
    [MENU_ID_HI_DUTY] = {MENU_ID_HI_DUTY, "MENU_ID_HI_DUTY"},
    [MENU_ID_HI_PWM_ON] = {MENU_ID_HI_PWM_ON, "MENU_ID_HI_PWM_ON"},
    [MENU_ID_LO_CHANNEL] = {MENU_ID_LO_CHANNEL, "MENU_ID_LO_CHANNEL"},
    [MENU_ID_LO_DELAY] = {MENU_ID_LO_DELAY, "MENU_ID_LO_DELAY"},
    [MENU_ID_LO_DURATION] = {MENU_ID_LO_DURATION, "MENU_ID_LO_DURATION"},
    [MENU_ID_LO_DUTY] = {MENU_ID_LO_DUTY, "MENU_ID_LO_DUTY"},
    [MENU_ID_LO_PWM_ON] = {MENU_ID_LO_PWM_ON, "MENU_ID_LO_PWM_ON"},
    [MENU_ID_COUNT] = {MENU_ID_COUNT, "MENU_ID_COUNT"},
};

#define LCD_STRING_LEN 0x20
#define LCD_NUM_STRINGS 2

// Базовые структуры (только forward declarations)
typedef struct s_menu_context_t menu_context_t;
typedef struct s_menu_node_t menu_node_t;
typedef struct s_menu_node_config_t menu_node_config_t;
typedef struct s_menu_node_value_t menu_node_value_t;

#endif /* MENU_COMMON_H */

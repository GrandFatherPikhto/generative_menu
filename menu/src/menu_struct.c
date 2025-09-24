#include "menu_struct.h"

typedef struct menu_node {
    menu_id_t id;
    menu_id_t parent;
    menu_id_t child;
    menu_id_t prev;
    menu_id_t next;
    const char *title;
} menu_node_t;

static const menu_node_t s_menu_nodes[] = {
    [MENU_ID_ROOT] = {
        .id = MENU_ID_ROOT,
        .title = "root",
        .parent = MENU_ID_COUNT,
        .child = MENU_ID_START,
        .prev = MENU_ID_COUNT,
        .next = MENU_ID_COUNT,
    },
    [MENU_ID_START] = {
        .id = MENU_ID_START,
        .title = "Start",
        .parent = MENU_ID_ROOT,
        .child = MENU_ID_COUNT,
        .prev = MENU_ID_SETTINGS,
        .next = MENU_ID_REGIMES,
    },
    [MENU_ID_REGIMES] = {
        .id = MENU_ID_REGIMES,
        .title = "Regimes",
        .parent = MENU_ID_ROOT,
        .child = MENU_ID_COUNT,
        .prev = MENU_ID_START,
        .next = MENU_ID_VERSION,
    },
    [MENU_ID_VERSION] = {
        .id = MENU_ID_VERSION,
        .title = "Firmware version",
        .parent = MENU_ID_ROOT,
        .child = MENU_ID_COUNT,
        .prev = MENU_ID_REGIMES,
        .next = MENU_ID_SETTINGS,
    },
    [MENU_ID_SETTINGS] = {
        .id = MENU_ID_SETTINGS,
        .title = "Settings",
        .parent = MENU_ID_ROOT,
        .child = MENU_ID_PWM_FREQUENCY,
        .prev = MENU_ID_VERSION,
        .next = MENU_ID_COUNT,
    },
    [MENU_ID_PWM_FREQUENCY] = {
        .id = MENU_ID_PWM_FREQUENCY,
        .title = "PWM Frequency",
        .parent = MENU_ID_SETTINGS,
        .child = MENU_ID_COUNT,
        .prev = MENU_ID_LO_CHANNEL,
        .next = MENU_ID_HI_CHANNEL,
    },
    [MENU_ID_HI_CHANNEL] = {
        .id = MENU_ID_HI_CHANNEL,
        .title = "Hi Channel",
        .parent = MENU_ID_SETTINGS,
        .child = MENU_ID_HI_DELAY,
        .prev = MENU_ID_PWM_FREQUENCY,
        .next = MENU_ID_LO_CHANNEL,
    },
    [MENU_ID_HI_DELAY] = {
        .id = MENU_ID_HI_DELAY,
        .title = "Delay",
        .parent = MENU_ID_HI_CHANNEL,
        .child = MENU_ID_COUNT,
        .prev = MENU_ID_COUNT,
        .next = MENU_ID_HI_DURATION,
    },
    [MENU_ID_HI_DURATION] = {
        .id = MENU_ID_HI_DURATION,
        .title = "Duration",
        .parent = MENU_ID_HI_CHANNEL,
        .child = MENU_ID_COUNT,
        .prev = MENU_ID_HI_DELAY,
        .next = MENU_ID_HI_PWM_ON,
    },
    [MENU_ID_HI_PWM_ON] = {
        .id = MENU_ID_HI_PWM_ON,
        .title = "PWM On/Off",
        .parent = MENU_ID_HI_CHANNEL,
        .child = MENU_ID_COUNT,
        .prev = MENU_ID_HI_DURATION,
        .next = MENU_ID_HI_DUTY,
    },
    [MENU_ID_HI_DUTY] = {
        .id = MENU_ID_HI_DUTY,
        .title = "PWM Duty",
        .parent = MENU_ID_HI_CHANNEL,
        .child = MENU_ID_COUNT,
        .prev = MENU_ID_HI_PWM_ON,
        .next = MENU_ID_COUNT,
    },
    [MENU_ID_LO_CHANNEL] = {
        .id = MENU_ID_LO_CHANNEL,
        .title = "Lo Channel",
        .parent = MENU_ID_SETTINGS,
        .child = MENU_ID_LO_DELAY,
        .prev = MENU_ID_HI_CHANNEL,
        .next = MENU_ID_COUNT,
    },
    [MENU_ID_LO_DELAY] = {
        .id = MENU_ID_LO_DELAY,
        .title = "Delay",
        .parent = MENU_ID_LO_CHANNEL,
        .child = MENU_ID_COUNT,
        .prev = MENU_ID_COUNT,
        .next = MENU_ID_LO_DURATION,
    },
    [MENU_ID_LO_DURATION] = {
        .id = MENU_ID_LO_DURATION,
        .title = "Duration",
        .parent = MENU_ID_LO_CHANNEL,
        .child = MENU_ID_COUNT,
        .prev = MENU_ID_LO_DELAY,
        .next = MENU_ID_LO_PWM_ON,
    },
    [MENU_ID_LO_PWM_ON] = {
        .id = MENU_ID_LO_PWM_ON,
        .title = "PWM On/Off",
        .parent = MENU_ID_LO_CHANNEL,
        .child = MENU_ID_COUNT,
        .prev = MENU_ID_LO_DURATION,
        .next = MENU_ID_LO_DUTY,
    },
    [MENU_ID_LO_DUTY] = {
        .id = MENU_ID_LO_DUTY,
        .title = "PWM Duty",
        .parent = MENU_ID_LO_CHANNEL,
        .child = MENU_ID_COUNT,
        .prev = MENU_ID_LO_PWM_ON,
        .next = MENU_ID_COUNT,
    },
    
}; 

const menu_node_t *menu_get_node(menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return &(s_menu_nodes[id]);
}

menu_id_t menu_get_next(menu_id_t id) {
    const menu_node_t *node = menu_get_node(id);
    if (node == 0)
        return MENU_ID_COUNT;

    if (node->next == MENU_ID_COUNT)
        return node->prev;
    
    return node->next;
}

menu_id_t menu_get_prev(menu_id_t id) {
    const menu_node_t *node = menu_get_node(id);
    if (node == 0)
        return MENU_ID_COUNT;
    return node->prev;
}

menu_id_t menu_get_parent (menu_id_t id) {
    const menu_node_t *node = menu_get_node(id);
    if (node == 0)
        return MENU_ID_COUNT;
    if (node->parent == MENU_ID_ROOT)
        return MENU_ID_COUNT;
    return node->parent;
}

menu_id_t menu_get_child (menu_id_t id) {
    const menu_node_t *node = menu_get_node(id);
    if (node == 0)
        return MENU_ID_COUNT;
    return node->child;
}

const char *menu_get_title(menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return s_menu_nodes[id].title;
}
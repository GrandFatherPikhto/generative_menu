#ifndef MENU_STRUCT_H
#define MENU_STRUCT_H

#include <stdint.h>
#include <stdbool.h>

#define MENU_TITLE_LEN 0x10

typedef enum {
    MENU_TREE_TYPE_NONE = 0,
    MENU_TREE_TYPE_BRANCH = 1,
    MENU_TREE_TYPE_LEAF = 2,
    MENU_TREE_TYPE_COUNT = 3
} menu_tree_type_t;

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

typedef struct menu_node menu_node_t;

typedef struct menu_node {
    menu_id_t id;
    menu_id_t parent;
    menu_id_t child;
    menu_id_t prev;
    menu_id_t next;
    const char *title;
    menu_tree_type_t type;
} menu_node_t;

const menu_node_t *menu_get_node(menu_id_t id);
menu_id_t menu_get_next(menu_id_t id);
menu_id_t menu_get_prev(menu_id_t id);
menu_id_t menu_get_parent(menu_id_t id);
menu_id_t menu_get_child(menu_id_t id);
const char *menu_get_title(menu_id_t id);
menu_tree_type_t menu_get_tree_type(menu_id_t id);
const menu_node_t *menu_get_tree(void);

#endif /* MENU_STRUCT_H */
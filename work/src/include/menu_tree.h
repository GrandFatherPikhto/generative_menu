#ifndef MENU_TREE_H
#define MENU_TREE_H

#include <stdint.h>
#include <stdbool.h>

#include "menu_common.h"

#define MENU_TITLE_LEN 0x10

typedef enum {
    MENU_TREE_TYPE_NONE = 0,
    MENU_TREE_TYPE_BRANCH = 1,
    MENU_TREE_TYPE_LEAF = 2,
    MENU_TREE_TYPE_COUNT = 3
} menu_tree_type_t;

typedef struct s_menu_node_t {
    menu_id_t id;
    menu_id_t parent;
    menu_id_t child;
    menu_id_t prev;
    menu_id_t next;
    const char *title;
    menu_tree_type_t type;
} menu_node_t;

const menu_node_t *menu_tree_get_nodes(void);
const menu_id_t menu_tree_get_first_id(void);

#endif /* MENU_TREE_H */

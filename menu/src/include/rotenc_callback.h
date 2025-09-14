#ifndef ROTENC_CALLBACK_H
#define ROTENC_CALLBACK_H

#include <stdint.h>
#include "rotenc_menu.h"

void collback_menu_event(menu_state_info_t *state, menu_item_values_t *value);
const char *collback_display_str(menu_id_t menu_id);

#endif // ROTENC_CALLBACK_H
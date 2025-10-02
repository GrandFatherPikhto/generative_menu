#ifndef MENU_DATA_H
#define MENU_DATA_H

#include <stdint.h>
#include <stdbool.h>

#include "menu_type.h"

void rotenc_hi_duty_changed(menu_context_t *ctx, menu_id_t id, menu_event_t event);
void pwm_frequency_display_cb(menu_context_t *ctx, menu_id_t id);
void pwm_frequency_change_cb(menu_context_t *ctx, menu_id_t id, int8_t delta);
void my_event_cb(menu_context_t *ctx, menu_id_t id, menu_event_t event);
void draw_version_cb(menu_context_t *ctx, menu_id_t);

#endif /* MENU_DATA_H */
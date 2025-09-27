#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <stdint.h>
#include <stdbool.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum {
    RENC_ACTION_NONE,
    RENC_ACTION_CHANGE_POSITION,
    RENC_ACTION_NEXT,
    RENC_ACTION_PREV,
    RENC_ACTION_PUSH_BUTTON,
    RENC_ACTION_LONG_PUSH_BUTTON,
    RENC_ACTION_DOUBLE_CLICK
} renc_state_t;

typedef struct renc_handle renc_handle_t;

typedef void (* position_cb_t) (int);
typedef void (* push_button_cb_t) (void);
typedef void (* long_push_buttont_cb_t) (void);
typedef void (* double_click_cb_t) (void);

typedef struct renc_cb {
    position_cb_t position_cb;
    push_button_cb_t push_cb;
    long_push_buttont_cb_t long_push_cb;
    double_click_cb_t double_click_cb;
} renc_cb_t;

#define renc_DEBOUNCE 2

void renc_init(renc_handle_t *handle);
void renc_change_position(int pos);
void renc_push_button(void);
void renc_long_push_button(void);
void renc_double_click_button(void);
bool renc_state(renc_state_t state);
// void renc_is_state(void);
void renc_reset_state(void);
int8_t renc_get_delta(void);

#endif /* ROTARY_ENCODER_H */

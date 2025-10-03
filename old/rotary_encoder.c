#include "rotary_encoder.h"
#include "common.h"

#define ENCODER_INPUT_FILTER 2

typedef enum {
    ROTARY_ENCODER_NONE = 0,
    ROTARY_ENCODER_CHANGE_POSITION = 1,
    ROTARY_ENCODER_PUSH_BUTTON = 2,
    ROTARY_ENCODER_LONG_PUSH_BUTTON = 3,
    ROTARY_ENCODER_DOUBLE_CLICK = 4
} renc_action_t;

struct renc_handle {
    int8_t delta;
    int pos;
    int prev;
    uint8_t state;
};

static struct renc_handle *s_handle;

void renc_init(renc_handle_t *handle)
{
    static renc_handle_t renc_handle = {0};
    handle = &renc_handle;
    s_handle = handle;
}

void renc_change_position(int pos)
{
    if ((pos / ENCODER_INPUT_FILTER) * ENCODER_INPUT_FILTER != pos) { // Дебаунсинг и Фильтрация
      return; // Неправильное значение, игнорировать
    }

    s_handle->delta = (int)(pos / ENCODER_INPUT_FILTER) - (int)s_handle->pos;
    s_handle->prev  = s_handle->pos;
    s_handle->pos += s_handle->delta;
    SET_BIT(s_handle->state, ROTARY_ENCODER_CHANGE_POSITION);
    // printf("%s:%d %s, pos: %d, delta: %d\n", __FILE__, __LINE__, IS_BIT_SET(s_handle->state, ROTARY_ENCODER_CHANGE_POSITION) ? "Yes" : "No", s_handle->pos, s_handle->delta);
}

void renc_push_button(void)
{
    SET_BIT(s_handle->state, ROTARY_ENCODER_PUSH_BUTTON);
}

void renc_long_push_button(void)
{
    SET_BIT(s_handle->state, ROTARY_ENCODER_LONG_PUSH_BUTTON);
}

void renc_double_click_button(void)
{
    SET_BIT(s_handle->state, ROTARY_ENCODER_DOUBLE_CLICK);
}

void renc_reset_state(void)
{
    s_handle->state = 0;
}

int8_t renc_get_delta(void)
{
    return s_handle->delta;
}

bool renc_state(renc_state_t state)
{
    switch (state)
    {
    case RENC_ACTION_NONE:
        return (s_handle->state == 0);
    
    case RENC_ACTION_CHANGE_POSITION:
        return (IS_BIT_SET(s_handle->state, ROTARY_ENCODER_CHANGE_POSITION));

    case RENC_ACTION_NEXT:
        return ((IS_BIT_SET(s_handle->state, ROTARY_ENCODER_CHANGE_POSITION)) 
            && s_handle->delta > 0);

    case RENC_ACTION_PREV:
        return (IS_BIT_SET(s_handle->state, ROTARY_ENCODER_CHANGE_POSITION) 
            && s_handle->delta < 0);

    case RENC_ACTION_PUSH_BUTTON:
        return IS_BIT_SET(s_handle->state, ROTARY_ENCODER_PUSH_BUTTON);
    
    case RENC_ACTION_LONG_PUSH_BUTTON:
        return IS_BIT_SET(s_handle->state, ROTARY_ENCODER_LONG_PUSH_BUTTON);

    case RENC_ACTION_DOUBLE_CLICK:
        return IS_BIT_SET(s_handle->state, ROTARY_ENCODER_DOUBLE_CLICK);

    default:
        break;
    }

    return false;
}
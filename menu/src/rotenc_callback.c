#include "rotenc_callback.h"
#include "rotenc_menu.h"

#include <string.h>
#include <stdio.h>

#define DEFAULT_FREQUENCY_IDX 4
#define STR_LEN 0x10
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

typedef struct {
    uint8_t frequency_idx;
    char str[STR_LEN];
} rotenc_data_context_t;

static rotenc_data_context_t s_context = {0};

typedef struct {
   int divider;
   int frequency;
} frequency_t;

static frequency_t s_frequencies[] = {
        {10000, 1000}, // 1.0 kHz
        {8000, 1250}, // 1.25 kHz
        {6250, 1600}, // 1.6 kHz
        {5000, 2000}, // 2.0 kHz
        {4000, 2500}, // 2.5 kHz
        {3200, 3125}, // 3.125 kHz
        {3125, 3200}, // 3.2 kHz
        {2500, 4000}, // 4.0 kHz
        {2000, 5000}, // 5.0 kHz
        {1600, 6250}, // 6.25 kHz
        {1250, 8000}, // 8.0 kHz
        {1000, 10000}, // 10.0 kHz
        {800, 12500}, // 12.5 kHz
        {640, 15625}, // 15.625 kHz
        {625, 16000}, // 16.0 kHz
        {500, 20000}, // 20.0 kHz
        {400, 25000}, // 25.0 kHz
        {320, 31250}, // 31.25 kHz
        {250, 40000}, // 40.0 kHz
        {200, 50000}, // 50.0 kHz
        {160, 62500}, // 62.5 kHz
        {128, 78125}, // 78.125 kHz
        {125, 80000}, // 80.0 kHz
        {100, 100000}, // 0.1 MHz
        {80, 125000}, // 0.125 MHz
        {64, 156250}, // 0.15625 MHz
        {50, 200000}, // 0.2 MHz
        {40, 250000}, // 0.25 MHz
        {32, 312500}, // 0.3125 MHz
        {25, 400000}, // 0.4 MHz
        {20, 500000}, // 0.5 MHz
        {16, 625000}, // 0.625 MHz
        {10, 1000000}, // 1.0 MHz
        {8, 1250000}, // 1.25 MHz
        {5, 2000000}, // 2.0 MHz
};

static void s_pwm_change(int8_t delta);
const char *s_pwm_display(void);

void rotenc_callback_init(void) {
    s_context.frequency_idx = DEFAULT_FREQUENCY_IDX;
}

void collback_menu_event(menu_state_info_t *state, menu_item_values_t *value) {
    // printf("Menu %d, events: %u, delta: %d, value: %p\n", state->current, state->event, state->delta, value);
    switch (state->current)
    {
    case MENU_ID_PWM_FREQUENCY:
        if (menu_is_editable())
            s_pwm_change(state->delta);
        break;
    
    default:
        break;
    }
}

const char *collback_display_str(menu_id_t menu_id) {
    switch (menu_id)
    {
    case MENU_ID_PWM_FREQUENCY:
        printf("Menu %d, frequency_idx: %u\n", menu_id, s_context.frequency_idx);
        return s_pwm_display();        

    default:
        return 0;
    }

    return 0;
}

const char *s_pwm_display(void) {
    int32_t frequency = s_frequencies[s_context.frequency_idx].frequency;
    printf("Frequency_idx: %u, Frequency: %d\n", s_context.frequency_idx, frequency);
    if (frequency < 1000)
        snprintf(s_context.str, STR_LEN, "%u", frequency);
    else if (frequency >= 1000 && frequency < 100000)
        snprintf(s_context.str, STR_LEN, "%2.2f kHz", (float)frequency/1000.0);
    else if (frequency >= 100000)
        snprintf(s_context.str, STR_LEN, "%1.2f MHz", (float)frequency/1000000.0);
    
    return s_context.str;
}

static void s_pwm_change(int8_t delta) {
    uint8_t size = sizeof(s_frequencies) / sizeof(frequency_t);
    s_context.frequency_idx = CLAMP(s_context.frequency_idx + delta, 0, size - 1);
    printf("%s:%d idx: %u, freq: %d\n", __FILE__, __LINE__, s_context.frequency_idx, s_frequencies[s_context.frequency_idx].frequency);
    menu_set_dirty(true);
}

const char * callback_info_display(void) {
    return "Display";
}

void callback_info_change(int8_t delta) {

}

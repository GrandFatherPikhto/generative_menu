#include "menu_handle.h"

#include <stdio.h>

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

void rotenc_hi_duty_changed(menu_context_t *ctx, menu_id_t id, menu_event_t event) {
    printf("%s:%d %d changed, event: %d\n", __FILE__, __LINE__, id, event);
}

void pwm_frequency_display_cb(menu_context_t *ctx, menu_id_t id) {
    int32_t frequency = s_frequencies[s_context.frequency_idx].frequency;
    // printf("Frequency_idx: %u, Frequency: %d\n", s_context.frequency_idx, frequency);
    char *value_str = menu_get_display_value();
    menu_state_t state = menu_get_state();
    int len = 0;

    if (frequency < 1000) {
        len = snprintf(NULL, 0, "%u", frequency);
        snprintf(value_str, LCD_STRING_LEN, "%u%*c"
            , frequency
            , 15 - len > 0 ? 15 - len : 0
            , state == MENU_STATE_EDIT ? '*' : '>');
    } else if (frequency >= 1000 && frequency < 100000) {
        len = snprintf(NULL, 0, "%2.2f kHz", (float)frequency/1000.0);
        snprintf(value_str, LCD_STRING_LEN, "%2.2f kHz%*c"
            , (float)frequency/1000.0
            , 15 - len > 0 ? 15 - len : 0
            , state == MENU_STATE_EDIT ? '*' : '>');
    } else if (frequency >= 100000) {
        len = snprintf(NULL, 0, "%1.2f MHz", (float)frequency/1000000.0);
        snprintf(value_str, LCD_STRING_LEN, "%1.2f MHz%*c"
            , (float)frequency/1000000.0
            , 15 - len > 0 ? 15 - len : 0
            , state == MENU_STATE_EDIT ? '*' : '>');
    }
    menu_set_dirty();
}

void pwm_frequency_change_cb(menu_context_t *ctx, menu_id_t id, int8_t delta) {
    uint8_t size = sizeof(s_frequencies) / sizeof(frequency_t);
    s_context.frequency_idx = CLAMP(s_context.frequency_idx + delta, 0, size - 1);
    // printf("%s:%d idx: %u, freq: %d\n", __FILE__, __LINE__, s_context.frequency_idx, s_frequencies[s_context.frequency_idx].frequency);
    menu_set_dirty();
}


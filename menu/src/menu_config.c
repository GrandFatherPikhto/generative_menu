#include "menu_config.h"
#include "menu_struct.h"

static const uint32_t s_factors_hi_delay[] = { 1, 10, 100, 1000 };
static const uint32_t s_factors_hi_duration[] = { 1, 10, 100, 1000 };
static const uint32_t s_factors_lo_delay[] = { 1, 10, 100, 1000 };
static const uint32_t s_factors_lo_duration[] = { 1, 10, 100, 1000 };

static const char *s_values_str_start[] = { "Start", "Started" };
static const char *s_values_str_regimes[] = { "Simple", "Strong", "Quality" };
static const char *s_values_str_hi_pwm_on[] = { "Off", "On" };
static const char *s_values_str_lo_pwm_on[] = { "Off", "On" };

/**
  * @note Пока тип callback не поддерживается
  **/
static const menu_item_config_t s_nodes_data[] = {
    [MENU_ID_START] = {        
        .id = MENU_ID_START,
        .click_cb = _cb,
        .position_cb = 0,
        .data.string_fixed = {
            .count = 2,
            .default_idx = 0,
            .values = s_values_str_start
        }
    },
    [MENU_ID_REGIMES] = {        
        .id = MENU_ID_REGIMES,
        .click_cb = 0,
        .position_cb = _cb,
        .data.string_fixed = {
            .count = 3,
            .default_idx = 2,
            .values = s_values_str_regimes
        }
    },
    [MENU_ID_HI_DELAY] = {        
        .id = MENU_ID_HI_DELAY,
        .click_cb = 0,
        .position_cb = _cb,
        .data.udword_factor = {
            .min = 10,
            .max = 10000,
            .default_value = 10,
            .default_idx = 0,
            .count = 4,
            .factors = s_factors_hi_delay
        }
    },
    [MENU_ID_HI_DURATION] = {        
        .id = MENU_ID_HI_DURATION,
        .click_cb = 0,
        .position_cb = _cb,
        .data.udword_factor = {
            .min = 10,
            .max = 10000,
            .default_value = 10,
            .default_idx = 2,
            .count = 4,
            .factors = s_factors_hi_duration
        }
    },
    [MENU_ID_HI_PWM_ON] = {        
        .id = MENU_ID_HI_PWM_ON,
        .click_cb = 0,
        .position_cb = _cb,
        .data.string_fixed = {
            .count = 2,
            .default_idx = 0,
            .values = s_values_str_hi_pwm_on
        }
    },
    [MENU_ID_HI_DUTY] = {        
        .id = MENU_ID_HI_DUTY,
        .click_cb = 0,
        .position_cb = _cb,
        .data.ubyte_simple = {
            .step = 1,
            .min = 0,
            .max = 95,
            .default_value = 30
        }
    },
    [MENU_ID_LO_DELAY] = {        
        .id = MENU_ID_LO_DELAY,
        .click_cb = 0,
        .position_cb = _cb,
        .data.udword_factor = {
            .min = 10,
            .max = 10000,
            .default_value = 10,
            .default_idx = 0,
            .count = 4,
            .factors = s_factors_lo_delay
        }
    },
    [MENU_ID_LO_DURATION] = {        
        .id = MENU_ID_LO_DURATION,
        .click_cb = 0,
        .position_cb = _cb,
        .data.udword_factor = {
            .min = 10,
            .max = 10000,
            .default_value = 10,
            .default_idx = 0,
            .count = 4,
            .factors = s_factors_lo_duration
        }
    },
    [MENU_ID_LO_PWM_ON] = {        
        .id = MENU_ID_LO_PWM_ON,
        .click_cb = 0,
        .position_cb = _cb,
        .data.string_fixed = {
            .count = 2,
            .default_idx = 0,
            .values = s_values_str_lo_pwm_on
        }
    },
    [MENU_ID_LO_DUTY] = {        
        .id = MENU_ID_LO_DUTY,
        .click_cb = 0,
        .position_cb = _cb,
        .data.ubyte_simple = {
            .step = 1,
            .min = 0,
            .max = 95,
            .default_value = 30
        }
    },
};

const menu_item_config_t *menu_get_config(menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return &(s_nodes_data[id]);
}

const menu_id_t get_first_id(void) {
    return MENU_ID_START;
}
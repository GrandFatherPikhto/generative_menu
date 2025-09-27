#include "menu_config.h"
#include "menu_draw.h"
#include "menu_tree.h"

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
static const menu_node_config_t s_menu_node_configs[] = {
    [MENU_ID_START] = {        
        .id = MENU_ID_START,
        .category = MENU_CATEGORY_STRING_FIXED,
        .click_cb = menu_edit_string_fixed_click_cyclic_cb,
        .position_cb = 0,
        .draw_value_cb = menu_draw_string_fixed_value_cb,
        .data.string_fixed = {
            .count = 2,
            .default_idx = 0,
            .values = s_values_str_start
        }
    },
    [MENU_ID_REGIMES] = {        
        .id = MENU_ID_REGIMES,
        .category = MENU_CATEGORY_STRING_FIXED,
        .click_cb = 0,
        .position_cb = menu_edit_string_fixed_position_cyclic_cb,
        .draw_value_cb = menu_draw_string_fixed_value_cb,
        .data.string_fixed = {
            .count = 3,
            .default_idx = 2,
            .values = s_values_str_regimes
        }
    },
    [MENU_ID_HI_DELAY] = {        
        .id = MENU_ID_HI_DELAY,
        .category = MENU_CATEGORY_UDWORD_FACTOR,
        .click_cb = menu_edit_udword_factor_click_cyclic_cb,
        .position_cb = menu_edit_udword_factor_position_limit_cb,
        .draw_value_cb = menu_draw_udword_factor_value_cb,
        .data.udword_factor = {
            .min = 10,
            .max = 1000,
            .default_value = 10,
            .default_idx = 0,
            .count = 4,
            .factors = s_factors_hi_delay
        }
    },
    [MENU_ID_HI_DURATION] = {        
        .id = MENU_ID_HI_DURATION,
        .category = MENU_CATEGORY_UDWORD_FACTOR,
        .click_cb = menu_edit_udword_factor_click_cyclic_cb,
        .position_cb = menu_edit_udword_factor_position_limit_cb,
        .draw_value_cb = menu_draw_udword_factor_value_cb,
        .data.udword_factor = {
            .min = 0,
            .max = 1000,
            .default_value = 10,
            .default_idx = 2,
            .count = 4,
            .factors = s_factors_hi_duration
        }
    },
    [MENU_ID_HI_PWM_ON] = {        
        .id = MENU_ID_HI_PWM_ON,
        .category = MENU_CATEGORY_STRING_FIXED,
        .click_cb = menu_edit_string_fixed_click_cyclic_cb,
        .position_cb = 0,
        .draw_value_cb = menu_draw_string_fixed_value_cb,
        .data.string_fixed = {
            .count = 2,
            .default_idx = 0,
            .values = s_values_str_hi_pwm_on
        }
    },
    [MENU_ID_HI_DUTY] = {        
        .id = MENU_ID_HI_DUTY,
        .category = MENU_CATEGORY_UBYTE_SIMPLE,
        .click_cb = 0,
        .position_cb = menu_edit_ubyte_simple_position_limit_cb,
        .draw_value_cb = menu_draw_ubyte_simple_value_cb,
        .data.ubyte_simple = {
            .step = 1,
            .min = 0,
            .max = 95,
            .default_value = 30
        }
    },
    [MENU_ID_LO_DELAY] = {        
        .id = MENU_ID_LO_DELAY,
        .category = MENU_CATEGORY_UDWORD_FACTOR,
        .click_cb = menu_edit_udword_factor_click_cyclic_cb,
        .position_cb = menu_edit_udword_factor_position_limit_cb,
        .draw_value_cb = menu_draw_udword_factor_value_cb,
        .data.udword_factor = {
            .min = 10,
            .max = 1000,
            .default_value = 10,
            .default_idx = 0,
            .count = 4,
            .factors = s_factors_lo_delay
        }
    },
    [MENU_ID_LO_DURATION] = {        
        .id = MENU_ID_LO_DURATION,
        .category = MENU_CATEGORY_UDWORD_FACTOR,
        .click_cb = menu_edit_udword_factor_click_cyclic_cb,
        .position_cb = menu_edit_udword_factor_position_limit_cb,
        .draw_value_cb = menu_draw_udword_factor_value_cb,
        .data.udword_factor = {
            .min = 10,
            .max = 1000,
            .default_value = 10,
            .default_idx = 0,
            .count = 4,
            .factors = s_factors_lo_duration
        }
    },
    [MENU_ID_LO_PWM_ON] = {        
        .id = MENU_ID_LO_PWM_ON,
        .category = MENU_CATEGORY_STRING_FIXED,
        .click_cb = menu_edit_string_fixed_click_cyclic_cb,
        .position_cb = 0,
        .draw_value_cb = menu_draw_string_fixed_value_cb,
        .data.string_fixed = {
            .count = 2,
            .default_idx = 0,
            .values = s_values_str_lo_pwm_on
        }
    },
    [MENU_ID_LO_DUTY] = {        
        .id = MENU_ID_LO_DUTY,
        .category = MENU_CATEGORY_UBYTE_SIMPLE,
        .click_cb = 0,
        .position_cb = menu_edit_ubyte_simple_position_limit_cb,
        .draw_value_cb = menu_draw_ubyte_simple_value_cb,
        .data.ubyte_simple = {
            .step = 1,
            .min = 0,
            .max = 95,
            .default_value = 30
        }
    },
};

const menu_node_config_t *menu_config_get_nodes(void) {
    return s_menu_node_configs;
}
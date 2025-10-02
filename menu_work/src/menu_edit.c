#include "menu_edit.h"
#include "menu_config.h"
#include "menu_value.h"
#include "menu_context.h"

/** @brief Функция для простейших числовых типов и строк
  * Последующее или предыдущее значение берётся из массива values
  * Если control == click, то navigation _только_ cycle.
  * @param id Идентификтор меню Определён, в enum в menu_struct.h
  * @param delta Количество "щелчков" энкодера. Передаётся только если control==position
  */
void menu_edit_string_fixed_click_cyclic_cb(menu_context_t *ctx, menu_id_t id) {
    const menu_node_config_t *config = &(ctx->configs[id]);
    menu_node_value_t *value = &(ctx->values[id]);

    uint8_t *idx = &(value->data.string_fixed.idx);
    uint8_t count = config->data.string_fixed.count;
    *idx = (*idx + 1) % count;

    ctx->dirty = true;
}

/** @brief Функция для простейших числовых типов и строк
  * Последующее или предыдущее значение берётся из массива values
  * Если control == click, то navigation _только_ cycle.
  * @param id Идентификтор меню Определён, в enum в menu_struct.h
  * @param delta Количество "щелчков" энкодера. Передаётся только если control==position
  */
void menu_edit_string_fixed_position_cyclic_cb(menu_context_t *ctx, menu_id_t id, int8_t delta) {
    const menu_node_config_t *config = &(ctx->configs[id]);
    menu_node_value_t *value = &(ctx->values[id]);

    uint8_t *idx = &(value->data.string_fixed.idx);
    uint8_t count = config->data.string_fixed.count;

    if (delta > 0)
        *idx = (*idx + delta) >= count ? 0 : (*idx + 1);
    else if (delta < 0)
        *idx = (*idx + delta) < 0 ? count - 1 : (*idx + delta);

    ctx->dirty = true;
}

/** @brief Функция для простейших числовых типов.
  * Изменение значение изменяется пропорционально выбранному значению factor
  * Если control == click, то navigation _только_ cycle.
  * @param id Идентификтор меню Определён, в enum в menu_struct.h
  * @param delta Количество "щелчков" энкодера. Передаётся только если control==position
  */
void menu_edit_udword_factor_click_cyclic_cb(menu_context_t *ctx, menu_id_t id) {
    const menu_node_config_t *config = &(ctx->configs[id]);
    menu_node_value_t *value = &(ctx->values[id]);

    uint32_t *val  = &(value->data.udword_factor.value);
    uint32_t  step = config->data.udword_factor.step;
    uint32_t   min = config->data.udword_factor.min;
    uint32_t   max = config->data.udword_factor.max;
    uint8_t *idx = &(value->data.udword_factor.idx);
    uint32_t  count = config->data.udword_factor.count;
    *idx = (*idx + 1) % count; // Кольцевое изменение индекса фактора по клику

    ctx->dirty = true;
}

/** @brief Функция для простейших числовых типов.
  * Изменение значение изменяется пропорционально выбранному значению factor
  * Если control == click, то navigation _только_ cycle.
  * @param id Идентификтор меню Определён, в enum в menu_struct.h
  * @param delta Количество "щелчков" энкодера. Передаётся только если control==position
  */
void menu_edit_udword_factor_position_limit_cb(menu_context_t *ctx, menu_id_t id, int8_t delta) {
    const menu_node_config_t *config = &(ctx->configs[id]);
    menu_node_value_t *value = &(ctx->values[id]);

    uint32_t *val  = &(value->data.udword_factor.value);
    uint32_t  step = config->data.udword_factor.step;
    uint32_t   min = config->data.udword_factor.min;
    uint32_t   max = config->data.udword_factor.max;
    uint8_t *idx = &(value->data.udword_factor.idx);
    uint32_t factor = config->data.udword_factor.factors[*idx];
    int res = *val + delta * factor;

    if (res < min)
      res = min;
    if (res > max)
      res = max;

    *val = (uint32_t) res;

    ctx->dirty = true;
}

/** @brief Функция для простейших численных типов.
  * Если control == click, то navigation _только_ cycle.
  * Значения зациклены. Достигнув max переходят в min, достигнув min переходят в max
  * @param id Идентификтор меню Определён, в enum в menu_struct.h
  * @param delta Количество "щелчков" энкодера. Передаётся только если control==position
  */
void menu_edit_ubyte_simple_click_cyclic_cb(menu_context_t *ctx, menu_id_t id) {
    const menu_node_config_t *config = &(ctx->configs[id]);
    menu_node_value_t *value = &(ctx->values[id]);

    uint8_t *val  = &(value->data.ubyte_simple.value);
    uint8_t  step = config->data.ubyte_simple.step;
    uint8_t   min = config->data.ubyte_simple.min;
    uint8_t   max = config->data.ubyte_simple.max;
    *val = (*val + step) < max ? (*val + step) : min; ///< Шагаем только вперёд. Зациклено

    ctx->dirty = true;
}

/** @brief Функция для простейших численных типов.
  * Если control == click, то navigation _только_ cycle.
  * Значения не зациклены. Достигнув max значение остаётся max, 
  * достигнув min значение останется min, пока не поменяем направление вращения энкодера
  * @param id Идентификтор меню Определён, в enum в menu_struct.h
  * @param delta Количество "щелчков" энкодера. Передаётся только если control==position
  */
void menu_edit_ubyte_simple_position_limit_cb(menu_context_t *ctx, menu_id_t id, int8_t delta) {
    const menu_node_config_t *config = &(ctx->configs[id]);
    menu_node_value_t *value = &(ctx->values[id]);

    uint8_t *val  = &(value->data.ubyte_simple.value);
    uint8_t  step = config->data.ubyte_simple.step;
    uint8_t   min = config->data.ubyte_simple.min;
    uint8_t   max = config->data.ubyte_simple.max;
    int res = *val + delta * step;
    if (res < min)
      res = min;
    if (res > max)
      res = max;

    *val = (uint8_t) res;

    ctx->dirty = true;
}


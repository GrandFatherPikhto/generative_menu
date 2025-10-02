#include "rotenc_callback.h"

#include <stdio.h>

void rotenc_hi_duty_changed(menu_context_t *ctx, menu_id_t id) {
    printf("%s:%d %s changed\n", __FILE__, __LINE__, menu_types_get_name(id));
}
#include <stdio.h>
#include <unistd.h>

#include "lcd1602.h"
#include "rotary_encoder.h"
#include "rotenc_menu.h"

int main(void)
{
    // menu_init();

    lcd1602_init();

     while(lcd1602_tick()) {
        if (renc_state(RENC_ACTION_CHANGE_POSITION)) 
        {
            // printf("%s:%d next\n", __FILE__, __LINE__);
            handle_change_position(renc_get_delta());
        }
        
        if (renc_state(RENC_ACTION_PUSH_BUTTON)) {
            // printf("%s:%d push button\n", __FILE__, __LINE__);
            handle_push_button();
        }

        if (renc_state(RENC_ACTION_LONG_PUSH_BUTTON)) {
            // printf("%s:%d long push button\n", __FILE__, __LINE__);
            handle_long_push_button();
        }

        if (!renc_state(RENC_ACTION_NONE)) {
            renc_reset_state();
        }

        if (menu_is_dirty()) {
            menu_draw();
        }

        usleep(100);
    }

    lcd1602_release();

    return 0;
}
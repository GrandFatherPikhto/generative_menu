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
        if (renc_state(RENC_ACTION_NEXT)) 
        {
            // menu_next(); 
            // menu_increment();
        }
        
        if (renc_state(RENC_ACTION_PREV)) { 
            // menu_prev(); 
            // menu_decrement();
        }
        
        // if (renc_state(RENC_ACTION_PUSH_BUTTON)) menu_enter();
        // if (renc_state(RENC_ACTION_LONG_PUSH_BUTTON)) menu_back();

        if (!renc_state(RENC_ACTION_NONE)) {
            renc_reset_state();
        }

        usleep(100);
    }

    lcd1602_release();

    return 0;
}
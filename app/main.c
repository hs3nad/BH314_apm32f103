#include "board_stm32f103rct8.h"
#include "app.h"

/* จุดเริ่มต้น firmware: init board, init app แล้ววน loop ตลอดเวลา */
int main(void)
{
    board_init();
    app_init();

    while (1) {
        app_loop();
    }
}

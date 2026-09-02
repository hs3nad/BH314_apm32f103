#include "app.h"
#include "bh314.h"

/* เริ่มต้น application หลัก โดยส่งต่อไป init ของ BH314 */
void app_init(void)
{
    bh314_init();
}

/* loop หลักของ application โดยส่งต่อไป task ของ BH314 */
void app_loop(void)
{
    bh314_task();
}

#include "synopsys_wei_delay.h"

void board_delay_cycle(uint32_t tick)
{
    uint32_t tick_end = 0;
    hx_drv_tick_start();
    while ((double)tick_end/400000.0 < tick)
    {
        hx_drv_tick_get(&tick_end);
    }
    return;
}

void hal_delay_ms(uint32_t tick)
{
    uint32_t tick_end = 0;
    hx_drv_tick_start();
    while ((double)tick_end/400000.0 < tick)
    {
        hx_drv_tick_get(&tick_end);
    }
    return;
}

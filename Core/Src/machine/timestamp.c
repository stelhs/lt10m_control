/*
 * timestamp.c
 */
#include "machine.h"
#include "stm32_lib/cmsis_thread.h"

static u32 _timestamp = 0;
static u32 _uptime = 0;

void timestamp_set(u32 new_timestamp)
{
    irq_disable();
    _timestamp = new_timestamp;
    irq_enable();
}

u32 now(void)
{
    u32 time;

    irq_disable();
    time = _timestamp;
    irq_enable();

    return time;
}

u32 uptime(void)
{
    u32 time;

    irq_disable();
    time = _uptime;
    irq_enable();

    return time;
}

int timestamp_to_hour(u32 timestamp, int gmt_offset)
{
    u32 seconds_in_day = timestamp % 86400;
    u32 hours = seconds_in_day / 3600;
    int hour_with_offset = hours + gmt_offset;

    if (hour_with_offset >= 24) {
        hour_with_offset -= 24;
    } else if (hour_with_offset < 0) {
        hour_with_offset += 24;
    }

    return hour_with_offset;
}

// IRQ context
void timestamp_tick(void)
{
    if (_timestamp > 0)
        _timestamp++;
    _uptime ++;
}

bool is_current_time_valid(void)
{
    return now() > 1723546958;
}


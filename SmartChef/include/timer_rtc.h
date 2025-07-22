#ifndef TIMER_RTC_H
#define TIMER_RTC_H

#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include <stdbool.h>

void timer_rtc_init(void);
void timer_rtc_start(uint16_t minutes);
bool timer_rtc_expired(void);

#endif

#include "timer_rtc.h"

static datetime_t start_time;
static uint16_t target_minutes = 0;
static bool counting = false;

void timer_rtc_init(void) {
    rtc_init();

    // Puedes poner una hora ficticia
    datetime_t t = {
        .year  = 2025,
        .month = 1,
        .day   = 1,
        .dotw  = 0,      // no importa
        .hour  = 0,
        .min   = 0,
        .sec   = 0
    };
    rtc_set_datetime(&t);
}

void timer_rtc_start(uint16_t minutes) {
    rtc_get_datetime(&start_time);
    target_minutes = minutes;
    counting = true;
}

bool timer_rtc_expired(void) {
    if (!counting) return false;

    datetime_t now;
    rtc_get_datetime(&now);

    int elapsed_minutes = (now.hour * 60 + now.min) - (start_time.hour * 60 + start_time.min);
    if (elapsed_minutes >= target_minutes) {
        counting = false; // para que no repita
        return true;
    }
    return false;
}

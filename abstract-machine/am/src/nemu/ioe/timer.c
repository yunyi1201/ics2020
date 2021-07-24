#include <am.h>
#include <nemu.h>

static AM_TIMER_UPTIME_T boot_time = {};

static uint64_t _get_time() {
	uint64_t usec = inl(RTC_ADDR);
	uint64_t sec  = inl(RTC_ADDR + 4);
	return sec * 1000000 + usec;
}
void __am_timer_init() {
	boot_time.us = _get_time();
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
	uptime->us = _get_time() - boot_time.us;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}

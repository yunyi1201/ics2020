#include <am.h>
#include <nemu.h>

static uint64_t last_usec;
void __am_timer_init() {
	last_usec = inl(RTC_ADDR) + ((uint64_t)inl(RTC_ADDR+4)) * 1000000;
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
	uint32_t usec = inl(RTC_ADDR);
	uint32_t sec  = inl(RTC_ADDR+4);
	uptime->us = usec + ((uint64_t)sec) * 1000000 - last_usec;
	last_usec = usec + ((uint64_t)sec) * 1000000;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}

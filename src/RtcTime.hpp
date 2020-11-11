#pragma once
#include <stdint.h>
#include <Arduino.h>

static const uint8_t kDaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
static const char kMonthNamesEnglish[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
static const char kDayNamesEngligh[] = "SatSunMonTueWedThuFri";
enum WeekInMonthOptions {Last, First, Second, Third, Fourth};
enum DayOfTheWeekOptions {Sun=1, Mon, Tue, Wed, Thu, Fri, Sat};
enum MonthNamesOptions {Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};
enum HemisphereOptions {North, South};
enum GetDateAndTimeOptions { DT_LOCAL, DT_UTC, DT_LOCALNOTZ, DT_DST, DT_STD, DT_RESTART, DT_ENERGY, DT_BOOTCOUNT, DT_LOCAL_MILLIS };
struct TIME_T;
struct RTC;
extern TIME_T rtcTime;
extern RTC Rtc;

typedef union {
  uint16_t data;
  struct {
    uint16_t hemis : 1;                    // bit 0        = 0=Northern, 1=Southern Hemisphere (=Opposite DST/STD)
    uint16_t week : 3;                     // bits 1 - 3   = 0=Last week of the month, 1=First, 2=Second, 3=Third, 4=Fourth
    uint16_t month : 4;                    // bits 4 - 7   = 1=Jan, 2=Feb, ... 12=Dec
    uint16_t dow : 3;                      // bits 8 - 10  = day of week, 1=Sun, 2=Mon, ... 7=Sat
    uint16_t hour : 5;                     // bits 11 - 15 = 0-23
  };
} TimeRule;

struct TIME_T {
  uint8_t       second;
  uint8_t       minute;
  uint8_t       hour;
  uint8_t       day_of_week;               // sunday is day 1
  uint8_t       day_of_month;
  uint8_t       month;
  char          name_of_month[4];
  uint16_t      day_of_year;
  uint16_t      year;
  unsigned long days;
  unsigned long valid;
};

struct RTC {
  uint32_t utc_time = 0;
  uint32_t local_time = 0;
  uint32_t daylight_saving_time = 0;
  uint32_t standard_time = 0;
  uint32_t ntp_time = 0;
  uint32_t midnight = 0;
  uint32_t restart_time = 0;
  uint32_t millis = 0;
  uint32_t last_sync = 0;
  int32_t time_timezone = 0;
  uint8_t ntp_sync_minute = 0;
  bool midnight_now = false;
  bool user_time_entry = false;               // Override NTP by user setting
};

void RtcInit(void);
void BreakTime(uint32_t time_input, TIME_T &tm);
uint32_t UtcTime(void);
uint32_t LocalTime(void);
uint32_t Midnight(void);
bool MidnightNow(void);
bool IsDst(void);
String GetBuildDateAndTime(void);
String GetMinuteTime(uint32_t minutes);
String GetTimeZone(void);
String GetDuration(uint32_t time);
String GetDT(uint32_t time);
String GetDateAndTime(uint8_t time_type);
uint32_t UpTime(void);
uint32_t MinutesUptime(void);
String GetUptime(void);
uint32_t MinutesPastMidnight(void);
uint32_t RtcMillis(void);
uint32_t MakeTime(TIME_T &tm);
uint32_t RuleToTime(TimeRule r, int yr);
void RtcSetTime(uint32_t epoch);
void RtcInit(void);
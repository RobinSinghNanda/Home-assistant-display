#include "RtcTime.hpp"
#include "Arduino.h"
#include "Settings.hpp"
#include "GlobalParams.hpp"
#include "Log.hpp"
#include "i18n.h"

TIME_T rtcTime;

const uint32_t SECS_PER_MIN = 60UL;
const uint32_t SECS_PER_HOUR = 3600UL;
const uint32_t SECS_PER_DAY = SECS_PER_HOUR * 24UL;
const uint32_t MINS_PER_HOUR = 60UL;
static bool bNetIsTimeSync = false;
#define LEAP_YEAR(Y)  (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))
#include <time.h>
#include <Ticker.h>

Ticker TickerRtc;
uint32_t RtcMillis(void);

RTC Rtc;

uint32_t SntpGetCurrentTimestamp(void) {
  time_t now = 0;
  if (bNetIsTimeSync || globalParams.getNtpForceSync()) {
    //Serial_DebugX(("timesync configTime %d\n", TasmotaGlobal.ntp_force_sync, bNetIsTimeSync));
    // init to UTC Time
    configTime(0, 0, settings.getNtpServer1(), settings.getNtpServer2(), settings.getNtpServer3());
    bNetIsTimeSync = false;
    globalParams.setNtpForceSync(false);
  }
  time(&now);
  return now;
}

uint32_t UtcTime(void) {
  return Rtc.utc_time;
}

uint32_t LocalTime(void) {
  return Rtc.local_time;
}

uint32_t Midnight(void) {
  return Rtc.midnight;
}

bool MidnightNow(void) {
  if (Rtc.midnight_now) {
    Rtc.midnight_now = false;
    return true;
  }
  return false;
}

bool IsDst(void) {
  if (Rtc.time_timezone == settings.getTimezoneOffset2()) {
    return true;
  }
  return false;
}

String GetBuildDateAndTime(void) {
  // "2017-03-07T11:08:02" - ISO8601:2004
  char bdt[21];
  char *p;
  char mdate[] = __DATE__;  // "Mar  7 2017"
  char *smonth = mdate;
  int day = 0;
  int year = 0;

  // sscanf(mdate, "%s %d %d", bdt, &day, &year);  // Not implemented in 2.3.0 and probably too much code
  uint8_t i = 0;
  for (char *str = strtok_r(mdate, " ", &p); str && i < 3; str = strtok_r(nullptr, " ", &p)) {
    switch (i++) {
    case 0:  // Month
      smonth = str;
      break;
    case 1:  // Day
      day = atoi(str);
      break;
    case 2:  // Year
      year = atoi(str);
    }
  }
  int month = (strstr(kMonthNamesEnglish, smonth) -kMonthNamesEnglish) /3 +1;
  snprintf_P(bdt, sizeof(bdt),"%d" D_YEAR_MONTH_SEPARATOR "%02d" D_MONTH_DAY_SEPARATOR "%02d" D_DATE_TIME_SEPARATOR "%s", year, month, day, __TIME__);
  return String(bdt);  // 2017-03-07T11:08:02
}

String GetMinuteTime(uint32_t minutes)
{
  char tm[6];
  snprintf_P(tm, sizeof(tm), PSTR("%02d:%02d"), minutes / 60, minutes % 60);

  return String(tm);  // 03:45
}

String GetTimeZone(void)
{
  char tz[7];
  snprintf_P(tz, sizeof(tz), PSTR("%+03d:%02d"), Rtc.time_timezone / 60, abs(Rtc.time_timezone % 60));

  return String(tz);  // -03:45
}

String GetDuration(uint32_t time)
{
  char dt[16];

  TIME_T ut;
  BreakTime(time, ut);

  // "P128DT14H35M44S" - ISO8601:2004 - https://en.wikipedia.org/wiki/ISO_8601 Durations
//  snprintf_P(dt, sizeof(dt), PSTR("P%dDT%02dH%02dM%02dS"), ut.days, ut.hour, ut.minute, ut.second);

  // "128 14:35:44" - OpenVMS
  // "128T14:35:44" - Tasmota
  snprintf_P(dt, sizeof(dt), PSTR("%uT%02u:%02u:%02u"), (uint8_t)ut.days, (uint8_t)ut.hour, (uint8_t)ut.minute, (uint8_t)ut.second);

  return String(dt);  // 128T14:35:44
}

String GetDT(uint32_t time)
{
  // "2017-03-07T11:08:02" - ISO8601:2004

  char dt[20];
  TIME_T tmpTime;

  BreakTime(time, tmpTime);
  snprintf_P(dt, sizeof(dt), PSTR("%04d-%02d-%02dT%02d:%02d:%02d"),
    tmpTime.year +1970, tmpTime.month, tmpTime.day_of_month, tmpTime.hour, tmpTime.minute, tmpTime.second);

  return String(dt);  // 2017-03-07T11:08:02
}

/*
 * timestamps in https://en.wikipedia.org/wiki/ISO_8601 format
 *
 *  DT_UTC     - current data and time in Greenwich, England (aka GMT)
 *  DT_LOCAL   - current date and time taking timezone into account
 *  DT_RESTART - the date and time this device last started, in local timezone
 *
 * Format:
 *  "2017-03-07T11:08:02-07:00" - if DT_LOCAL and SetOption52 = 1
 *  "2017-03-07T11:08:02"       - otherwise
 */
String GetDateAndTime(uint8_t time_type)
{
  // "2017-03-07T11:08:02-07:00" - ISO8601:2004
  uint32_t time = Rtc.local_time;

  switch (time_type) {
    case DT_UTC:
      time = Rtc.utc_time;
      break;
//    case DT_LOCALNOTZ:  // Is default anyway but allows for not appendig timezone
//      time = Rtc.local_time;
//      break;
    case DT_DST:
      time = Rtc.daylight_saving_time;
      break;
    case DT_STD:
      time = Rtc.standard_time;
      break;
    case DT_RESTART:
      if (Rtc.restart_time == 0) {
        return "";
      }
      time = Rtc.restart_time;
      break;
    // case DT_BOOTCOUNT:
    //   time = Settings.bootcount_reset_time;
    //   break;
  }
  String dt = GetDT(time);  // 2017-03-07T11:08:02

  if (DT_LOCAL_MILLIS == time_type) {
    char ms[10];
    snprintf_P(ms, sizeof(ms), PSTR(".%03d"), RtcMillis());
    dt += ms;
    time_type = DT_LOCAL;
  }

//   if (Settings.flag3.time_append_timezone && (DT_LOCAL == time_type)) {  // SetOption52 - Append timezone to JSON time
//     dt += GetTimeZone();    // 2017-03-07T11:08:02-07:00
//   }
  return dt;  // 2017-03-07T11:08:02-07:00
}

uint32_t UpTime(void) {
  if (Rtc.restart_time) {
    return Rtc.utc_time - Rtc.restart_time;
  } else {
    return globalParams.getUptime();
  }
}

uint32_t MinutesUptime(void) {
  return (UpTime() / 60);
}

String GetUptime(void) {
  return GetDuration(UpTime());
}

uint32_t MinutesPastMidnight(void) {
  uint32_t minutes = 0;

  if (rtcTime.valid) {
    minutes = (rtcTime.hour *60) + rtcTime.minute;
  }
  return minutes;
}

uint32_t RtcMillis(void) {
  return (millis() - Rtc.millis) % 1000;
}

void BreakTime(uint32_t time_input, TIME_T &tm) {
// break the given time_input into time components
// this is a more compact version of the C library localtime function
// note that year is offset from 1970 !!!

  uint8_t year;
  uint8_t month;
  uint8_t month_length;
  uint32_t time;
  unsigned long days;

  time = time_input;
  tm.second = time % 60;
  time /= 60;                // now it is minutes
  tm.minute = time % 60;
  time /= 60;                // now it is hours
  tm.hour = time % 24;
  time /= 24;                // now it is days
  tm.days = time;
  tm.day_of_week = ((time + 4) % 7) + 1;  // Sunday is day 1

  year = 0;
  days = 0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  tm.year = year;            // year is offset from 1970

  days -= LEAP_YEAR(year) ? 366 : 365;
  time -= days;              // now it is days in this year, starting at 0
  tm.day_of_year = time;

  for (month = 0; month < 12; month++) {
    if (1 == month) { // february
      if (LEAP_YEAR(year)) {
        month_length = 29;
      } else {
        month_length = 28;
      }
    } else {
      month_length = kDaysInMonth[month];
    }

    if (time >= month_length) {
      time -= month_length;
    } else {
      break;
    }
  }
  strlcpy(tm.name_of_month, kMonthNames + (month *3), 4);
  tm.month = month + 1;      // jan is month 1
  tm.day_of_month = time + 1;         // day of month
  tm.valid = (time_input > START_VALID_TIME);  // 2016-01-01
}

uint32_t MakeTime(TIME_T &tm)
{
// assemble time elements into time_t
// note year argument is offset from 1970

  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds = tm.year * (SECS_PER_DAY * 365);
  for (i = 0; i < tm.year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }

  // add days for this year, months start from 1
  for (i = 1; i < tm.month; i++) {
    if ((2 == i) && LEAP_YEAR(tm.year)) {
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * kDaysInMonth[i-1];  // monthDay array starts from 0
    }
  }
  seconds+= (tm.day_of_month - 1) * SECS_PER_DAY;
  seconds+= tm.hour * SECS_PER_HOUR;
  seconds+= tm.minute * SECS_PER_MIN;
  seconds+= tm.second;
  return seconds;
}

uint32_t RuleToTime(TimeRule r, int yr)
{
  TIME_T tm;
  uint32_t t;
  uint8_t m;
  uint8_t w;                // temp copies of r.month and r.week

  m = r.month;
  w = r.week;
  if (0 == w) {             // Last week = 0
    if (++m > 12) {         // for "Last", go to the next month
      m = 1;
      yr++;
    }
    w = 1;                  // and treat as first week of next month, subtract 7 days later
  }

  tm.hour = r.hour;
  tm.minute = 0;
  tm.second = 0;
  tm.day_of_month = 1;
  tm.month = m;
  tm.year = yr - 1970;
  t = MakeTime(tm);         // First day of the month, or first day of next month for "Last" rules
  BreakTime(t, tm);
  t += (7 * (w - 1) + (r.dow - tm.day_of_week + 7) % 7) * SECS_PER_DAY;
  if (0 == r.week) {
    t -= 7 * SECS_PER_DAY;  // back up a week if this is a "Last" rule
  }
  return t;
}

void RtcSecond(void)
{
  TIME_T tmpTime;

  Rtc.millis = millis();

  if (!Rtc.user_time_entry) {
    if (globalParams.isWifiConnected()) {
      uint8_t uptime_minute = (globalParams.getUptime() / 60) % 60;  // 0 .. 59
      if ((Rtc.ntp_sync_minute > 59) && (uptime_minute > 2)) {
        Rtc.ntp_sync_minute = 1;                   // If sync prepare for a new cycle
      }
      uint8_t offset = (globalParams.getUptime() < 30) ? rtcTime.second : (((((uint32_t)ESP.getEfuseMac()) & 0xF) * 3) + 3) ;  // First try ASAP to sync. If fails try once every 60 seconds based on chip id
      if ( (((offset == rtcTime.second) && ( (rtcTime.year < 2016) ||                          // Never synced
                                            (Rtc.ntp_sync_minute == uptime_minute))) ||       // Re-sync every hour
                                              globalParams.getNtpForceSync() ) ) {                             // Forced sync
        Rtc.ntp_time = SntpGetCurrentTimestamp();
        if (Rtc.ntp_time > START_VALID_TIME) {  // Fix NTP bug in core 2.4.1/SDK 2.2.1 (returns Thu Jan 01 08:00:10 1970 after power on)
          globalParams.setNtpForceSync(false);
          Rtc.utc_time = Rtc.ntp_time;
          Rtc.last_sync = Rtc.ntp_time;
          Rtc.ntp_sync_minute = 60;  // Sync so block further requests
          if (Rtc.restart_time == 0) {
            Rtc.restart_time = Rtc.utc_time - globalParams.getUptime();  // save first ntp time as restart time
          }
          BreakTime(Rtc.utc_time, tmpTime);
          rtcTime.year = tmpTime.year + 1970;
          Rtc.daylight_saving_time = RuleToTime(settings.getTimeRuleDST(), rtcTime.year);
          Rtc.standard_time = RuleToTime(settings.getTimeRuleSTD(), rtcTime.year);

          // Do not use AddLog_P2 here (interrupt routine) if syslog or mqttlog is enabled. UDP/TCP will force exception 9
          Log::addLog(LOG_LEVEL_DEBUG, PSTR("NTP: " D_UTC_TIME " %s, " D_DST_TIME " %s, " D_STD_TIME " %s"),
            GetDateAndTime(DT_UTC).c_str(), GetDateAndTime(DT_DST).c_str(), GetDateAndTime(DT_STD).c_str());

          if (Rtc.local_time < START_VALID_TIME) {  // 2016-01-01
            globalParams.setTimeInit(1);
          } else {
            globalParams.setTimeSet(1);
          }
        } else {
          Rtc.ntp_sync_minute++;  // Try again in next minute
        }
      }
    }
    if ((Rtc.utc_time > (2 * 60 * 60)) && (Rtc.last_sync < Rtc.utc_time - (2 * 60 * 60))) {  // Every two hours a warning
      // Do not use AddLog_P2 here (interrupt routine) if syslog or mqttlog is enabled. UDP/TCP will force exception 9
      Log::addLog(LOG_LEVEL_DEBUG, PSTR("NTP: Not synced"));
      Rtc.last_sync = Rtc.utc_time;
    }
  }

  Rtc.utc_time++;  // Increment every second
  Rtc.local_time = Rtc.utc_time;
  if (Rtc.local_time > START_VALID_TIME) {  // 2016-01-01
    int16_t timezone_minutes = settings.getTimezoneMinutes();
    if (settings.getTimezone() < 0) { timezone_minutes *= -1; }
    Rtc.time_timezone = (settings.getTimezone() * SECS_PER_HOUR) + (timezone_minutes * SECS_PER_MIN);
    if (99 == settings.getTimezone()) {
      int32_t dstoffset = settings.getTimezoneOffset2() * SECS_PER_MIN;
      int32_t stdoffset = settings.getTimezoneOffset1() * SECS_PER_MIN;
      if (settings.getTimeRuleDST().hemis) {
        // Southern hemisphere
        if ((Rtc.utc_time >= (Rtc.standard_time - dstoffset)) && (Rtc.utc_time < (Rtc.daylight_saving_time - stdoffset))) {
          Rtc.time_timezone = stdoffset;  // Standard Time
        } else {
          Rtc.time_timezone = dstoffset;  // Daylight Saving Time
        }
      } else {
        // Northern hemisphere
        if ((Rtc.utc_time >= (Rtc.daylight_saving_time - stdoffset)) && (Rtc.utc_time < (Rtc.standard_time - dstoffset))) {
          Rtc.time_timezone = dstoffset;  // Daylight Saving Time
        } else {
          Rtc.time_timezone = stdoffset;  // Standard Time
        }
      }
    }
    Rtc.local_time += Rtc.time_timezone;
    Rtc.time_timezone /= 60;
    // if (Settings.bootcount_reset_time < START_VALID_TIME) {
    //   Settings.bootcount_reset_time = Rtc.local_time;
    // }
  }

  BreakTime(Rtc.local_time, rtcTime);
  if (rtcTime.valid) {
    if (!Rtc.midnight) {
      Rtc.midnight = Rtc.local_time - (rtcTime.hour * 3600) - (rtcTime.minute * 60) - rtcTime.second;
    }
    if (!rtcTime.hour && !rtcTime.minute && !rtcTime.second) {
      Rtc.midnight = Rtc.local_time;
      Rtc.midnight_now = true;
    }
  }

  rtcTime.year += 1970;
}

void RtcSetTime(uint32_t epoch)
{
  if (epoch < START_VALID_TIME) {  // 2016-01-01
    Rtc.user_time_entry = false;
    globalParams.setNtpForceSync(true);
    bNetIsTimeSync = true;
  } else {
    Rtc.user_time_entry = true;
    Rtc.utc_time = epoch -1;    // Will be corrected by RtcSecond
  }
}

void RtcInit(void) {
  bNetIsTimeSync = true;
  Rtc.utc_time = 0;
  BreakTime(Rtc.utc_time, rtcTime);
  TickerRtc.attach(1, RtcSecond);
}

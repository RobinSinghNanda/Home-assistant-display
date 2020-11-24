#include "Support.hpp"
#include "GlobalParams.hpp"
#include <string.h>
#include <stdlib.h>
#include <Arduino.h>
#include "i18n.h"
#include "WifiManager.hpp"
extern WifiManager wifiManger;

bool Support::parseIp(uint32_t* addr, const char* str) {
  uint8_t *part = (uint8_t*)addr;
  uint8_t i;

  *addr = 0;
  for (i = 0; i < 4; i++) {
    part[i] = strtoul(str, nullptr, 10);        // Convert byte
    str = strchr(str, '.');
    if (str == nullptr || *str == '\0') {
      break;  // No more separators, exit
    }
    str++;                                   // Point to next character after separator
  }
  return (3 == i);
}

// see https://stackoverflow.com/questions/6357031/how-do-you-convert-a-byte-array-to-a-hexadecimal-string-in-c
// char* ToHex_P(unsigned char * in, size_t insz, char * out, size_t outsz, char inbetween = '\0'); in tasmota_globals.h
char* Support::toHex(const unsigned char * in, size_t insz, char * out, size_t outsz, char inbetween) {
  // ToHex_P(in, insz, out, outz)      -> "12345667"
  // ToHex_P(in, insz, out, outz, ' ') -> "12 34 56 67"
  // ToHex_P(in, insz, out, outz, ':') -> "12:34:56:67"
  static const char * hex = "0123456789ABCDEF";
  int between = (inbetween) ? 3 : 2;
  const unsigned char * pin = in;
  char * pout = out;
  for (; pin < in+insz; pout += between, pin++) {
    pout[0] = hex[(pgm_read_byte(pin)>>4) & 0xF];
    pout[1] = hex[ pgm_read_byte(pin)     & 0xF];
    if (inbetween) { pout[2] = inbetween; }
    if (pout + 3 - out > outsz) { break; }  // Better to truncate output string than overflow buffer
  }
  pout[(inbetween && insz) ? -1 : 0] = 0;   // Discard last inbetween if any input
  return out;
}

String Support::getDuration(uint32_t time) {
  char dt[16];

  TIME_T ut;
  breakTime(time, ut);

  // "P128DT14H35M44S" - ISO8601:2004 - https://en.wikipedia.org/wiki/ISO_8601 Durations
//  snprintf_P(dt, sizeof(dt), PSTR("P%dDT%02dH%02dM%02dS"), ut.days, ut.hour, ut.minute, ut.second);

  // "128 14:35:44" - OpenVMS
  // "128T14:35:44" - Tasmota
  snprintf_P(dt, sizeof(dt), PSTR("%uT%02u:%02u:%02u"), (uint8_t)ut.days, (uint8_t)ut.hour, (uint8_t)ut.minute, (uint8_t)ut.second);

  return String(dt);  // 128T14:35:44
}

void Support::breakTime(uint32_t time_input, TIME_T &tm) {
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

uint32_t Support::getUpTime(void) {
  // if (Rtc.restart_time) {
  //   return Rtc.utc_time - Rtc.restart_time;
  // } else {
    return globalParams.getUptime();
  // }
}

bool Support::timeReached(uint32_t timer)
{
  // Check if a certain timeout has been reached.
  const long passed = timePassedSince(timer);
  return (passed >= 0);
}

int32_t Support::timePassedSince(uint32_t timestamp) {
  // Compute the number of milliSeconds passed since timestamp given.
  // Note: value can be negative if the timestamp has not yet been reached.
  return timeDifference(timestamp, millis());
}

void Support::setNextTimeInterval(uint32_t& timer, const uint32_t step) {
  timer += step;
  const long passed = timePassedSince(timer);
  if (passed < 0) { return; }   // Event has not yet happened, which is fine.
  if (static_cast<unsigned long>(passed) > step) {
    // No need to keep running behind, start again.
    timer = millis() + step;
    return;
  }
  // Try to get in sync again.
  timer = millis() + (step - passed);
}

void Support::EspRestart(void) {
  wifiManager.shutdown(true);
  //CrashDumpClear();           // Clear the stack dump in RTC

  if (globalParams.getRestartHalt()) {
    while (1) {
      //Blink led
      //SetLedLink(1);          // Wifi led on
      delay(200);             // Satisfy SDK
      //SetLedLink(0);          // Wifi led off
      delay(800);             // Satisfy SDK
    }
  } else {
      ESP.restart();
  }
}

char* Support::getTextIndexed(char* destination, size_t destination_size, uint32_t index, const char* haystack) {
  // Returns empty string if not found
  // Returns text of found
  char* write = destination;
  const char* read = haystack;

  index++;
  while (index--) {
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {
      ch = pgm_read_byte(read++);
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    if (0 == ch) {
      if (index) {
        write = destination;
      }
      break;
    }
  }
  *write = '\0';
  return destination;
}

char* Support::dtostrfd(double number, unsigned char prec, char *s) {
  if ((isnan(number)) || (isinf(number))) {  // Fix for JSON output (https://stackoverflow.com/questions/1423081/json-left-out-infinity-and-nan-json-status-in-ecmascript)
    strcpy(s, "null");
    return s;
  } else {
    return dtostrf(number, 1, prec, s);
  }
}

char* Support::noAlNumToUnderscore(char* dest, const char* source) {
  char* write = dest;
  const char* read = source;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    *write++ = (isalnum(ch) || ('\0' == ch)) ? ch : '_';
  }
  return dest;
}

String Support::getBuildDateAndTime(void) {
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
  snprintf_P(bdt, sizeof(bdt), PSTR("%d" D_YEAR_MONTH_SEPARATOR "%02d" D_MONTH_DAY_SEPARATOR "%02d" D_DATE_TIME_SEPARATOR "%s"), year, month, day, __TIME__);
  return String(bdt);  // 2017-03-07T11:08:02
}

uint32_t Support::getSketchSize(void) {
  static uint32_t sketchsize = 0;

  if (!sketchsize) {
    sketchsize = ESP.getSketchSize();  // This takes almost 2 seconds on an ESP32
  }
  return sketchsize;
}

size_t Support::strchrspn(const char *str1, int character) {
  size_t ret = 0;
  char *start = (char*)str1;
  char *end = strchr(str1, character);
  if (end) ret = end - start;
  return ret;
}

void Support::bufferFree(uint8_t * &buffer) {
  if (buffer != nullptr) {
    free(buffer);
    buffer = nullptr;
  }
}

bool Support::bufferAlloc(uint8_t * &buffer) {
  bufferFree(buffer);
  if (!(buffer = (uint8_t *)malloc(sizeof(Settings)))) {
    return false;
  }
  return true;
}

void Support::snprintfloat (char *s, size_t length, float num, int n) {
    snprintf (s, length, "%.20g",num);
    char *p;
    int count;

    p = strchr (s,'.');         // Find decimal point, if any.
    if (p != NULL) {
        count = n;              // Adjust for more or less decimals.
        while (count >= 0) {    // Maximum decimals allowed.
             count--;
             if (*p == '\0')    // If there's less than desired.
                 break;
             p++;               // Next character.
        }

        *p-- = '\0';            // Truncate string.
        while (*p == '0')       // Remove trailing zeros.
            *p-- = '\0';

        if (*p == '.') {        // If all decimals were zeros, remove ".".
            *p = '\0';
        }
    }
}
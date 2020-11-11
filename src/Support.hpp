#ifndef SUPPORT_HPP_
#define SUPPORT_HPP_
#include <stdint.h>
#include <Arduino.h>
#include "RtcTime.hpp"

#define LEAP_YEAR(Y)  (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))

const uint32_t START_VALID_TIME = 1451602800;  // Time is synced and after 2016-01-01

class Support {
    public:
        static bool parseIp(uint32_t* addr, const char* str);
        static char* toHex(const unsigned char * in, size_t insz, char * out, size_t outsz, char inbetween);
        static String getDuration(uint32_t time);
        static void breakTime(uint32_t time_input, TIME_T &tm);
        static uint32_t getUpTime(void);
        static inline int32_t timeDifference(uint32_t prev, uint32_t next);
        static int32_t timePassedSince(uint32_t timestamp);
        static bool timeReached(uint32_t timer);
        static void setNextTimeInterval(uint32_t& timer, const uint32_t step);
        static char* getTextIndexed(char* destination, size_t destination_size, uint32_t index, const char* haystack);
        static char* dtostrfd(double number, unsigned char prec, char *s);
        static char* noAlNumToUnderscore(char* dest, const char* source);
        static String getBuildDateAndTime(void);
        static uint32_t getSketchSize(void);
        static void EspRestart(void);
        static void rtcSecond(void);
        static size_t strchrspn(const char *str1, int character);
        static void bufferFree(uint8_t * &buffer);
        static bool bufferAlloc(uint8_t * &buffer);
};


int32_t Support::timeDifference(uint32_t prev, uint32_t next) {
  return ((int32_t) (next - prev));
}

#endif
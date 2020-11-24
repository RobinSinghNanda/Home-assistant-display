#ifndef __SERVICEDATA_H__
#define __SERVICEDATA_H__

#include <stdint.h>
#include <string>
#include <vector>
#include <string.h>
#include <stdio.h> 
using namespace std;

typedef char * Buffer;
class ServiceData {
    public:
        ServiceData();
        static constexpr int BUFFER_SIZE = 400;
        inline void add(const char * key, bool value);
        inline void add(const char * key, const char * value);
        inline void add(const char * key, string value);
        inline void add(const char * key, float value);
        inline void add(const char * key, int32_t value);
        inline void add(const char * key, uint32_t value);
        inline void add(const char * key, vector<string> value);
        inline void add(const char * key, vector<uint32_t> value);
        inline void add(const char * key, uint32_t * arr, size_t length);
        inline void add(const char * key, uint8_t * arr, size_t length);
        inline void add(const char * key, string * arr, size_t length);
        inline void clear();
        operator char*() const;
    protected:
        char buffer[BUFFER_SIZE];
        char seperator[2];
        static constexpr const char * S_TRUE = "true";
        static constexpr const char * S_FALSE = "false";
};

extern ServiceData * serviceData;

void ServiceData::add(const char * key, bool value) {
    snprintf(buffer, BUFFER_SIZE,"%s%s\"%s\":%s", buffer, seperator, key, value?S_TRUE:S_FALSE);
    seperator[0] = ',';
}

void ServiceData::add(const char * key, string value) {
    add(key, value.c_str());
}

void ServiceData::add(const char * key, const char * value) {
    snprintf(buffer, BUFFER_SIZE,"%s%s\"%s\":\"%s\"", buffer, seperator, key, value);
    seperator[0] = ',';
}

void ServiceData::add(const char * key, float value) {
    snprintf(buffer, BUFFER_SIZE,"%s%s\"%s\":%f", buffer, seperator, key, value);
    seperator[0] = ',';
}

void ServiceData::add(const char * key, int32_t value) {
    snprintf(buffer, BUFFER_SIZE,"%s%s\"%s\":%d", buffer, seperator, key, value);
    seperator[0] = ',';
}

void ServiceData::add(const char * key, uint32_t value) {
    snprintf(buffer, BUFFER_SIZE,"%s%s\"%s\":%d", buffer, seperator, key, value);
    seperator[0] = ',';
}

void ServiceData::add(const char * key, vector<string> value) {
    snprintf(buffer, BUFFER_SIZE,"%s%s\"%s\":[", buffer, seperator, key);
    bool first = true;
    for (string str: value) {
        snprintf(buffer, BUFFER_SIZE,"%s%s\"%s\"", buffer, (first)?"":",", str.c_str());
        first = false;
    }
    snprintf(buffer, BUFFER_SIZE,"%s]", buffer);
    seperator[0] = ',';
}

void ServiceData::add(const char * key, vector<uint32_t> value) {
    snprintf(buffer, BUFFER_SIZE,"%s%s\"%s\":[", buffer, seperator, key);
    bool first = true;
    for (uint32_t val: value) {
        snprintf(buffer, BUFFER_SIZE,"%s%s%d", buffer, (first)?"":",", val);
        first = false;
    }
    snprintf(buffer, BUFFER_SIZE,"%s]", buffer);
    seperator[0] = ',';
}

void ServiceData::add(const char * key, uint32_t * arr, size_t length) {
    snprintf(buffer, BUFFER_SIZE,"%s%s\"%s\":[", buffer, seperator, key);
    bool first = true;
    for (uint8_t index = 0;index<length;index++) {
        snprintf(buffer, BUFFER_SIZE,"%s%s%d", buffer, (first)?"":",", arr[index]);
        first = false;
    }
    snprintf(buffer, BUFFER_SIZE,"%s]", buffer);
    seperator[0] = ',';
}

void ServiceData::add(const char * key, uint8_t * arr, size_t length) {
    snprintf(buffer, BUFFER_SIZE,"%s%s\"%s\":[", buffer, seperator, key);
    bool first = true;
    for (uint8_t index = 0;index<length;index++) {
        snprintf(buffer, BUFFER_SIZE,"%s%s%d", buffer, (first)?"":",", arr[index]);
        first = false;
    }
    snprintf(buffer, BUFFER_SIZE,"%s]", buffer);
    seperator[0] = ',';
}

void ServiceData::add(const char * key, string * arr, size_t length) {
    snprintf(buffer, BUFFER_SIZE,"%s%s\"%s\":[", buffer, seperator, key);
    bool first = true;
    for (uint8_t index = 0;index<length;index++) {
        snprintf(buffer, BUFFER_SIZE,"%s%s%s", buffer, (first)?"":",", arr[index].c_str());
        first = false;
    }
    snprintf(buffer, BUFFER_SIZE,"%s]", buffer);
    seperator[0] = ',';
}

void ServiceData::clear() {
    seperator[0] = '\0';
    buffer[0] ='\0';
}

#endif // __SERVICEDATA_H__
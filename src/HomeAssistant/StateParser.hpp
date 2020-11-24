#ifndef __STATEPARSER_H__
#define __STATEPARSER_H__

#include <stdint.h>
#include <string>
#include <vector>
#include <ArduinoJson.h>
#include "RtcTime.hpp"

using namespace std;

class StateParser {
    public:
        static inline void parse(JsonObject object, const char * key, float &value);
        static inline void parse(JsonObject object, const char * key, uint32_t &value);
        static inline void parse(JsonObject object, const char * key, uint16_t &value);
        static inline void parse(JsonObject object, const char * key, uint8_t &value);
        static inline void parse(JsonObject object, const char * key, int32_t &value);
        static inline void parse(JsonObject object, const char * key, string &value);
        static inline void parse(JsonObject object, const char * key, bool &value);
        static inline void parse(JsonObject object, const char * key, vector<string> &value);
        static inline void parseColor(JsonObject object, const char * key, uint32_t &color);
        static inline void parseTime(JsonObject object, const char * key, uint32_t &time);
};

void StateParser::parse(JsonObject object, const char * key, float &value) {
    if (object.containsKey(key)) {
        value = object[key];
    }
}

void StateParser::parse(JsonObject object, const char * key, uint32_t &value) {
    if (object.containsKey(key)) {
        value = object[key];
    }
}

void StateParser::parse(JsonObject object, const char * key, uint16_t &value) {
    if (object.containsKey(key)) {
        value = object[key];
    }
}

void StateParser::parse(JsonObject object, const char * key, uint8_t &value) {
    if (object.containsKey(key)) {
        value = object[key];
    }
}

void StateParser::parse(JsonObject object, const char * key, int32_t &value) {
    if (object.containsKey(key)) {
        value = object[key];
    }
}

void StateParser::parse(JsonObject object, const char * key, string &value) {
    if (object.containsKey(key)) {
        value = object[key].as<string>();
    }
}

void StateParser::parse(JsonObject object, const char * key, bool &value) {
    if (object.containsKey(key)) {
        value = object[key];
    }
}

void StateParser::parse(JsonObject object, const char * key, vector<string> &value) {
    if (object.containsKey(key)) {
        if (object[key].is<JsonArray>()) {
            value.clear();
            for (string str: object[key].as<JsonArray>()) {
                value.push_back(str);
            }
        }
    }
}

void StateParser::parseColor(JsonObject object, const char * key, uint32_t &color) {
    if (object.containsKey(key)) {
        JsonVariant colorVariant = object[key];
        if (colorVariant.is<JsonArray>()) {
            JsonArray colorArray = colorVariant.as<JsonArray>();
            if (colorArray == 3) {
                color = 0;
                color |= (colorArray[0].as<uint8_t>())&0xFF;
                color |= ((colorArray[1].as<uint8_t>())&0xFF)<<8;
                color |= ((colorArray[2].as<uint8_t>())&0xFF)<<16;
            }
        }
    }
}

void StateParser::parseTime(JsonObject object, const char * key, uint32_t &time) {
    if (object.containsKey(key)) {
        string datetime;
        datetime = object[key].as<string>();
        time = MakeTime(datetime.c_str());
    }
}

#endif // __STATEPARSER_H__
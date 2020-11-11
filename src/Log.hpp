#ifndef LOG_HPP_
#define LOG_HPP_
#include "language/en_GB.h"
#include "Config.hpp"
#include <Arduino.h>

#define D_LOG_APPLICATION "APP: "  // Application
#define D_LOG_BRIDGE "BRG: "       // Bridge
#define D_LOG_CONFIG "CFG: "       // Settings
#define D_LOG_COMMAND "CMD: "      // Command
#define D_LOG_DEBUG "DBG: "        // Debug
#define D_LOG_DHT "DHT: "          // DHT sensor
#define D_LOG_DOMOTICZ "DOM: "     // Domoticz
#define D_LOG_DSB "DSB: "          // DS18xB20 sensor
#define D_LOG_HTTP "HTP: "         // HTTP webserver
#define D_LOG_HRE "HRE: "
#define D_LOG_I2C "I2C: "          // I2C
#define D_LOG_IRR "IRR: "          // Infra Red Received
#define D_LOG_KNX "KNX: "
#define D_LOG_LOG "LOG: "          // Logging
#define D_LOG_MODULE "MOD: "       // Module
#define D_LOG_MDNS "DNS: "         // mDNS
#define D_LOG_MQTT "MQT: "         // MQTT
#define D_LOG_OTHER "OTH: "        // Other
#define D_LOG_RESULT "RSL: "       // Result
#define D_LOG_RFR "RFR: "          // RF Received
#define D_LOG_SERIAL "SER: "       // Serial
#define D_LOG_SHT1 "SHT: "         // SHT1x sensor
#define D_LOG_UPLOAD "UPL: "       // Upload
#define D_LOG_UPNP "UPP: "         // UPnP
#define D_LOG_WIFI "WIF: "         // Wifi
#define D_LOG_ZIGBEE "ZIG: "       // Zigbee
#define D_LOG_TCP "TCP: "          // TCP bridge
#define D_LOG_HA  "HA: "           // Home Assistant
#define D_LOG_LCD "LCD: "          // LCD

enum LogLevel {
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_DEBUG_MORE
};


class Log {
    public:
        Log();
        ~Log();
        static void log(LogLevel loglevel, const char * formatP, ...);
        static void addLog(LogLevel loglevel, PGM_P formatP, ...);
        #ifdef USE_WEBSERVER
            static void getWebLog(uint32_t idx, char** entry_pp, size_t* len_p);
        #endif
    protected:
        static void log(LogLevel loglevel);
};

#endif
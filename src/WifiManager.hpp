#ifndef WIFI_MANAGER_HPP_
#define WIFI_MANAGER_HPP_

#include "Settings.hpp"
#include "WiFi.h"
#include "WebServer.hpp"

class WifiManager;
extern WifiManager wifiManager;

enum WifiConfigOptions {
    WIFI_RESTART,
    WIFI_MANAGER,
    WIFI_RETRY,
    WIFI_WAIT,
    WIFI_MANAGER_RESET_ONLY
};


class WifiManager {
    private:
        uint32_t lastEvent = 0;                 // Last wifi connection event
        uint32_t downtime = 0;                   // Wifi down duration
        uint16_t linkCount = 0;                 // Number of wifi re-connect
        int8_t counter;
        uint8_t retryInit;
        uint8_t retry;
        uint8_t status;
        uint8_t configCounter = 0;
        uint8_t scanState = 0;
        uint8_t bssid[6];
        int8_t bestNetworkDb;
        uint8_t configType = 0;
    public:
        WifiManager();
        ~WifiManager();
        static int getRssiAsQuality(int rssi);
        bool getConfigCounter(void);
        void config(uint8_t type);
        void setMode(WiFiMode_t wifi_mode);
        void setSleepMode(void);
        void begin(uint8_t flag, uint8_t channel);
        void beginAfterScan(void);
        uint16_t getLinkCount(void);
        String getDowntime(void);
        void setState(uint8_t state);
        bool checkIPv6(void);
        String getIPv6(void);
        bool checkIPAddrStatus(void);
        void checkIp(void);
        void check(uint8_t param);
        int getState(void);
        String getOutputPower(void);
        void setOutputPower(void);
        void connect(void);
        void shutdown(bool option = false);
        void stationKeepAliveNow(void);
        void wifiKeepAlive(void);
        
};

#endif
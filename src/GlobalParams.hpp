#include "Arduino.h"
#include "Config.hpp"
#include "WifiManager.hpp"
#include "TFT_LCD.h"

#ifndef GLOBAL_PARAMS_HPP_
#define GLOBAL_PARAMS_HPP_

class GlobalParams;
extern GlobalParams globalParams;

class GlobalParams {
    protected:
        uint32_t buadrate;
        uint32_t uptime;
        uint8_t wifiState = 0;
        bool wifiEnabled = false;
        bool networkEnabled = false;
        bool httpEnabled = false;
        uint8_t wifiManagerEnabled = false;
        uint8_t restartFlag = false;
        bool restartHalt= false;
        bool wifiConnected =  false;
        bool homeAssistantConnected = false;
        bool homeAssistantSyncing = false;
        bool homeAssistantConnectionFailed = false;
        bool wifiApMode = false;
        uint32_t sleepTime = 10;
        uint8_t currentScreenNum = 0;
        bool ntpForceSync = 0;
        bool timeInit = 0;
        bool timeSet = 0;
        bool screenRebuild = true;
        bool screenDarkMode = true;
        bool screenBottomHeader = false;
        ScreenOrientation screenRotation = ScreenLandscape;
        #ifndef FIRMWARE_MINIMAL
        ScreenPageType screenPageType = ScreenPageMain;
        #else
        ScreenPageType screenPageType = ScreenPageScreenSaver;
        #endif
        uint8_t screenPageNumber = 0;
        uint16_t webLogIndex = 0;
        uint32_t lastTouchTimeMillis = 0;
    public:
        GlobalParams();
        ~GlobalParams();
        inline uint8_t getRestartFlag();
        inline void setRestartFlag(uint8_t restartFlag);
        inline bool getRestartHalt();
        inline void setRestartHalt(bool restartHalt);
        inline void setWifiState(uint8_t state);
        inline uint8_t getWifiState();
        inline bool isWifiDown();
        inline bool isWifiUp();
        inline void setWifiEnabled(bool enabled);
        inline bool isHttpEnabled();
        inline void setHttpEnabled(bool enabled);
        inline void setUptime(uint32_t uptime);
        inline uint32_t getUptime();
        String getUpTimeString();
        inline void setWifiConnected(bool wifiConnected);
        inline bool isWifiConnected();
        inline void setHomeAssistantConnected(bool homeAssistantConnected);
        inline bool isHomeAssistantConnected();
        inline void setWifiApMode(bool mode);
        inline bool isWifiApMode();
        inline void setSleepTime(uint32_t sleepTime);
        inline uint32_t getSleepTime();
        inline void setNtpForceSync(bool sync);
        inline bool getNtpForceSync();
        inline void setTimeInit(bool init);
        inline bool isTimeInit();
        inline void setTimeSet(bool set);
        inline bool isTimeSet();
        inline void setWebLogIndex(uint16_t index);
        inline uint16_t getWebLogIndex();
        inline void incrWebLogIndex();
        inline void incrUptime();
        inline void setScreenRebuild(bool rebuild);
        inline bool getScreenRebuild();
        inline void setScreenPageType(ScreenPageType type);
        inline ScreenPageType getScreenPageType();
        inline void setScreenPageNumber(uint8_t pageNum);
        inline uint8_t getScreenPageNumber();
        inline void setHomeAssistantSyncing(bool syncing);
        inline bool isHomeAssistantSyncing();
        inline void setHomeAssistantConnectionFailed(bool failed);
        inline bool isHomeAssistantConnectionFailed();
        inline void setLastTouchTimeMillis(uint32_t millis);
        inline uint32_t getLastTouchTimeMillis();
};

uint8_t GlobalParams::getRestartFlag() {
    return this->restartFlag;
}

void GlobalParams::setRestartFlag(uint8_t restartFlag) {
    this->restartFlag = restartFlag;
}

bool GlobalParams::getRestartHalt() {
    return this->restartHalt;
}

void GlobalParams::setRestartHalt(bool restartHalt) {
    this->restartHalt = restartHalt;
}

void GlobalParams::setWifiState(uint8_t state) {
    this->wifiState = state;
}

uint8_t GlobalParams::getWifiState() {
    return this->wifiState;
}

bool GlobalParams::isWifiDown() {
    return !this->wifiEnabled;
}

bool GlobalParams::isWifiUp() {
    return this->wifiEnabled;
}

void GlobalParams::setWifiEnabled(bool enabled) {
    this->wifiEnabled = enabled;
    if (!enabled) {
        this->networkEnabled = 0;
        this->httpEnabled = 0;
    }
}

bool GlobalParams::isHttpEnabled() {
    return this->httpEnabled;
}

void GlobalParams::setHttpEnabled(bool enabled) {
    this->httpEnabled = enabled;
}

void GlobalParams::setUptime(uint32_t uptime)  {
    this->uptime = uptime;
}

uint32_t GlobalParams::getUptime() {
    return this->uptime;
}

void GlobalParams::setWifiConnected(bool wifiConnected) {
    this->wifiConnected = wifiConnected;
    if (!wifiConnected)
        this->homeAssistantConnected = false;
}

bool GlobalParams::isWifiConnected() {
    return this->wifiConnected;
}

void GlobalParams::setHomeAssistantConnected(bool homeAssistantConnected) {
    this->homeAssistantConnected = homeAssistantConnected;
}

bool GlobalParams::isHomeAssistantConnected() {
    return this->homeAssistantConnected;
}

void GlobalParams::setSleepTime(uint32_t sleepTime) {
    this->sleepTime = sleepTime;
}

uint32_t GlobalParams::getSleepTime() {
    return this->sleepTime;
}

void GlobalParams::setNtpForceSync(bool sync) {
    this->ntpForceSync = sync;
}

bool GlobalParams::getNtpForceSync() {
    return this->ntpForceSync;
}

void GlobalParams::setTimeInit(bool init) {
    this->timeInit = init;
}

bool GlobalParams::isTimeInit() {
    return this->timeInit;
}

void GlobalParams::setTimeSet(bool set) {
    this->timeSet = set;
}

bool GlobalParams::isTimeSet() {
    return this->timeSet;
}

void GlobalParams::setWebLogIndex(uint16_t index) {
    this->webLogIndex = index;
}

void GlobalParams::incrWebLogIndex() {
    this->webLogIndex++;
}

uint16_t GlobalParams::getWebLogIndex() {
    return this->webLogIndex;
}

void GlobalParams::incrUptime() {
    this->uptime++;
}

void GlobalParams::setScreenRebuild(bool rebuild) {
    this->screenRebuild = rebuild;
}

bool GlobalParams::getScreenRebuild() {
    return this->screenRebuild;
}

void GlobalParams::setScreenPageType(ScreenPageType type) {
    if (this->screenPageType != type) {
        this->screenPageType = type;
        this->screenRebuild = true;
    }
}

ScreenPageType GlobalParams::getScreenPageType() {
    return this->screenPageType;
}

void GlobalParams::setScreenPageNumber(uint8_t pageNum) {
    if (this->screenPageNumber != pageNum) {
        this->screenPageNumber = pageNum;
        this->screenRebuild = true;
    }
}

uint8_t GlobalParams::getScreenPageNumber() {
    return this->screenPageNumber;
}

void GlobalParams::setWifiApMode(bool mode) {
    this->wifiApMode = mode;
}

bool GlobalParams::isWifiApMode() {
    return this->wifiApMode;
}

void GlobalParams::setHomeAssistantSyncing(bool syncing) {
    this->homeAssistantSyncing = syncing;
}

bool GlobalParams::isHomeAssistantSyncing() {
    return this->homeAssistantSyncing;
}

void GlobalParams::setHomeAssistantConnectionFailed(bool failed) {
    this->homeAssistantConnectionFailed = failed;
}

bool GlobalParams::isHomeAssistantConnectionFailed() {
    return this->homeAssistantConnectionFailed;
}

void GlobalParams::setLastTouchTimeMillis(uint32_t millis) {
    this->lastTouchTimeMillis = millis;
}

uint32_t GlobalParams::getLastTouchTimeMillis() {
    return this->lastTouchTimeMillis;
}

#endif
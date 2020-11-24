#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include <stdint.h>
#include <Arduino.h>
#include "Config.hpp"
#include "Support.hpp"
#include "TFT_LCD.h"
#include "GlobalParams.hpp"
#include "LcdTheme.hpp"

class Settings;
extern Settings settings;

enum WifiManagerModes{
    WIFI_AP_MODE
};

typedef union {                            
  uint32_t data;                          
  struct {
    bool activeSta : 1;
    bool useWifiScan : 1;
    bool useWifiRescan : 1;
    bool webserverEnabled: 1;
    bool darkMode : 1;
    bool bottomBar : 1;
    bool wifiManagerMode : 2;
    bool showIpGui : 1;
    uint32_t spare : 25;
  };
} SysBitfield;

struct WifiCredential {
    char ssid[WIFI_MAX_SSID_SIZE+1];
    char password[WIFI_MAX_PASSWORD_SIZE+1];
};

struct WebTheme {
    Color colorText;
    Color colorBackground;
    Color colorForm;
    Color colorInputText;
    Color colorInput;
    Color colorConsoleText;
    Color colorConsole;
    Color colorTextWarning;
    Color colorTextSuccess;
    Color colorButtonText;
    Color colorButton;
    Color colorButtonHover;
    Color colorButtonReset;
    Color colorButtonResetHover;
    Color colorButtonSave;
    Color colorButtonSaveHover;
    Color colorButtonTimerTabText;
    Color colorButtonTimerTabBackground;
    Color colorTitle;
};

const WebTheme DEFAULT_WEB_THEME = {
    .colorText = {COLOR_TEXT},
    .colorBackground = {COLOR_BACKGROUND},
    .colorForm = {COLOR_FORM},
    .colorInputText = {COLOR_INPUT_TEXT},
    .colorInput = {COLOR_INPUT},
    .colorConsoleText = {COLOR_CONSOLE_TEXT},
    .colorConsole = {COLOR_CONSOLE},
    .colorTextWarning = {COLOR_TEXT_WARNING},
    .colorTextSuccess = {COLOR_TEXT_SUCCESS},
    .colorButtonText = {COLOR_BUTTON_TEXT},
    .colorButton = {COLOR_BUTTON},
    .colorButtonHover = {COLOR_BUTTON_HOVER},
    .colorButtonReset = {COLOR_BUTTON_RESET},
    .colorButtonResetHover = {COLOR_BUTTON_RESET_HOVER},
    .colorButtonSave = {COLOR_BUTTON_SAVE},
    .colorButtonSaveHover = {COLOR_BUTTON_SAVE_HOVER},
    .colorButtonTimerTabText = {COLOR_TIMER_TAB_TEXT},
    .colorButtonTimerTabBackground  = {COLOR_TIMER_TAB_BACKGROUND},
    .colorTitle = {COLOR_TITLE_TEXT}
}; 

const LcdTheme DEFAULT_LIGHT_LCD_THEME = {
    .colorBodyBackground = {COLOR_LIGHT_LCD_BODY_BACKGROUND},
    .colorBodyText = {COLOR_LIGHT_LCD_BODY_TEXT},
    .colorHeaderBackground = {COLOR_LIGHT_LCD_HEADER_BACKGROUND},
    .colorHeaderText = {COLOR_LIGHT_LCD_HEADER_TEXT},
    .colorHeaderIcon = {COLOR_LIGHT_LCD_HEADER_ICON},
    .colorPageIndicator = {COLOR_LIGHT_LCD_PAGE_INDICATOR},
    .colorPageIndicatorSelected = {COLOR_LIGHT_LCD_PAGE_INDICATOR_SELECTED},
    .colorScreenSaverBackground = {COLOR_LIGHT_LCD_SS_BACKGROUND},
    .colorScreenSaverText = {COLOR_LIGHT_LCD_SS_TEXT},
    .colorScreenSaverHeaderIcon = {COLOR_LIGHT_LCD_SS_HEADER_ICON},
    .colorEntityIcon = {COLOR_LIGHT_LCD_ENTITY_ICON},
    .colorEntityIconUnavailable = {COLOR_LIGHT_LCD_ENTITY_ICON_UNAVAILABLE},
    .colorEntityIconEnabled = {COLOR_LIGHT_LCD_LCD_ENTITY_ICON_ENABLED},
    .colorSwitchSecondary = {COLOR_LIGHT_SWITCH_SECONDARY},
    .colorSwitchSurface = {COLOR_LIGHT_SWITCH_SURFACE},
    .colorSwitchOnSurface = {COLOR_LIGHT_SWITCH_ON_SURFACE},
};

const LcdTheme DEFAULT_DARK_LCD_THEME = {
    .colorBodyBackground = {COLOR_DARK_LCD_BODY_BACKGROUND},
    .colorBodyText = {COLOR_DARK_LCD_BODY_TEXT},
    .colorHeaderBackground = {COLOR_DARK_LCD_HEADER_BACKGROUND},
    .colorHeaderText = {COLOR_DARK_LCD_HEADER_TEXT},
    .colorHeaderIcon = {COLOR_DARK_LCD_HEADER_ICON},
    .colorPageIndicator = {COLOR_DARK_LCD_PAGE_INDICATOR},
    .colorPageIndicatorSelected = {COLOR_DARK_LCD_PAGE_INDICATOR_SELECTED},
    .colorScreenSaverBackground = {COLOR_DARK_LCD_SS_BACKGROUND},
    .colorScreenSaverText = {COLOR_DARK_LCD_SS_TEXT},
    .colorScreenSaverHeaderIcon = {COLOR_DARK_LCD_SS_HEADER_ICON},
    .colorEntityIcon = {COLOR_DARK_LCD_ENTITY_ICON},
    .colorEntityIconUnavailable = {COLOR_DARK_LCD_ENTITY_ICON_UNAVAILABLE},
    .colorEntityIconEnabled = {COLOR_DARK_LCD_LCD_ENTITY_ICON_ENABLED},
    .colorSwitchSecondary = {COLOR_DARK_SWITCH_SECONDARY},
    .colorSwitchSurface = {COLOR_DARK_SWITCH_SURFACE},
    .colorSwitchOnSurface = {COLOR_DARK_SWITCH_ON_SURFACE},
};

#pragma pack(push, 1)
class Settings {
    public:
        Settings();
        inline uint32_t getVersion();
        inline uint16_t getVersionMajor();
        inline uint16_t getVersionMinor();
        inline String getVersionString();
        inline size_t getSize();
        uint32_t computeCrc32();
        inline uint32_t getCrc32();
        inline void setCrc32(uint32_t crc32);
        inline void updateCrc32();
        inline bool isCrc32Matching();
        void updateTimestamp();
        void save();
        void load();
        void setDefault();
        inline const char * getOtaUrl();
        inline void setStaWifiCredentials(const char * ssid1, const char * password1, const char * ssid2, const char * password2);
        inline void setStaWifiCredentials(const String ssid1, const String password1, const String ssid2, const String password2);
        inline void setStaWifiSsid1(const char * ssid);
        inline void setStaWifiSsid2(const char * ssid);
        inline void setStaWifiPassword1(const char * password);
        inline void setStaWifiPassword2(const char * password);
        inline const char * getStaWifiSsid(uint8_t index);
        inline const char * getStaWifiPassword(uint8_t index);
        inline const char * getStaWifiSsid1();
        inline const char * getStaWifiSsid2();
        inline const char * getStaWifiPassword1();
        inline const char * getStaWifiPassword2();
        inline const char * getStaWifiActiveSsid();
        inline const char * getStaWifiActivePassword();
        inline void setApWifiCredential(const char * ssid, const char * password);
        inline void setApWifiCredential(const String ssid, const String password);
        inline const char * getApWifiSsid();
        inline const char * getApWifiPassword();
        inline void setWifiCorsHeader(const char * wifiCorsHeader);
        inline const char * getWifiCorsHeader();
        inline void setStaIpAddress(const uint32_t ipAddress);
        inline void setStaIpAddress(const char * ipAddress);
        inline void setStaIpAddress(const String ipAddress);
        inline const uint32_t getStaIpAddress();
        inline void setStaGatewayIpAddress(const uint32_t ipAddress);
        inline void setStaGatewayIpAddress(const char * ipAddress);
        inline void setStaGatewayIpAddress(const String ipAddress);
        inline const uint32_t getStaGatewayIpAddress();
        inline void setStaSubnetIpAddress(const uint32_t ipAddress);
        inline void setStaSubnetIpAddress(const char * ipAddress);
        inline void setStaSubnetIpAddress(const String ipAddress);
        inline const uint32_t getStaSubnetIpAddress();
        inline void setStaDnsIpAddress(const uint32_t ipAddress);
        inline void setStaDnsIpAddress(const char * ipAddress);
        inline void setStaDnsIpAddress(const String ipAddress);
        inline const uint32_t getStaDnsIpAddress();
        inline bool isStaIpAddressValid();
        inline bool isStaGatewayIpAddressValid();
        inline bool isStaSubnetIpAddressValid();
        inline bool isStaDnsIpAddressValid();
        inline void setWebUsername(const char * username);
        inline const char * getWebUsername();
        inline void setWebPassword(const char * password);
        inline const char * getWebPassword();
        inline void setDeviceName(const char * deviceName);
        inline const char * getDeviceName();
        inline void setDarkMode(bool darkMode);
        inline bool isDarkMode();
        inline void setBottomBar(bool bottomBar);
        inline bool isBottomBar();
        inline void getBgColor();
        inline void getFgColor();
        inline void setWifiOutputPower(uint8_t wifiOutputPower);
        inline uint8_t getWifiOutputPower();
        inline void setWifiConfigOption(uint8_t option);
        inline uint8_t getWifiConfigOption();
        inline uint32_t getSaveCount();
        inline void incrBootCount();
        inline uint32_t getBootCount();
        inline bool isWebserverEnabled();
        inline void setWebServerEnabled(bool enabled);
        inline void setSerialLogLevel(uint8_t level);
        inline uint8_t getSerialLogLevel();
        inline void setWebLogLevel(uint8_t level);
        inline uint8_t getWebLogLevel();
        inline void setSysLogLevel(uint8_t level);
        inline uint8_t getSysLogLevel();
        inline void setMasterLogLevel(uint8_t level);
        inline uint8_t getMasterLogLevel();
        inline void setActiveSta(uint8_t active);
        inline uint8_t getActiveSta();
        inline void setShowIpGui(bool showIpGui);
        inline bool getShowIpGui();
        inline void setUseWifiScan(bool useWifiScan);
        inline bool getUseWifiScan();
        inline void setUseWifiRescan(bool useWifiRescan);
        inline bool getUseWifiRescan();
        inline void setHostname(const char * hostname);
        inline const char * getHostname();
        inline const char * getHostnameRaw();
        inline void setSysLogHostname(const char * sysLogHostname);
        inline const char * getSysLogHostname();
        inline void setSysLogHostPort(uint16_t sysLogHostPort);
        inline uint16_t getSysLogHostPort();
        inline void setSysLogTelePeriod(uint16_t sysLogTelePeriod);
        inline uint16_t getSysLogTelePeriod();
        inline void setWifiChannel(uint8_t channel);
        inline uint8_t getWifiChannel();
        inline void setWifiBssid(uint8_t * bssid);
        inline uint8_t * getWifiBssid();
        inline uint16_t getWebRefreshInterval();
        inline void setWebRefreshInterval(uint16_t webRefreshInterval);
        inline WebTheme * getWebTheme();
        inline void setWebTheme(WebTheme * theme);
        inline LcdTheme * getLcdDarkTheme();
        inline void setLcdDarkTheme(LcdTheme * theme);
        inline LcdTheme * getLcdLightTheme();
        inline void setLcdSelectedTheme(LcdTheme * theme);
        inline LcdTheme * getLcdSelectedTheme();
        inline void setLcdLightTheme(LcdTheme * theme);
        inline void setNtpServer(const char * server, uint8_t index);
        inline void setNtpServer1(const char * server);
        inline void setNtpServer2(const char * server);
        inline void setNtpServer3(const char * server);
        inline const char * getNtpServer(uint8_t index);
        inline const char * getNtpServer1();
        inline const char * getNtpServer2();
        inline const char * getNtpServer3();
        inline void setTimezoneOffset1(uint16_t offset);
        inline uint16_t getTimezoneOffset1();
        inline void setTimezoneOffset2(uint16_t offset);
        inline uint16_t getTimezoneOffset2();
        inline void setTimezone(uint16_t timezone);
        inline uint16_t getTimezone();
        inline void setTimezoneMintues(uint16_t timezoneMinutes);
        inline uint16_t getTimezoneMinutes();
        inline void setTimeRuleSTD(TimeRule rule);
        inline TimeRule getTimeRuleSTD();
        inline void setTimeRuleDST(TimeRule rule);
        inline TimeRule getTimeRuleDST();
        void resetTimeRuleSTD();
        void resetTimeRuleDST();
        inline void setScreenOrientation(ScreenOrientation orientation);
        inline ScreenOrientation getScreenOrientation();
        inline void setScreenSaverTime(uint32_t screenSaverTime);
        inline uint32_t getScreenSaverTime();
        inline void setScreenSaverEnable(bool enable);
        inline bool isScreenSaverEnabled();
    protected:
        uint16_t magic;
        uint16_t size;
        uint32_t saveCount;
        uint32_t version;
        uint32_t bootCount;
        LcdTheme lcdLightTheme;
        LcdTheme lcdDarkTheme;
        WebTheme webTheme;
        SysBitfield flags;
        uint8_t wifiOutputPower;
        uint8_t wifiStaConfig;
        uint8_t masterLogLevel;
        uint8_t seriallogLevel;
        uint8_t webLogLevel;
        uint8_t sysLogLevel;
        uint8_t wifiChannel;
        uint8_t       wifiBssid[6]; 
        WifiCredential staWifiCredential[2];
        WifiCredential apWifiCredential;
        char otaUrl[100];
        uint32_t staIpAddress;
        uint32_t staGatewayIpAddress;
        uint32_t staSubnetIpAddress;
        uint32_t staDnsIpAddress;
        uint16_t webRefreshInterval; 
        uint16_t sysLogHostPort;
        uint16_t sysLogTelePeriod;
        char wifiCorsHeader[WIFI_MAX_CORS_HEADER_SIZE+1];
        char webUsername[WEB_MAX_USERNAME_SIZE+1];
        char webPassword[WEB_MAX_PASSWORD_SIZE+1];
        char deviceName[MAX_DEVICE_NAME_SIZE+1];
        char hostname[MAX_HOSTNAME_SIZE+1];
        char hostnameRaw[MAX_HOSTNAME_SIZE+1];
        char sysLogHostname[MAX_HOSTNAME_SIZE+1];
        char ntpServers[3][MAX_HOSTNAME_SIZE+1];
        uint16_t timezoneOffset[2];
        TimeRule      timeRules[2];
        uint16_t timezone;
        uint16_t timezoneMinutes;
        ScreenOrientation screenOrientation;
        uint32_t screenSaverTime;
        bool screenSaverEnable;
        uint32_t      timestamp;
        uint32_t      crc32;     
};
#pragma pack(pop)

uint32_t Settings::getVersion() {
    return this->version;
}
uint16_t Settings::getVersionMajor() {
    return this->version >> 16;
}

uint16_t Settings::getVersionMinor() {
    return this->version & 0xFFFF;
}

String Settings::getVersionString() {
    return String(getVersionMajor())+"."+String(getVersionMinor());
}

size_t Settings::getSize() {
    return sizeof(Settings);
}

const char * Settings::getOtaUrl() {
    return this->otaUrl;
}

uint32_t Settings::getCrc32() {
    return this->crc32;
}

void Settings::setCrc32(uint32_t crc32) {
    this->crc32 = crc32;
}

void Settings::updateCrc32() {
    this->crc32 = computeCrc32();
}

bool Settings::isCrc32Matching() {
    return this->computeCrc32() == this->crc32;
}

void Settings::setStaWifiCredentials(const char * ssid1, const char * password1, const char * ssid2, const char * password2) {
    if (ssid1 != nullptr)
        strncpy(this->staWifiCredential[0].ssid, ssid1, WIFI_MAX_SSID_SIZE);
    if (password1 != nullptr)
        strncpy(this->staWifiCredential[0].password, password1, WIFI_MAX_PASSWORD_SIZE);
    if (ssid2 != nullptr)
        strncpy(this->staWifiCredential[1].ssid, ssid2, WIFI_MAX_SSID_SIZE);
    if (password2 != nullptr)
        strncpy(this->staWifiCredential[1].password, password2, WIFI_MAX_PASSWORD_SIZE);
}

void Settings::setStaWifiCredentials(const String ssid1, const String password1, const String ssid2, const String password2) {
    strncpy(this->staWifiCredential[0].ssid, ssid1.c_str(), WIFI_MAX_SSID_SIZE);
    strncpy(this->staWifiCredential[0].password, password1.c_str(), WIFI_MAX_PASSWORD_SIZE);
    strncpy(this->staWifiCredential[1].ssid, ssid2.c_str(), WIFI_MAX_SSID_SIZE);
    strncpy(this->staWifiCredential[1].password, password2.c_str(), WIFI_MAX_PASSWORD_SIZE);
}

void Settings::setStaWifiSsid1(const char * ssid) {
     if (ssid != nullptr)
        strncpy(this->staWifiCredential[0].ssid, ssid, WIFI_MAX_SSID_SIZE);
}

void Settings::setStaWifiSsid2(const char * ssid) {
    if (ssid != nullptr)
        strncpy(this->staWifiCredential[1].ssid, ssid, WIFI_MAX_SSID_SIZE);
}

void Settings::setStaWifiPassword1(const char * password) {
    if (password != nullptr)
        strncpy(this->staWifiCredential[0].password, password, WIFI_MAX_PASSWORD_SIZE);
}

void Settings::setStaWifiPassword2(const char * password) {
    if (password != nullptr)
        strncpy(this->staWifiCredential[1].password, password, WIFI_MAX_PASSWORD_SIZE);
}

const char * Settings::getStaWifiSsid(uint8_t index) {
    if (index<2)
        return this->staWifiCredential[index].ssid;
    else
        return nullptr;
}

const char * Settings::getStaWifiPassword(uint8_t index) {
    if (index<2)
        return this->staWifiCredential[index].password;
    else
        return nullptr;
}

const char * Settings::getStaWifiSsid1() {
    return this->staWifiCredential[0].ssid;
}

const char * Settings::getStaWifiSsid2() {
    return this->staWifiCredential[1].ssid;
}

const char * Settings::getStaWifiPassword1() {
    return this->staWifiCredential[0].password;
}

const char * Settings::getStaWifiPassword2() {
    return this->staWifiCredential[1].password;
}

const char * Settings::getStaWifiActiveSsid() {
    if (this->flags.activeSta == 0) {
        return getStaWifiSsid1();
    } else {
        return getStaWifiSsid2();
    }
}

const char * Settings::getStaWifiActivePassword() {
    if (this->flags.activeSta == 0) {
        return getStaWifiPassword1();
    } else {
        return getStaWifiPassword2();
    }
}

void Settings::setWifiCorsHeader(const char * wifiCorsHeader) {
    if (wifiCorsHeader != nullptr)
        strncpy(this->wifiCorsHeader, wifiCorsHeader, WIFI_MAX_CORS_HEADER_SIZE);
}

void Settings::setApWifiCredential(const char * ssid, const char * password) {
    if (ssid != nullptr)
        strncpy(this->apWifiCredential.ssid, ssid, WIFI_MAX_SSID_SIZE);
    if (password != nullptr)
        strncpy(this->apWifiCredential.password, password, WIFI_MAX_PASSWORD_SIZE);
}

void Settings::setApWifiCredential(const String ssid, const String password) {
    strncpy(this->apWifiCredential.ssid, ssid.c_str(), WIFI_MAX_SSID_SIZE);
    strncpy(this->apWifiCredential.password, password.c_str(), WIFI_MAX_PASSWORD_SIZE);
}

const char * Settings::getApWifiSsid() {
    return this->apWifiCredential.ssid;
}

const char * Settings::getApWifiPassword() {
    return this->apWifiCredential.password;
}

const char * Settings::getWifiCorsHeader() {
    return this->wifiCorsHeader;
}

void Settings::setStaIpAddress(const char * ipAddress) {
    Support::parseIp(&(this->staIpAddress), ipAddress);
}

void Settings::setStaIpAddress(const String ipAddress) {
    setStaIpAddress(ipAddress.c_str());
}

void Settings::setStaIpAddress(const uint32_t ipAddress) {
    this->staIpAddress = ipAddress;
}

const uint32_t Settings::getStaIpAddress() {
    return this->staIpAddress;
}

bool Settings::isStaIpAddressValid() {
    return (this->staIpAddress != 0);
}

void Settings::setStaGatewayIpAddress(const char * ipAddress) {
    Support::parseIp(&(this->staGatewayIpAddress), ipAddress);
}

void Settings::setStaGatewayIpAddress(const String ipAddress) {
    setStaGatewayIpAddress(ipAddress.c_str());
}

void Settings::setStaGatewayIpAddress(const uint32_t ipAddress) {
    this->staGatewayIpAddress = ipAddress;
}

const uint32_t Settings::getStaGatewayIpAddress() {
    return this->staGatewayIpAddress;
}

bool Settings::isStaGatewayIpAddressValid() {
    return (this->staGatewayIpAddress != 0);
}

void Settings::setStaSubnetIpAddress(const char * ipAddress) {
    Support::parseIp(&(this->staSubnetIpAddress), ipAddress);
}

void Settings::setStaSubnetIpAddress(const String ipAddress) {
    setStaSubnetIpAddress(ipAddress.c_str());
}

void Settings::setStaSubnetIpAddress(const uint32_t ipAddress) {
    this->staSubnetIpAddress = ipAddress;
}

const uint32_t Settings::getStaSubnetIpAddress() {
    return this->staSubnetIpAddress;
}

bool Settings::isStaSubnetIpAddressValid() {
    return (this->staSubnetIpAddress != 0);
}

void Settings::setStaDnsIpAddress(const char * ipAddress) {
    Support::parseIp(&(this->staDnsIpAddress), ipAddress);
}

void Settings::setStaDnsIpAddress(const String ipAddress) {
    setStaDnsIpAddress(ipAddress.c_str());
}

void Settings::setStaDnsIpAddress(const uint32_t ipAddress) {
    this->staDnsIpAddress = ipAddress;
}

const uint32_t Settings::getStaDnsIpAddress() {
    return this->staDnsIpAddress;
}

bool Settings::isStaDnsIpAddressValid() {
    return (this->staDnsIpAddress != 0);
}

void Settings::setWebUsername(const char * username) {
    strncpy(this->webUsername, username, WEB_MAX_USERNAME_SIZE);
}

const char * Settings::getWebUsername() {
    return this->webUsername;
}

void Settings::setWebPassword(const char * password) {
    strncpy(this->webPassword, password, WEB_MAX_PASSWORD_SIZE);
}

const char * Settings::getWebPassword() {
    return this->webPassword;
}

void Settings::setDeviceName(const char * deviceName) {
    strncpy(this->deviceName, deviceName, MAX_DEVICE_NAME_SIZE);
}

const char * Settings::getDeviceName() {
    return this->deviceName;
}

void Settings::setDarkMode(bool darkMode) {
    if (this->flags.darkMode != darkMode) {
        this->flags.darkMode = darkMode;
        globalParams.setScreenRebuild(true);
    }
}

bool Settings::isDarkMode() {
    return this->flags.darkMode;
}

void Settings::setBottomBar(bool bottomBar) {
    if (bottomBar != this->flags.bottomBar) {
        this->flags.bottomBar = bottomBar;
        globalParams.setScreenRebuild(true);
    }
}

bool Settings::isBottomBar() {
    return this->flags.bottomBar;
}

void Settings::setWifiOutputPower(uint8_t wifiOutputPower) {
    this->wifiOutputPower = wifiOutputPower;
}

uint8_t Settings::getWifiOutputPower() {
    return this->wifiOutputPower;
}

void Settings::setWifiConfigOption(uint8_t option) {
    if (option < MAX_WIFI_OPTION)
        this->wifiStaConfig = option;
}

uint8_t Settings::getWifiConfigOption() {
    return this->wifiStaConfig;
}

uint32_t Settings::getSaveCount() {
    return this->saveCount;
}

void Settings::incrBootCount() {
    this->bootCount++;
}

uint32_t Settings::getBootCount() {
    return this->bootCount;
}   

bool Settings::isWebserverEnabled() {
    return this->flags.webserverEnabled;
}
void Settings::setWebServerEnabled(bool enabled) {
    this->flags.webserverEnabled = enabled;
}

void Settings::setSerialLogLevel(uint8_t level) {
    this->seriallogLevel = level;
}

uint8_t Settings::getSerialLogLevel() {
    return this->seriallogLevel;
}

void Settings::setWebLogLevel(uint8_t level) {
    this->webLogLevel = level;
}

uint8_t Settings::getWebLogLevel() {
    return this->webLogLevel;
}

void Settings::setSysLogLevel(uint8_t level) {
    this->sysLogLevel = level;
}

uint8_t Settings::getSysLogLevel() {
    return this->sysLogLevel;
}

void Settings::setMasterLogLevel(uint8_t level) {
    this->masterLogLevel = level;
}

uint8_t Settings::getMasterLogLevel() {
    return this->masterLogLevel;
}

void Settings::setActiveSta(uint8_t active) {
    this->flags.activeSta = active;
}

uint8_t Settings::getActiveSta() {
    return this->flags.activeSta;
}

void Settings::setShowIpGui(bool showIpGui) {
    this->flags.showIpGui = showIpGui;
}

bool Settings::getShowIpGui() {
    return this->flags.showIpGui;
}

void Settings::setUseWifiScan(bool useWifiScan) {
    this->flags.useWifiScan = useWifiScan;
}

bool Settings::getUseWifiScan() {
    return this->flags.useWifiScan;
}

void Settings::setUseWifiRescan(bool useWifiRescan) {
    this->flags.useWifiRescan = useWifiRescan;
}

bool Settings::getUseWifiRescan() {
    return this->flags.useWifiRescan;
}

void Settings::setHostname(const char * hostname) {
    if (strstr(hostname, "%") != nullptr) {
        snprintf(this->hostname, 
            MAX_HOSTNAME_SIZE, hostname, PROJECT, ((uint32_t)ESP.getEfuseMac()) & 0x1FFF);
    } else {
        strncpy(this->hostname, hostname, MAX_HOSTNAME_SIZE);
    }
}

const char * Settings::getHostname() {
    return this->hostname;
}

const char * Settings::getHostnameRaw() {
    return this->hostnameRaw;
}

void Settings::setSysLogHostname(const char * sysLogHostname) {
    strncpy(this->sysLogHostname, sysLogHostname, MAX_HOSTNAME_SIZE);
}

const char * Settings::getSysLogHostname() {
    return this->sysLogHostname;
}

void Settings::setSysLogHostPort(uint16_t sysLogHostPort) {
    this->sysLogHostPort = sysLogHostPort;
}

uint16_t Settings::getSysLogHostPort() {
    return this->sysLogHostPort;
}

void Settings::setSysLogTelePeriod(uint16_t sysLogTelePeriod) {
    this->sysLogTelePeriod = sysLogTelePeriod;
}

uint16_t Settings::getSysLogTelePeriod() {
    return this->sysLogTelePeriod;
}

void Settings::setWifiChannel(uint8_t channel) {
    this->wifiChannel = channel;
}

uint8_t Settings::getWifiChannel() {
    return this->wifiChannel;
}

void Settings::setWifiBssid(uint8_t * bssid) {
    memcpy((void*) &(wifiBssid), (void*) bssid, sizeof(wifiBssid));
}

uint8_t * Settings::getWifiBssid() {
    return this->wifiBssid;
}

uint16_t Settings::getWebRefreshInterval() {
	return this->webRefreshInterval;
}
void Settings::setWebRefreshInterval(uint16_t webRefreshInterval) {
	this->webRefreshInterval = webRefreshInterval;
}

WebTheme * Settings::getWebTheme() {
    return &(this->webTheme);
}

void Settings::setWebTheme(WebTheme * theme) {
    this->webTheme = *theme;
}

LcdTheme * Settings::getLcdLightTheme() {
    return &(this->lcdLightTheme);
}

void Settings::setLcdLightTheme(LcdTheme * theme) {
    this->lcdLightTheme = *theme;
}

LcdTheme * Settings::getLcdDarkTheme() {
    return &(this->lcdDarkTheme);
}

void Settings::setLcdDarkTheme(LcdTheme * theme) {
    this->lcdDarkTheme = *theme;
}

void Settings::setLcdSelectedTheme(LcdTheme * theme) {
    if (isDarkMode())
        setLcdDarkTheme(theme);
    else
        setLcdLightTheme(theme);
}

LcdTheme * Settings::getLcdSelectedTheme() {
    if (isDarkMode())
        return getLcdDarkTheme();
    else
        return getLcdLightTheme();
}

void Settings::setNtpServer(const char * server, uint8_t index) {
    if (index < 3)
        strncpy(this->ntpServers[index],server, MAX_HOSTNAME_SIZE);
}

void Settings::setNtpServer1(const char * server) {
    strncpy(this->ntpServers[0], server, MAX_HOSTNAME_SIZE);
}

void Settings::setNtpServer2(const char * server) {
    strncpy(this->ntpServers[1], server, MAX_HOSTNAME_SIZE);
}

void Settings::setNtpServer3(const char * server) {
    strncpy(this->ntpServers[2], server, MAX_HOSTNAME_SIZE);
}

const char * Settings::getNtpServer(uint8_t index) {
    if (index < 3)
        return this->ntpServers[index];
    else
        return nullptr;
}

const char * Settings::getNtpServer1() {
    return this->ntpServers[0];
}

const char * Settings::getNtpServer2() {
    return this->ntpServers[1];
}

const char * Settings::getNtpServer3() {
    return this->ntpServers[2];
}

void Settings::setTimezoneOffset1(uint16_t offset) {
    this->timezoneOffset[0] = offset;
}

uint16_t Settings::getTimezoneOffset1() {
    return this->timezoneOffset[0];
}
void Settings::setTimezoneOffset2(uint16_t offset){
    this->timezoneOffset[1] = offset;
}

uint16_t Settings::getTimezoneOffset2() {
    return this->timezoneOffset[1];
}

void Settings::setTimezone(uint16_t timezone) {
    this->timezone = timezone;
}

uint16_t Settings::getTimezone() {
    return timezone;
}
void Settings::setTimezoneMintues(uint16_t timezoneMinutes) {
    this->timezoneMinutes = timezoneMinutes;
}

uint16_t Settings::getTimezoneMinutes() {
    return timezoneMinutes;
}

void Settings::setTimeRuleSTD(TimeRule rule) {
    this->timeRules[0] = rule;
}

TimeRule Settings::getTimeRuleSTD() {
    return this->timeRules[0];
}

void Settings::setTimeRuleDST(TimeRule rule) {
    this->timeRules[1] = rule;
}

TimeRule Settings::getTimeRuleDST() {
    return this->timeRules[1];
}

void Settings::setScreenOrientation(ScreenOrientation orientation) {
    if (this->screenOrientation != orientation) {
        this->screenOrientation = orientation;
        globalParams.setScreenRebuild(true);
    }
}

ScreenOrientation Settings::getScreenOrientation() {
    return this->screenOrientation;
}

void Settings::setScreenSaverTime(uint32_t screenSaverTime) {
    this->screenSaverTime = screenSaverTime;
}

uint32_t Settings::getScreenSaverTime() {
    return this->screenSaverTime;
}

void Settings::setScreenSaverEnable(bool enable){
    this->screenSaverEnable = enable;
}

bool Settings::isScreenSaverEnabled() {
    return this->screenSaverEnable;
}

#endif
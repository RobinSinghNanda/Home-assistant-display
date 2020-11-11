#include "Settings.hpp"
#include "Support.hpp"
#include "NvmHelper.hpp"
#include "Log.hpp"
#include "WifiManager.hpp"

Settings settings;

Settings::Settings() {
    setDefault();
}

uint32_t Settings::computeCrc32() {
  // https://create.stephan-brumme.com/crc32/#bitwise
  uint32_t size = this->getSize() - sizeof(uint32_t);
  uint8_t * bytes = (uint8_t*)(this);
  uint32_t crc = 0;

  while (size--) {
    crc ^= *bytes++;
    for (uint32_t j = 0; j < 8; j++) {
      crc = (crc >> 1) ^ (-int(crc & 1) & 0xEDB88320);
    }
  }
  return ~crc;
}

void Settings::updateTimestamp() {
    //TODO: Fix timestamp.
    //timestamp = 
    // if (UtcTime() > START_VALID_TIME) {
    //   this->timestamp = UtcTime();
    // } else {
    //   this->timestamp++;
    // }
}

void Settings::save() {
#ifndef FIRMWARE_MINIMAL
    if ((this->computeCrc32() != this->crc32)) {
        this->saveCount++;
        updateTimestamp();
        updateCrc32();
        NvmHelper::NvmSave("main", "settings", this, sizeof(Settings));
    }
#endif  // FIRMWARE_MINIMAL
}

void Settings::load() {
    NvmHelper::NvmLoad("main", "settings", this, sizeof(Settings));
#ifndef FIRMWARE_MINIMAL
    if ((this->magic != (uint16_t)SETTINGS_MAGIC)) {
        Serial.println("Using defualt settings");
        this->setDefault();
    }
#endif  // FIRMWARE_MINIMAL
}

void Settings::setDefault() {
    this->magic = SETTINGS_MAGIC;
    this->size = sizeof(Settings);
    this->saveCount = 0;
    this->version = SOFTWARE_VERSION;
    this->bootCount = 0;
    this->lcdLightTheme = DEFAULT_LIGHT_LCD_THEME;
    this->lcdDarkTheme = DEFAULT_DARK_LCD_THEME;
    this->webTheme = DEFAULT_WEB_THEME;
    this->flags.data = 0;
    this->flags.darkMode = 1;
    this->flags.bottomBar = 0;
    this->flags.showIpGui = 0;
    this->flags.activeSta = 0;
    this->flags.useWifiScan = 1;
    this->flags.useWifiRescan = 1;
    this->flags.webserverEnabled = 1;
    this->flags.wifiManagerMode = 1;
    this->webRefreshInterval = HTTP_REFRESH_TIME;
    this->wifiStaConfig = WIFI_CONFIG_TOOL;
    this->wifiOutputPower = 0;
    this->screenOrientation = ScreenLandscape;
    memset(this->wifiBssid, 0, sizeof(this->wifiBssid));
    setStaWifiCredentials(WIFI_STA_SSID1, WIFI_STA_PASSWORD1, WIFI_STA_SSID2, WIFI_STA_PASSWORD2);
    setWifiCorsHeader(WIFI_CORS_HEADER);
    strncpy(this->otaUrl, OTA_URL, 100);
    setHostname(WIFI_HOSTNAME);
    setDeviceName(PROJECT);
    setWebUsername(WEB_USERNAME);
    setWebPassword(WEB_PASSWORD);
    setSysLogHostname(SYS_LOG_HOST);
    setSysLogHostPort(SYS_LOG_PORT);
    Support::parseIp(&this->staIpAddress, WIFI_IP_ADDRESS);
    Support::parseIp(&this->staGatewayIpAddress, WIFI_GATEWAY);
    Support::parseIp(&this->staSubnetIpAddress, WIFI_SUBNETMASK);
    Support::parseIp(&this->staDnsIpAddress, WIFI_DNS);
    setApWifiCredential(WIFI_AP_SSID, WIFI_AP_PASSWORD);
    this->masterLogLevel = MASTER_LOG_LEVEL;
    this->seriallogLevel = SERIAL_LOG_LEVEL;
    this->webLogLevel = WEB_LOG_LEVEL;
    this->sysLogLevel = SYS_LOG_LEVEL;
    this->flags.activeSta = 0;
    setNtpServer1(NTP_SERVER1);
    setNtpServer2(NTP_SERVER2);
    setNtpServer3(NTP_SERVER3);
    resetTimeRuleSTD();
    resetTimeRuleDST();
    if (((APP_TIMEZONE > -14) && (APP_TIMEZONE < 15)) || (99 == APP_TIMEZONE)) {
        this->timezone = APP_TIMEZONE;
        this->timezoneMinutes = 0;
    } else {
        this->timezone = APP_TIMEZONE / 60;
        this->timezoneMinutes = abs(APP_TIMEZONE % 60);
    }
    this->screenSaverTime = SCREEN_SAVER_TIME;
    this->screenSaverEnable = SCREEN_SAVER_ENABLE;
    this->updateTimestamp();
    this->updateCrc32();
}

void Settings::resetTimeRuleSTD() {
  this->timeRules[0].hemis = TIME_STD_HEMISPHERE;
  this->timeRules[0].week = TIME_STD_WEEK;
  this->timeRules[0].dow = TIME_STD_DAY;
  this->timeRules[0].month = TIME_STD_MONTH;
  this->timeRules[0].hour = TIME_STD_HOUR;
  this->timezoneOffset[0] = TIME_STD_OFFSET;
}

void Settings::resetTimeRuleDST() {
  this->timeRules[1].hemis = TIME_DST_HEMISPHERE;
  this->timeRules[1].week = TIME_DST_WEEK;
  this->timeRules[1].dow = TIME_DST_DAY;
  this->timeRules[1].month = TIME_DST_MONTH;
  this->timeRules[1].hour = TIME_DST_HOUR;
  this->timezoneOffset[1] = TIME_DST_OFFSET;
}
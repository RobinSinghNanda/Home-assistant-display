#ifndef __WIFIMANAGER_H__
#define __WIFIMANAGER_H__

#include "TFT_LCD.h"
#define ESP32 1
#define _ESPASYNC_WIFIMGR_LOGLEVEL_    3
// Default is 30s, using 20s now
#define TIME_BETWEEN_MODAL_SCANS          20000
// Default is 60s, using 30s now
#define TIME_BETWEEN_MODELESS_SCANS       30000

#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>

// From v1.1.1
#include <WiFiMulti.h>
#define FS_NO_GLOBALS
#include <FS.h>
#include <SPIFFS.h>

#define FileFS        SPIFFS
#define FS_Name       "SPIFFS"

#define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())

#define LED_BUILTIN       2
#define LED_ON            HIGH
#define LED_OFF           LOW

#define MIN_AP_PASSWORD_SIZE    8
#define SSID_MAX_LEN            32
#define PASS_MAX_LEN            64

typedef struct
{
  char wifi_ssid[SSID_MAX_LEN];
  char wifi_pw  [PASS_MAX_LEN];
}  WiFi_Credentials;

typedef struct
{
  String wifi_ssid;
  String wifi_pw;
}  WiFi_Credentials_String;

#define NUM_WIFI_CREDENTIALS      2

typedef struct
{
  WiFi_Credentials  WiFi_Creds [NUM_WIFI_CREDENTIALS];
} WM_Config;

#define  CONFIG_FILENAME              F("/wifi_cred.dat")

// Use false if you don't like to display Available Pages in Information Page of Config Portal
// Comment out or use true to display Available Pages in Information Page of Config Portal
// Must be placed before #include <ESP_WiFiManager.h>
#define USE_AVAILABLE_PAGES     true

// From v1.0.10 to permit disable/enable StaticIP configuration in Config Portal from sketch. Valid only if DHCP is used.
// You'll loose the feature of dynamically changing from DHCP to static IP, or vice versa
// You have to explicitly specify false to disable the feature.
//#define USE_STATIC_IP_CONFIG_IN_CP          false

// Use false to disable NTP config. Advisable when using Cellphone, Tablet to access Config Portal.
// See Issue 23: On Android phone ConfigPortal is unresponsive (https://github.com/khoih-prog/ESP_WiFiManager/issues/23)
#define USE_ESP_WIFIMANAGER_NTP     true

// Use true to enable CloudFlare NTP service. System can hang if you don't have Internet access while accessing CloudFlare
// See Issue #21: CloudFlare link in the default portal (https://github.com/khoih-prog/ESP_WiFiManager/issues/21)
#define USE_CLOUDFLARE_NTP          false

// New in v1.0.11
//#define USING_CORS_FEATURE          true
//////

// Use USE_DHCP_IP == true for dynamic DHCP IP, false to use static IP which you have to change accordingly to your network
#if (defined(USE_STATIC_IP_CONFIG_IN_CP) && !USE_STATIC_IP_CONFIG_IN_CP)
// Force DHCP to be true
  #if defined(USE_DHCP_IP)
    #undef USE_DHCP_IP
  #endif
  #define USE_DHCP_IP     true
#else
  // You can select DHCP or Static IP here
  #define USE_DHCP_IP     true
  //#define USE_DHCP_IP     false
#endif
#define USE_CONFIGURABLE_DNS      true

#include <ESPAsync_WiFiManager.h>              //https://github.com/khoih-prog/ESPAsync_WiFiManager

#define HTTP_PORT           80


class WifiManager {
    public:
        WifiManager();
        uint8_t connectMultiWiFi(void);
        void heartBeatPrint(void);
        void check_WiFi(void);
        void check_status(void);
        void loadConfigData(void);
        void saveConfigData(void);
        void setup();
        void loop();
    protected:
        WiFiMulti wifiMulti;
        AsyncWebServer * webServer;
        DNSServer dnsServer;
        fs::FS* filesystem =      &SPIFFS;
        String Router_SSID;
        String Router_Pass;
        WM_Config         WM_config;
        bool initialConfig = false;
        IPAddress stationIP   = IPAddress(0, 0, 0, 0);
        IPAddress gatewayIP   = IPAddress(192, 168, 2, 1);
        IPAddress netMask     = IPAddress(255, 255, 255, 0);
        IPAddress dns1IP      = gatewayIP;
        IPAddress dns2IP      = IPAddress(8, 8, 8, 8);
};

#endif // __WIFIMANAGER_H__
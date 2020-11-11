#include "WifiManager.hpp"
#include "ESPAsyncWebServer.h"
#include "Settings.hpp"
#include "GlobalParams.hpp"
#include "Log.hpp"
#include <Arduino.h>
#include <ESPmDNS.h>
#include "i18n.h"
#include "WebServer.hpp"
WifiManager wifiManager;

WifiManager::WifiManager(/* args */) {
}

WifiManager::~WifiManager() {

}

//#define WIFI_FORCE_RF_CAL_ERASE            // Erase rf calibration sector on restart only
//#define WIFI_RF_MODE_RF_CAL                // Set RF_MODE to RF_CAL for restart and deepsleep during user_rf_pre_init
//#define WIFI_RF_PRE_INIT                   // Set RF_MODE to RF_CAL for restart, deepsleep and power on during user_rf_pre_init

#ifndef WIFI_RSSI_THRESHOLD
#define WIFI_RSSI_THRESHOLD     10         // Difference in dB between current network and scanned network
#endif
#ifndef WIFI_RESCAN_MINUTES
#define WIFI_RESCAN_MINUTES     44         // Number of minutes between wifi network rescan
#endif

const uint8_t WIFI_CONFIG_SEC = 180;       // seconds before restart
const uint8_t WIFI_CHECK_SEC = 20;         // seconds
const uint8_t WIFI_RETRY_OFFSET_SEC = 12;  // seconds

#include <WiFi.h>                   // Wifi, MQTT, Ota, WifiManager
// #if LWIP_IPV6
// #include <IPv6Address.h>
// #include <Arduino/>                      // IPv6 DualStack
// #endif  // LWIP_IPV6=1

int WifiManager::getRssiAsQuality(int rssi) {
  int quality = 0;
  if (rssi <= -100) {
    quality = 0;
  } else if (rssi >= -50) {
    quality = 100;
  } else {
    quality = 2 * (rssi + 100);
  }
  return quality;
}

bool WifiManager::getConfigCounter() {
  if (this->configCounter) {
    this->configCounter = WIFI_CONFIG_SEC;
  }
  return (this->configCounter);
}

void WifiManager::config(uint8_t type) {
  if (!this->configType) {
    if ((WIFI_RETRY == type) || (WIFI_WAIT == type)) { return; }
    WiFi.disconnect();                       // Solve possible Wifi hangs
    this->configType = type;
    this->configCounter = WIFI_CONFIG_SEC;   // Allow up to WIFI_CONFIG_SECS seconds for phone to provide ssid/pswd
    this->counter = this->configCounter +5;
    //TasmotaGlobal.blinks = 255;
    if (WIFI_RESTART == settings.getWifiConfigOption()) {
      globalParams.setRestartFlag(2);
    }
#ifdef USE_WEBSERVER
    else if (WIFI_MANAGER == this->configType || WIFI_MANAGER_RESET_ONLY == this->configType) {
      Log::log(LOG_LEVEL_INFO, S_LOG_WIFI, D_WCFG_2_WIFIMANAGER " " D_ACTIVE_FOR_3_MINUTES);
      WifiManagerBegin(WIFI_MANAGER_RESET_ONLY == this->configType);
    }
#endif  // USE_WEBSERVER
  }
}

void WifiManager::setMode(WiFiMode_t wifi_mode) {
  if (WiFi.getMode() == wifi_mode) { return; }

//   if (wifi_mode != WIFI_OFF) {
//     // See: https://github.com/esp8266/Arduino/issues/6172#issuecomment-500457407
//     WiFi.forceSleepWake(); // Make sure WiFi is really active.
//     delay(100);
//   }

  uint32_t retry = 2;
  while (!WiFi.mode(wifi_mode) && retry--) {
    Log::log(LOG_LEVEL_INFO, D_LOG_WIFI "Retry set Mode...");
    delay(100);
  }

  if (wifi_mode == WIFI_OFF) {
    delay(1000);
    WiFi.setSleep(1);
    delay(1);
  } else {
    delay(30); // Must allow for some time to init.
  }
}

void WifiManager::begin(uint8_t flag, uint8_t channel) {
  WiFi.persistent(false);   // Solve possible wifi init errors (re-add at 6.2.1.16 #4044, #4083)
  WiFi.disconnect(true);    // Delete SDK wifi config
  delay(200);
  this->setMode(WIFI_STA);
  if (!WiFi.getAutoConnect()) { WiFi.setAutoConnect(true); }
  switch (flag) {
    case 0:  // AP1
    case 1:  // AP2
        settings.setActiveSta(flag);
        break;
    case 2:  // Toggle
        settings.setActiveSta(!settings.getActiveSta());
  }        // 3: Current AP
  if (settings.getActiveSta() == 0 && strlen(settings.getStaWifiSsid1()) == 0) {
      settings.setActiveSta(1);
  }
  if (settings.isStaIpAddressValid()) {
    WiFi.config(settings.getStaIpAddress(), settings.getStaGatewayIpAddress(),
        settings.getStaSubnetIpAddress(), settings.getStaDnsIpAddress());  // Set static IP
  }
  WiFi.setHostname(settings.getHostname());

  char stemp[40] = { 0 };
  // if (channel) {
  //   WiFi.begin(settings.getStaWifiActiveSsid(), settings.getStaWifiActivePassword(),channel, this->bssid, false);
  //   // Add connected BSSID and channel for multi-AP installations
  //   char hex_char[18];
  //   snprintf_P(stemp, sizeof(stemp), PSTR(" Channel %d BSSId %s"), channel, Support::toHex((unsigned char*)this->bssid, 6, hex_char, sizeof(hex_char), ':'));
  // } else {
       WiFi.begin(settings.getStaWifiActiveSsid(), settings.getStaWifiActivePassword());
  // }
  Log::log(LOG_LEVEL_INFO, D_LOG_WIFI D_CONNECTING_TO_AP "%d %s%s " D_AS " %s...",
    settings.getActiveSta() +1, settings.getStaWifiActiveSsid(), stemp, settings.getHostname());

// #if LWIP_IPV6
//   for (bool configured = false; !configured;) {
//     uint16_t cfgcnt = 0;
//     for (auto addr : addrList) {
//       if ((configured = !addr.isLocal() && addr.isV6()) || cfgcnt==30) {
//         Log::log(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "Got IPv6 global address %s"), addr.toString().c_str());
//         break;  // IPv6 is mandatory but stop after 15 seconds
//       }
//       delay(500);  // Loop until real IPv6 address is aquired or too many tries failed
//       cfgcnt++;
//     }
//   }
// #endif  // LWIP_IPV6=1
}

void WifiManager::beginAfterScan(void) {
  // Not active
  if (0 == this->scanState) { return; }
  // Init scan when not connected
  if (1 == this->scanState) {
    memset((void*) &(this->bssid), 0, sizeof(this->bssid));
    this->bestNetworkDb = -127;
    this->scanState = 3;
  }
  // Init scan when connected
  if (2 == this->scanState) {
    uint8_t* bssid = WiFi.BSSID();                  // Get current bssid
    memcpy((void*) &this->bssid, (void*) bssid, sizeof(this->bssid));
    this->bestNetworkDb = WiFi.RSSI();             // Get current rssi and add threshold
    if (this->bestNetworkDb < -WIFI_RSSI_THRESHOLD) {
      this->bestNetworkDb += WIFI_RSSI_THRESHOLD;
    }
    this->scanState = 3;
  }
  // Init scan
  if (3 == this->scanState) {
    if (WiFi.scanComplete() != WIFI_SCAN_RUNNING) {
      WiFi.scanNetworks(true);                      // Start wifi scan async
      this->scanState++;
      Log::log(LOG_LEVEL_DEBUG, D_LOG_WIFI "Network (re)scan started...");
      return;
    }
  }
  int8_t wifi_scan_result = WiFi.scanComplete();
  // Check scan done
  if (4 == this->scanState) {
    if (wifi_scan_result != WIFI_SCAN_RUNNING) {
      this->scanState++;
    }
  }
  // Scan done
  if (5 == this->scanState) {
    int32_t channel = 0;                            // No scan result
    int8_t ap = 3;                                  // AP default if not found
    uint8_t last_bssid[6];                          // Save last bssid
    memcpy((void*) &last_bssid, (void*) &(this->bssid), sizeof(last_bssid));

    if (wifi_scan_result > 0) {
      // Networks found
      for (uint32_t i = 0; i < wifi_scan_result; ++i) {

        String ssid_scan;
        int32_t rssi_scan;
        uint8_t sec_scan;
        uint8_t* bssid_scan;
        int32_t chan_scan;

        WiFi.getNetworkInfo(i, ssid_scan, sec_scan, rssi_scan, bssid_scan, chan_scan);

        bool known = false;
        uint32_t j;
        for (j = 0; j < 2; j++) {
          if (ssid_scan == String(settings.getStaWifiSsid(j))) {  // SSID match
            known = true;
            if (rssi_scan > this->bestNetworkDb) {      // Best network
              if (String(settings.getStaWifiPassword(j))) {  // Check for passphrase if not open wlan
                this->bestNetworkDb = (int8_t)rssi_scan;
                channel = chan_scan;
                ap = j;                             // AP1 or AP2
                memcpy((void*) &(this->bssid), (void*) bssid_scan, sizeof(this->bssid));
              }
            }
            break;
          }
        }
        char hex_char[18];
        Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI "Network %d, AP%c, SSId %s, Channel %d, BSSId %s, RSSI %d"),
          i,
          (known) ? (j) ? '2' : '1' : '-',
          ssid_scan.c_str(),
          chan_scan,
          Support::toHex((unsigned char*)bssid_scan, 6, hex_char, sizeof(hex_char), ':'),
          rssi_scan);
        delay(0);
      }
      WiFi.scanDelete();                            // Clean up Ram
      delay(0);
    }
    this->scanState = 0;
    // If bssid changed then (re)connect wifi
    for (uint32_t i = 0; i < sizeof(this->bssid); i++) {
      if (last_bssid[i] != this->bssid[i]) {
        this->begin(ap, channel);                     // 0 (AP1), 1 (AP2) or 3 (default AP)
        break;
      }
    }
  }
}

uint16_t WifiManager::getLinkCount(void) {
  return this->linkCount;
}

String WifiManager::getDowntime(void) {
  return Support::getDuration(this->downtime);
}

void WifiManager::setState(uint8_t state) {
  if (state == globalParams.isWifiDown()) {
    if (state) {
        globalParams.setWifiConnected(1);
        this->linkCount++;
      this->downtime += Support::getUpTime() - this->lastEvent;
    } else {
        globalParams.setWifiConnected(0);
        this->lastEvent = Support::getUpTime();
    }
  }
  globalParams.setWifiEnabled(state);
}

// #if LWIP_IPV6
// bool WifiManager::checkIPv6(void) {
//   bool ipv6_global=false;

//   for (auto a : addrList) {
//     if(!a.isLocal() && a.isV6()) ipv6_global=true;
//   }
//   return ipv6_global;
// }

// String WifiManager::getIPv6(void) {
//   for (auto a : addrList) {
//     if(!a.isLocal() && a.isV6()) return a.toString();
//   }
//   return "";
// }

// // Return false for 169.254.x.x or fe80::/64
// bool  WifiManager::checkIPAddrStatus(void) {
//   bool ip_global=false;

//   for (auto a : addrList) {
//     if(!a.isLocal()) ip_global=true;
//   }
//   return ip_global;
// }
// #endif  // LWIP_IPV6=1

void WifiManager::checkIp(void) {
// #if LWIP_IPV6
//   if(checkIPAddrStatus()) {
//     this->status = WL_CONNECTED;
// #else
  if ((WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
// #endif  // LWIP_IPV6=1
    setState(1);
    this->counter = WIFI_CHECK_SEC;
    this->retry = this->retryInit;
    if (this->status != WL_CONNECTED) {
      Log::log(LOG_LEVEL_INFO, D_LOG_WIFI D_CONNECTED);
//      Log::log(LOG_LEVEL_INFO, PSTR("Wifi: Set IP addresses"));
      settings.setStaGatewayIpAddress((uint32_t)WiFi.gatewayIP());
      settings.setStaSubnetIpAddress((uint32_t)WiFi.subnetMask());
      settings.setStaDnsIpAddress((uint32_t)WiFi.dnsIP());

      // Save current AP parameters for quick reconnect
      settings.setWifiChannel(WiFi.channel());
      settings.setWifiBssid(WiFi.BSSID());
    }
    this->status = WL_CONNECTED;
#ifdef USE_DISCOVERY
#ifdef WEBSERVER_ADVERTISE
    //MdnsUpdate();
#endif  // USE_DISCOVERY
#endif  // WEBSERVER_ADVERTISE
  } else {
    setState(0);
    uint8_t wifiConfigTool = settings.getWifiConfigOption();
    this->status = WiFi.status();
    switch (this->status) {
      case WL_CONNECTED:
        Log::log(LOG_LEVEL_INFO, D_LOG_WIFI D_CONNECT_FAILED_NO_IP_ADDRESS);
        this->status = 0;
        this->retry = this->retryInit;
        break;
      case WL_NO_SSID_AVAIL:
        Log::log(LOG_LEVEL_INFO, D_LOG_WIFI D_CONNECT_FAILED_AP_NOT_REACHED);
        settings.setWifiChannel(0);
        if (WIFI_WAIT == settings.getWifiConfigOption()) {
          this->retry = this->retryInit;
        } else {
          if (this->retry > (this->retryInit / 2)) {
            this->retry = this->retryInit / 2;
          }
          else if (this->retry) {
            this->retry = 0;
          }
        }
        break;
      case WL_CONNECT_FAILED:
        Log::log(LOG_LEVEL_INFO, D_LOG_WIFI D_CONNECT_FAILED_WRONG_PASSWORD);
        settings.setWifiChannel(0);
        if (this->retry > (this->retryInit / 2)) {
          this->retry = this->retryInit / 2;
        }
        else if (this->retry) {
          this->retry = 0;
        }
        break;
      default:  // WL_IDLE_STATUS and WL_DISCONNECTED
        if (!this->retry || ((this->retryInit / 2) == this->retry)) {
          Log::log(LOG_LEVEL_INFO, D_LOG_WIFI D_CONNECT_FAILED_AP_TIMEOUT);
          settings.setWifiChannel(0);
        } else {
            if(!strlen(settings.getStaWifiSsid1()) && !strlen(settings.getStaWifiSsid2())) {
                settings.setWifiChannel(0);
            wifiConfigTool = WIFI_MANAGER;
            this->retry = 0;
          } else {
            Log::log(LOG_LEVEL_DEBUG, D_LOG_WIFI D_ATTEMPTING_CONNECTION);
          }
        }
    }
    if (this->retry) {
        if (settings.getUseWifiScan()) {
            if (this->retryInit == this->retry) {
              this->scanState = 1;    // Select scanned SSID
            }
        } else {
            if (this->retryInit == this->retry) {
                this->begin(3, settings.getWifiChannel());  // Select default SSID
            }
            if ((settings.getWifiConfigOption() != WIFI_WAIT) && ((this->retryInit / 2) == this->retry)) {
                this->begin(2, 0);        // Select alternate SSID
            }
        }
      this->counter = 1;
      this->retry--;
    } else {
      this->config(wifiConfigTool);
      this->counter = 1;
      this->retry = this->retryInit;
    }
  }
}

void WifiManager::check(uint8_t param) {
  this->counter--;
  switch (param) {
  case WIFI_MANAGER:
    this->config(param);
    break;
  default:
    if (this->configCounter) {
      this->configCounter--;
      this->counter = this->configCounter +5;
      if (this->configCounter) {
        if (!this->configCounter) {
          if (strlen(WiFi.SSID().c_str())) {
              settings.setStaWifiSsid1(WiFi.SSID().c_str());
          }
          if (strlen(WiFi.psk().c_str())) {
              settings.setStaWifiPassword1(WiFi.psk().c_str());
          }
          settings.setActiveSta(0);
          Log::log(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_WCFG_2_WIFIMANAGER D_CMND_SSID "1 %s"), settings.getStaWifiSsid1());
        }
      }
      if (!this->configCounter) {
          globalParams.setRestartFlag(2);
      }
    } else {
      if (this->scanState) {
        this->beginAfterScan();
      }
      if (this->counter <= 0) {
        Log::log(LOG_LEVEL_DEBUG_MORE, D_LOG_WIFI D_CHECKING_CONNECTION);
        this->counter = WIFI_CHECK_SEC;
        this->checkIp();
      }
// #if LWIP_IPV6
//       if (WifiCheckIPAddrStatus()) {
// #else
      if ((WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0) && !this->configType) {
// #endif  // LWIP_IPV6=1
        this->setState(1);
        if (settings.getUseWifiRescan()) { // SetOption57 - Scan wifi network every 44 minutes for configured AP's
          if (!(globalParams.getUptime() % (60 * WIFI_RESCAN_MINUTES))) {
            this->scanState = 2;
          }
        }
      } else {
        setState(0);
        //Mdns.begun = 0;
      }
    }
  }
}

int WifiManager::getState(void) {
    int state = -1;
    if (globalParams.isWifiUp()) { state = WIFI_RESTART; }
    if (this->configType) { state = this->configType; }
    return state;
}

String WifiManager::getOutputPower(void) {
    return String((float)settings.getWifiOutputPower()/10);
//   char stemp1[TOPSZ];
//   dtostrfd((float)(Settings.wifi_output_power) / 10, 1, stemp1);
//   return String(stemp1);
}

void WifiManager::setOutputPower(void) {
    WiFi.setTxPower(WIFI_POWER_19_5dBm);
    //settings.getWifiOutputPower());
}

/*
  See Esp.h, core_esp8266_phy.cpp and test_overrides.ino
  RF_DEFAULT = 0,  // RF_CAL or not after deep-sleep wake up, depends on init data byte 108.
  RF_CAL = 1,      // RF_CAL after deep-sleep wake up, there will be large current.
  RF_NO_CAL = 2,   // no RF_CAL after deep-sleep wake up, there will only be small current.
  RF_DISABLED = 4  // disable RF after deep-sleep wake up, just like modem sleep, there will be the smallest current.
*/
#ifdef WIFI_RF_MODE_RF_CAL
#ifndef USE_DEEPSLEEP
RF_MODE(RF_CAL);
#endif  // USE_DEEPSLEEP
#endif  // WIFI_RF_MODE_RF_CAL

#ifdef WIFI_RF_PRE_INIT
bool rf_pre_init_flag = false;
RF_PRE_INIT()
{
#ifndef USE_DEEPSLEEP
  system_deep_sleep_set_option(1);   // The option is 1 by default.
  system_phy_set_rfoption(RF_CAL);
#endif  // USE_DEEPSLEEP
  system_phy_set_powerup_option(3);  // 3: RF initialization will do the whole RF calibration which will take about 200ms; this increases the current consumption.
  rf_pre_init_flag = true;
}
#endif  // WIFI_RF_PRE_INIT

void WifiManager::connect(void) {
  //if (!Settings.flag4.network_wifi) { return; }
  this->setState(0);
  this->setOutputPower();
  WiFi.persistent(false);     // Solve possible wifi init errors
  this->status = 0;
  this->retryInit = WIFI_RETRY_OFFSET_SEC + (((uint32_t)ESP.getEfuseMac()) & 0xF);  // Add extra delay to stop overrun by simultanous re-connects
  this->retry = this->retryInit;
  this->counter = 1;

  memcpy((void*) &this->bssid, (void*) settings.getWifiBssid(), sizeof(this->bssid));

#ifdef WIFI_RF_PRE_INIT
  if (rf_pre_init_flag) {
    Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI "Pre-init done"));
  }
#endif  // WIFI_RF_PRE_INIT
}

void WifiManager::shutdown(bool option) {
  // option = false - Legacy disconnect also used by DeepSleep
  // option = true  - Disconnect with SDK wifi calibrate sector erase when WIFI_FORCE_RF_CAL_ERASE enabled
  delay(100);                 // Allow time for message xfer - disabled v6.1.0b

  //Disconnect from home Assistant

#ifdef WIFI_FORCE_RF_CAL_ERASE
  if (option) {
    WiFi.disconnect(false);   // Disconnect wifi
    SettingsErase(4);         // Delete SDK wifi config and calibrate data
  } else
#endif  // WIFI_FORCE_RF_CAL_ERASE
  {
    // Enable from 6.0.0a until 6.1.0a - disabled due to possible cause of bad wifi connect on core 2.3.0
    // Re-enabled from 6.3.0.7 with ESP.restart replaced by ESP.reset
    // Courtesy of EspEasy
    // WiFi.persistent(true);    // use SDK storage of SSID/WPA parameters
    // ETS_UART_INTR_DISABLE();
    // wifi_station_disconnect();  // this will store empty ssid/wpa into sdk storage
    // ETS_UART_INTR_ENABLE();
    // WiFi.persistent(false);   // Do not use SDK storage of SSID/WPA parameters
  }
  delay(100);                 // Flush anything in the network buffers.
}

//
// Gratuitous ARP, backported from https://github.com/esp8266/Arduino/pull/6889
//
// extern "C" {
// #if LWIP_VERSION_MAJOR == 1
// #include "netif/wlan_lwip_if.h" // eagle_lwip_getif()
// #include "netif/etharp.h" // gratuitous arp
// #else
// #include "lwip/etharp.h" // gratuitous arp
// #endif
// }

// void WifiManager::stationKeepAliveNow(void) {
//   Log::log(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_WIFI "Sending Gratuitous ARP"));
//   for (netif* interface = netif_list; interface != nullptr; interface = interface->next)
//     if (
//           (interface->flags & NETIF_FLAG_LINK_UP)
//       && (interface->flags & NETIF_FLAG_UP)
// #if LWIP_VERSION_MAJOR == 1
//       && interface == eagle_lwip_getif(0) /* lwip1 does not set if->num properly */
//       && (!ip_addr_isany(&interface->ip_addr))
// #else
//       && interface->num == 0
//       && (!ip4_addr_isany_val(*netif_ip4_addr(interface)))
// #endif
//   )
//   {
//     etharp_gratuitous(interface);
//     break;
//   }
// }

// void WifiManager::wifiKeepAlive(void) {
//   static uint32_t wifi_timer = 0;                            // Wifi keepalive timer

//   uint32_t wifiTimerSec = settings.getARPGratutious();  // 8-bits number of seconds, or minutes if > 100

//   if ((WL_CONNECTED != this->status) || (0 == wifiTimerSec)) { return; }   // quick exit if wifi not connected or feature disabled

//   if (Support::timeReached(wifi_timer)) {
//     stationKeepAliveNow();
//     if (wifiTimerSec > 100) {
//       wifiTimerSec = (wifiTimerSec - 100) * 60;              // convert >100 as minutes, ex: 105 = 5 minutes, 110 = 10 minutes
//     }
//     Support::setNextTimeInterval(wifi_timer, wifiTimerSec * 1000);
//   }
// }
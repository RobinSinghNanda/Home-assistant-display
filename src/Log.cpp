#include "Log.hpp"
#include <Arduino.h>
#include "Settings.hpp"
#include "RtcTime.hpp"
#include "GlobalParams.hpp"
#include "GlobalDefines.hpp"

char data[MAX_LOG_LENGTH];
char webLog[WEB_LOG_SIZE];

Log::Log(/* args */) {
}

Log::~Log() {
}

void Log::addLog(LogLevel loglevel, PGM_P formatP, ...) {
  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(data, sizeof(data), formatP, arg);
  va_end(arg);
}

void Log::log(LogLevel loglevel, PGM_P formatP, ...) {
  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(data, sizeof(data), formatP, arg);
  va_end(arg);

  Log::log(loglevel);
}

void Log::log(LogLevel loglevel) {
    char mxtime[10];
    snprintf(mxtime, sizeof(mxtime), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d "), rtcTime.hour, rtcTime.minute, rtcTime.second);
    if ((loglevel <= settings.getSerialLogLevel()) &&
        (settings.getMasterLogLevel() <= settings.getSerialLogLevel())) {
        Serial.printf("%s%s\r\n", mxtime, data);
    }
#ifdef USE_WEBSERVER
  if ((loglevel <= settings.getWebLogLevel()) &&
     (settings.getMasterLogLevel() <= settings.getWebLogLevel())) {
    // Delimited, zero-terminated buffer of log lines.
    // Each entry has this format: [index][log data]['\1']
    globalParams.setWebLogIndex(globalParams.getWebLogIndex() & 0xFF);
    if (!globalParams.getWebLogIndex()) {
      globalParams.incrWebLogIndex();// Index 0 is not allowed as it is the end of char string
    }
    while (globalParams.getWebLogIndex() == webLog[0] ||  // If log already holds the next index, remove it
           strlen(webLog) + strlen(data) + 13 > WEB_LOG_SIZE)  // 13 = web_log_index + mxtime + '\1' + '\0'
    {
      char* it = webLog;
      it++;                                // Skip web_log_index
      it += Support::strchrspn(it, '\1');           // Skip log line
      it++;                                // Skip delimiting "\1"
      memmove(webLog, it, WEB_LOG_SIZE -(it-webLog));  // Move buffer forward to remove oldest log line
    }
    snprintf_P(webLog, sizeof(webLog), PSTR("%s%c%s%s\1"), webLog, globalParams.getWebLogIndex(), mxtime, data);
    globalParams.incrWebLogIndex();
    globalParams.setWebLogIndex(globalParams.getWebLogIndex() & 0xFF);
    if (!globalParams.getWebLogIndex()) {
      globalParams.incrWebLogIndex();// Index 0 is not allowed as it is the end of char string
    }
  }
#endif  // USE_WEBSERVER

//   if (!TasmotaGlobal.global_state.network_down &&
//       (loglevel <= TasmotaGlobal.syslog_level) &&
//       (TasmotaGlobal.masterlog_level <= TasmotaGlobal.syslog_level)) { Syslog(); }

//   TasmotaGlobal.prepped_loglevel = 0;
}

// #ifdef USE_WEBSERVER
// void Log::getLog(uint32_t idx, char** entry_pp, size_t* len_p)
// {
//   char* entry_p = nullptr;
//   size_t len = 0;

//   if (idx) {
//     char* it = TasmotaGlobal.web_log;
//     do {
//       uint32_t cur_idx = *it;
//       it++;
//       size_t tmp = strchrspn(it, '\1');
//       tmp++;                             // Skip terminating '\1'
//       if (cur_idx == idx) {              // Found the requested entry
//         len = tmp;
//         entry_p = it;
//         break;
//       }
//       it += tmp;
//     } while (it < TasmotaGlobal.web_log + WEB_LOG_SIZE && *it != '\0');
//   }
//   *entry_pp = entry_p;
//   *len_p = len;
// }
// #endif  // USE_WEBSERVER

// void Syslog(void)
// {
//   // Destroys TasmotaGlobal.log_data

//   uint32_t current_hash = GetHash(SettingsText(SET_SYSLOG_HOST), strlen(SettingsText(SET_SYSLOG_HOST)));
//   if (syslog_host_hash != current_hash) {
//     syslog_host_hash = current_hash;
//     WiFi.hostByName(SettingsText(SET_SYSLOG_HOST), syslog_host_addr);  // If sleep enabled this might result in exception so try to do it once using hash
//   }
//   if (PortUdp.beginPacket(syslog_host_addr, Settings.syslog_port)) {
//     char syslog_preamble[64];  // Hostname + Id
//     snprintf_P(syslog_preamble, sizeof(syslog_preamble), PSTR("%s ESP-"), NetworkHostname());
//     memmove(TasmotaGlobal.log_data + strlen(syslog_preamble), TasmotaGlobal.log_data, sizeof(TasmotaGlobal.log_data) - strlen(syslog_preamble));
//     TasmotaGlobal.log_data[sizeof(TasmotaGlobal.log_data) -1] = '\0';
//     memcpy(TasmotaGlobal.log_data, syslog_preamble, strlen(syslog_preamble));
//     PortUdp_write(TasmotaGlobal.log_data, strlen(TasmotaGlobal.log_data));
//     PortUdp.endPacket();
//     delay(1);  // Add time for UDP handling (#5512)
//   } else {
//     TasmotaGlobal.syslog_level = 0;
//     TasmotaGlobal.syslog_timer = SYSLOG_TIMER;
//     AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SYSLOG_HOST_NOT_FOUND ". " D_RETRY_IN " %d " D_UNIT_SECOND), SYSLOG_TIMER);
//   }
// }

// void AddLog_P(uint32_t loglevel, const char *formatP)
// {
//   snprintf_P(TasmotaGlobal.log_data, sizeof(TasmotaGlobal.log_data), formatP);
//   AddLog(loglevel);
// }

// void AddLog_P(uint32_t loglevel, const char *formatP, const char *formatP2)
// {
//   char message[sizeof(TasmotaGlobal.log_data)];

//   snprintf_P(TasmotaGlobal.log_data, sizeof(TasmotaGlobal.log_data), formatP);
//   snprintf_P(message, sizeof(message), formatP2);
//   strncat(TasmotaGlobal.log_data, message, sizeof(TasmotaGlobal.log_data) - strlen(TasmotaGlobal.log_data) -1);
//   AddLog(loglevel);
// }

// void PrepLog_P2(uint32_t loglevel, PGM_P formatP, ...)
// {
//   va_list arg;
//   va_start(arg, formatP);
//   vsnprintf_P(TasmotaGlobal.log_data, sizeof(TasmotaGlobal.log_data), formatP, arg);
//   va_end(arg);

//   TasmotaGlobal.prepped_loglevel = loglevel;
// }

// void AddLog_P2(uint32_t loglevel, PGM_P formatP, ...)
// {
//   va_list arg;
//   va_start(arg, formatP);
//   vsnprintf_P(TasmotaGlobal.log_data, sizeof(TasmotaGlobal.log_data), formatP, arg);
//   va_end(arg);

//   AddLog(loglevel);
// }

// void AddLog_Debug(PGM_P formatP, ...)
// {
//   va_list arg;
//   va_start(arg, formatP);
//   vsnprintf_P(TasmotaGlobal.log_data, sizeof(TasmotaGlobal.log_data), formatP, arg);
//   va_end(arg);

//   AddLog(LOG_LEVEL_DEBUG);
// }

// void AddLogBuffer(uint32_t loglevel, uint8_t *buffer, uint32_t count)
// {
// /*
//   snprintf_P(TasmotaGlobal.log_data, sizeof(TasmotaGlobal.log_data), PSTR("DMP:"));
//   for (uint32_t i = 0; i < count; i++) {
//     snprintf_P(TasmotaGlobal.log_data, sizeof(TasmotaGlobal.log_data), PSTR("%s %02X"), TasmotaGlobal.log_data, *(buffer++));
//   }
//   AddLog(loglevel);
// */
// /*
//   strcpy_P(TasmotaGlobal.log_data, PSTR("DMP: "));
//   ToHex_P(buffer, count, TasmotaGlobal.log_data + strlen(TasmotaGlobal.log_data), sizeof(TasmotaGlobal.log_data) - strlen(TasmotaGlobal.log_data), ' ');
//   AddLog(loglevel);
// */
//   char hex_char[(count * 3) + 2];
//   AddLog_P2(loglevel, PSTR("DMP: %s"), ToHex_P(buffer, count, hex_char, sizeof(hex_char), ' '));
// }

// void AddLogSerial(uint32_t loglevel)
// {
//   AddLogBuffer(loglevel, (uint8_t*)TasmotaGlobal.serial_in_buffer, TasmotaGlobal.serial_in_byte_counter);
// }

// void AddLogMissed(const char *sensor, uint32_t misses)
// {
//   AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SNS: %s missed %d"), sensor, SENSOR_MAX_MISS - misses);
// }

// void AddLogBufferSize(uint32_t loglevel, uint8_t *buffer, uint32_t count, uint32_t size) {
//   snprintf_P(TasmotaGlobal.log_data, sizeof(TasmotaGlobal.log_data), PSTR("DMP:"));
//   for (uint32_t i = 0; i < count; i++) {
//     if (1 ==  size) {  // uint8_t
//       snprintf_P(TasmotaGlobal.log_data, sizeof(TasmotaGlobal.log_data), PSTR("%s %02X"), TasmotaGlobal.log_data, *(buffer));
//     } else {           // uint16_t
//       snprintf_P(TasmotaGlobal.log_data, sizeof(TasmotaGlobal.log_data), PSTR("%s %02X%02X"), TasmotaGlobal.log_data, *(buffer +1), *(buffer));
//     }
//     buffer += size;
//   }
//   AddLog(loglevel);
// }


void Log::getWebLog(uint32_t idx, char** entry_pp, size_t* len_p) {
  char* entry_p = nullptr;
  size_t len = 0;

  if (idx) {
    char* it = webLog;
    do {
      uint32_t cur_idx = *it;
      it++;
      size_t tmp = Support::strchrspn(it, '\1');
      tmp++;                             // Skip terminating '\1'
      if (cur_idx == idx) {              // Found the requested entry
        len = tmp;
        entry_p = it;
        break;
      }
      it += tmp;
    } while (it < webLog + WEB_LOG_SIZE && *it != '\0');
  }
  *entry_pp = entry_p;
  *len_p = len;
}
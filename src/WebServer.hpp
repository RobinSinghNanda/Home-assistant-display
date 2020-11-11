#ifndef WEB_SERVER_HPP_
#define WEB_SERVER_HPP_
#include <ESPAsyncWebServer.h>

void HandleNotFound(AsyncWebServerRequest *request);
void HandleRoot(AsyncWebServerRequest *request);
void HandleUpgradeFirmware(AsyncWebServerRequest * request);
void HandleUpgradeFirmwareStart(AsyncWebServerRequest * request);
void HandlePreflightRequest(AsyncWebServerRequest * request);
void HandleUploadDone(AsyncWebServerRequest * request);
void HandleConsole(AsyncWebServerRequest * request);
void HandlePreflightRequest(AsyncWebServerRequest * request);
void HandleHttpCommand(AsyncWebServerRequest * request);
#ifndef FIRMWARE_MINIMAL
void HandleConfiguration(AsyncWebServerRequest * request);
//void HandleModuleConfiguration(AsyncWebServerRequest * request);
void HandleWifiConfiguration(AsyncWebServerRequest * request);
void HandleCardsConfiguration (AsyncWebServerRequest *request);
void HandleLoggingConfiguration(AsyncWebServerRequest * request);
//void HandleTemplateConfiguration(AsyncWebServerRequest * request);
void HandleOtherConfiguration(AsyncWebServerRequest * request);
void HandleBackupConfiguration(AsyncWebServerRequest * request);
void HandleRestoreConfiguration(AsyncWebServerRequest * request);
void HandleResetConfiguration(AsyncWebServerRequest * request);
void HandleInformation(AsyncWebServerRequest * request);
#endif


void startwebServer(int type, IPAddress ipweb);
void requestAuthentication(AsyncWebServerRequest *request);
void WifiManagerBegin(bool reset_only);
bool CaptivePortal(AsyncWebServerRequest *request);
void WifiSaveSettings(AsyncWebServerRequest *request);
void CardsSaveSettings(AsyncWebServerRequest *request);
void LoggingSaveSettings(AsyncWebServerRequest *request);
void OtherSaveSettings(AsyncWebServerRequest *request);
void HandleConsoleRefresh(AsyncWebServerRequest *request);
void HandleUploadLoop(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final);
void Webserver_loop();

#endif
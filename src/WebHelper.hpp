#ifndef WEB_HELPER_H_
#define WEB_HELPER_H_
#include "Arduino.h"
#include <unishox.h>



String Decompress(const char * compressed, size_t uncompressed_size);
String HtmlEscape(const String unescaped);
char* GetTextIndexed(char* destination, size_t destination_size, uint32_t index, const char* haystack);
int WifiGetRssiAsQuality(int rssi);
#endif // WEB_HELPER_H_
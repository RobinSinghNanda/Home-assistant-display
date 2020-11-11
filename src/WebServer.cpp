#include "WebServer.hpp"
#include <Arduino.h>
#include "language/en_GB.h"
#include "i18n.h"
#include "Config.hpp"
#include "Log.hpp"
#include "ESPAsyncWebServer.h"
#include "GlobalParams.hpp"
#define FS_NO_GLOBALS
#include <FS.h>
#include "SPIFFS.h"
#include <Update.h>

#define USE_webServer
#ifdef USE_webServer

#ifndef WIFI_SOFT_AP_CHANNEL
#define WIFI_SOFT_AP_CHANNEL                  1          // Soft Access Point Channel number between 1 and 11 as used by WifiManager web GUI
#endif

#define STR_HELPER(x) #x
#ifndef STR
#define STR(x) STR_HELPER(x)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

//const uint16_t CHUNKED_BUFFER_SIZE = (MESSZ / 2) - 100;  // Chunk buffer size (should be smaller than half mqtt_data size = MESSZ)
const uint16_t CHUNKED_BUFFER_SIZE = 600;

#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

enum UploadTypes { UPL_TASMOTA, UPL_SETTINGS, UPL_EFM8BB1, UPL_TASMOTACLIENT, UPL_EFR32 };

const char WEB_USER_REALM[] = "Users";

#ifdef USE_UNISHOX_COMPRESSION
#ifdef USE_JAVASCRIPT_ES6
// insert D_HTML_LANGUAGE later
const size_t HTTP_HEADER1_SIZE = 377;
const char HTTP_HEADER1_COMPRESSED[] PROGMEM = "\x3D\x0F\xE1\x10\x98\x1D\x19\x0C\x64\x85\x50\xD0\x8F\xC3\xD0\x55\x0D\x09\x05\x7C"
                             "\x3C\x7C\x3F\xB2\xEC\xD7\xE6\x86\x7D\x78\xFE\xCB\xB3\x5F\x9A\x1A\x0C\x2B\xF7\x8F"
                             "\x87\xB0\xF6\x1F\x87\xA0\xA7\x62\x1F\x87\xA0\xD7\x56\x83\x15\x7F\xF3\xA3\xE1\xF6"
                             "\x2E\x8C\x1D\x67\x3E\x7D\x90\x21\x52\xEB\x1A\xCF\x87\xB0\xCF\x58\xF8\xCC\xFD\x1E"
                             "\xC4\x1E\x75\x3E\xA3\xE1\xEC\x1F\xD1\x28\x51\xF0\x46\x67\xA1\xB3\xAC\x7F\x44\xA1"
                             "\x47\x56\xF6\xD6\xD8\x47\x5F\x83\xB0\x99\xF0\xE4\x3A\x88\x5F\x9F\xCE\xBF\x07\x61"
                             "\x58\xE0\x99\xF3\xB0\xF6\x1D\x87\xE1\xE9\x5B\x41\x33\xF0\xFA\xF2\x3A\xD1\xF5\xE3"
                             "\xD0\xEC\x04\x19\x67\xA7\x83\xFE\x8C\xA3\xF0\xCE\xFE\x8D\x87\xCE\x16\x10\x47\x50"
                             "\x54\x75\x56\x1D\x54\x30\xEA\x18\x19\xF0\xFB\x3E\xCF\x0C\x71\xF3\xC7\xC3\xF0\x4C"
                             "\x0C\x58\xD7\xD4\x74\x1E\x74\x4C\x26\x35\xF5\x10\xE3\x22\xD1\x0E\xEF\x8E\xF1\xE0"
                             "\xD5\xE0\x48\xBA\x6A\x16\xFE\x64\x5E\x61\x30\xEB\x3E\x77\x7C\x77\x8F\x1E\x18\x7C"
                             "\xD3\xE1\xF8\xC7\x1D\xDD\x3B\xC7\x4A\x32\x18\xCF\x87\x74\x11\xA4\x1F\x0F\x87\xDD"
                             "\x33\x65\x1F\x67\x68\xFB\x19\x7E\xF0\xFE\x7C\x43\xEC\xF3\x04\x19\xC7\x78\xF0\x3E"
                             "\x11\xF0\xC1\xF0\xFC\x1F\xDE\x13\x07\xCE\x96\x20\x84\xCC\xDF\x51\x05\xBE\xA7\xCF"
                             "\xE7\x74\xFB\x0B\x2C\x43\xEC\xEA\x30\x77\x8F\x06";
#define  HTTP_HEADER1       Decompress(HTTP_HEADER1_COMPRESSED,HTTP_HEADER1_SIZE).c_str()
#else
const size_t HTTP_HEADER1_SIZE = 431;
const char HTTP_HEADER1_COMPRESSED[] PROGMEM = "\x3D\x0F\xE1\x10\x98\x1D\x19\x0C\x64\x85\x50\xD0\x8F\xC3\xD0\x55\x0D\x09\x05\x7C"
                             "\x3C\x7C\x3F\xB2\xEC\xD7\xE6\x86\x7D\x78\xFE\xCB\xB3\x5F\x9A\x1A\x0C\x2B\xF7\x8F"
                             "\x87\xB0\xF6\x1F\x87\xA0\xA7\x62\x1F\x87\xA0\xD7\x56\x83\x15\x7F\xF3\xA3\xE1\xF6"
                             "\x2E\x8C\x1D\x67\x3E\x7D\x90\x21\x52\xEB\x1A\xCF\x87\xB0\xCF\x58\xF8\xCC\xFD\x1E"
                             "\xC4\x1E\x75\x3E\xA3\xE1\xEC\x1F\xD1\x28\x51\xF0\x46\x67\xA1\xB3\xAC\x7F\x44\xA1"
                             "\x47\x56\xF6\xD6\xD8\x47\x5F\x83\xB0\x99\xF0\xE4\x3A\x88\x5F\x9F\xCE\xBF\x07\x61"
                             "\x58\xE0\x99\xF3\xB0\xF6\x1D\x87\xE1\xE9\x5B\x41\x33\xF0\xFA\xF2\x3A\xD1\xF5\xE3"
                             "\xD0\xEC\x04\x19\x67\xA7\x83\xFE\x8C\xA3\xF0\xCE\xFE\x8D\x87\xCE\x16\x10\x47\x50"
                             "\x54\x75\x56\x1D\x54\x30\xEA\x18\x19\xF0\xFB\x3E\xCF\x06\x05\xF0\x75\xB9\xC9\x8E"
                             "\x3B\xBE\x3B\xC7\xB7\xEE\x85\xFF\x90\x98\x18\xB1\xAF\xA8\xE8\x3C\xE8\x98\x4C\x6B"
                             "\xEA\x21\xC6\x45\xA2\x1D\xDF\x1D\xE3\xC1\xEE\x04\x4C\x38\xD5\xE0\x4F\xC3\x8D\x42"
                             "\xDF\xCC\x8B\xCC\x26\x1D\x67\xC1\x27\x0D\xF0\xC3\xBB\xA7\x78\xF6\xB1\xC7\x77\x4E"
                             "\xF1\xD2\x8C\x86\x33\xE1\xDD\x04\x69\x07\xC3\xE1\xF7\x4C\xD9\x47\xD9\xDA\x3E\xC6"
                             "\x5F\xBC\x3F\x9F\x10\xFB\x3C\xC1\x06\x70\x23\xE3\xE3\xE1\x1D\xD3\x07\x78\xF6\x8F"
                             "\xEF\x09\x83\xE7\x4B\x10\x42\x66\x6F\xA8\x82\xDF\x53\xE7\xF3\xBA\x7D\x85\x96\x21"
                             "\xF6\x75\x18\x3B\xC7\x83\xDC";
#define  HTTP_HEADER1       Decompress(HTTP_HEADER1_COMPRESSED,HTTP_HEADER1_SIZE).c_str()
#endif // USE_JAVASCRIPT_ES6
#else
const char HTTP_HEADER1[] PROGMEM =
  "<!DOCTYPE html><html lang=\"%s\" class=\"\">"
  "<head>"
  "<meta charset='utf-8'>"
  "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1,user-scalable=no\"/>"
  "<title>%s - %s</title>"

  "<script>"
  "var x=null,lt,to,tp,pc='';"            // x=null allow for abortion

#ifdef USE_JAVASCRIPT_ES6
// Following bytes saving ES6 syntax fails on old browsers like IE 11 - https://kangax.github.io/compat-table/es6/
  "eb=s=>document.getElementById(s);"     // Alias to save code space
  "qs=s=>document.querySelector(s);"      // Alias to save code space
  "sp=i=>eb(i).type=(eb(i).type==='text'?'password':'text');"  // Toggle password visibility
  "wl=f=>window.addEventListener('load',f);" // Execute multiple window.onload
  ;
#else
  "function eb(s){"
    "return document.getElementById(s);"  // Alias to save code space
  "}"
  "function qs(s){"                       // Alias to save code space
    "return document.querySelector(s);"
  "}"
  "function sp(i){"                       // Toggle password visibility
    "eb(i).type=(eb(i).type==='text'?'password':'text');"
  "}"
  "function wl(f){"                       // Execute multiple window.onload
    "window.addEventListener('load',f);"
  "}";
#endif // USE_JAVASCRIPT_ES6
#endif //USE_UNISHOX_COMPRESSION

const char HTTP_SCRIPT_COUNTER[] PROGMEM =
  "var cn=180;"                           // seconds
  "function u(){"
    "if(cn>=0){"
      "eb('t').innerHTML='" D_RESTART_IN " '+cn+' " D_SECONDS "';"
      "cn--;"
      "setTimeout(u,1000);"
    "}"
  "}"
  "wl(u);";

const char HTTP_STYLE_CARDS[] PROGMEM =
  "#ed{"
     "width:100%;"
     "height:320px;"
     "border-radius: 0 0 0.3rem 0.3rem;"
  "}"
  ".tabb{"
     "margin:auto;"
  "}"
  ".iwrap{"
     "width:40px;"
     "height:40px;"
     "display:inline-block;"
     "text-align:center;"
  "}"
  ".ic{"
     "font-size:24px;"
     "margin:auto;"
     "line-height:40px;"
  "}"
  ".row{"
     "height:40px;"
  "}"
  "#header{"
     "height:40px;"
     "width:auto;"
  "}"
  ".text{"
     "font-family:'Roboto',sans-serif;"
     "font-size:24px;"
     "user-select:none;"
  "}"
  ".screen{"
     "height:240px;"
     "width:320px;"
     "background-color:#000000;"
     "display:grid;"
     "margin:auto;"
     "margin-top:20px;"
     "padding: 10px;"
     "border-radius: 0.3rem;"
  "}"
  ".screen div{"
    "padding:0px;"
  "}"
  ".body{"
     "margin:0px;"
     "height:200px;"
  "}"
  ".ecard{"
     "height:200px;"
     "width:100%%;"
     "color:red;"
     "text-align:center;"
  "}"
  ".erow{"
     "height:40px;"
     "width:100%%;"
     "color:red;"
     "text-align:left;"
     "font-size:12px;"
     "margin:0px;"
  "}"
  ".erow p{"
     "vertical-align:middle;"
     "display:inline-block;"
  "}"
  ".sw{"
     "margin:auto;"
     "position:absolute;"
     "top:0;"
     "bottom:0;"
     "left:0;"
     "right:0;"
     "user-select:none;"
  "}"
  ".sww{"
     "height:40px;"
     "float:right;"
     "width:60px;"
     "position:relative;"
  "}"
  ".btnG label{"
     "padding:5px 13px;"
     "cursor:pointer;"
     "color:#757575;"
     "transition:all 0.2s;"
     "font-size:24px;"
  "}"
  ".btn:hover{"
     "background-color:#DDDDDD;"
  "}"
  ".btn{"
     "height:40px;"
     "width:40px;"
     "display:inline-block;"
     "text-align:center;"
  "}"
  ".btnG label:hover{"
     "background-color:#DDDDDD;"
  "}"
  "input[name='cS']{"
     "display:none;"
  "}"
  "input[name='cS']:checked+label{"
     "border-bottom:#5dbcd2 solid 2px;"
  "}"
  ".navbar{"
     "width:100%;"
     "margin:auto;"
     "background-color: white;"
     "padding:0px;"
     "padding-bottom:2px;"
     "border-radius: 0.3rem 0.3rem 0 0;"
  "}";

const char HTTP_SCRIPT_CARDS[] PROGMEM = 
  "<script src='cards.min.js'></script>";



#ifdef USE_UNISHOX_COMPRESSION
#ifdef USE_SCRIPT_WEB_DISPLAY
const size_t HTTP_SCRIPT_ROOT_SIZE = 706;
const char HTTP_SCRIPT_ROOT_COMPRESSED[] PROGMEM = "\x33\xBF\xAF\x98\xF0\xA3\xE1\xC8\x78\x23\x02\xF8\x3A\xDC\xE4\x15\x9D\xD1\x87\x78"
                             "\xF6\x99\xDF\xD5\x9F\x0F\xB3\xEC\xF1\xA6\x0E\xE8\x56\x74\xBF\x8F\x0B\x1A\xFA\xBC"
                             "\x74\x09\xF0\xF5\x0A\x3E\x1F\x0E\x43\xBC\x7B\x4A\xCF\x83\x0F\x01\x84\xEF\xE5\x5A"
                             "\x35\xE0\xBA\x3B\xA1\x51\xDE\x3C\x1E\xED\x30\x77\x4D\x87\xF0\xF9\xC2\xC2\x08\xEF"
                             "\x1E\xD3\x61\xD2\xC7\x67\xE8\xEE\x9D\xE3\xC1\xEE\x36\x1F\x39\x8F\xA2\x36\x10\xD2"
                             "\x08\x85\x55\x0C\x2F\xB3\x50\xB7\xEA\x3B\xA7\x78\xF0\x6C\x3A\x67\x7D\xD8\x86\x5E"
                             "\xAB\xA6\x18\xAB\xE1\xE6\x7C\x04\x3D\xA0\xEE\x9D\xE3\xDB\xA6\x0E\xE9\xB0\xE9\xF7"
                             "\x62\x19\x17\xAA\xE9\x9F\x0F\x87\x30\xFD\x1F\xA2\x36\x1D\x3D\x57\x42\xFC\x7C\x3E"
                             "\x1C\xA6\xC8\x10\x78\x07\xF1\xF0\xD8\x74\xFB\xF0\xCC\xEF\x32\xA6\x6C\xA3\xA7\xD8"
                             "\xC0\xAC\x36\x77\x4E\xC3\xDB\x47\xB8\xEC\x1E\x3A\x8F\x61\xE9\x56\x38\x26\xBD\x46"
                             "\x41\x33\xE1\xF6\x3F\xA2\x50\xA3\xCC\xE4\x6C\xFA\x3E\x8F\xB3\xF0\xF6\x1D\xE2\x04"
                             "\x6C\x2B\xC0\x85\x85\x7C\xFC\x3D\x28\x50\x24\xD7\x1A\x08\x35\xCE\xCA\x14\x7E\x1E"
                             "\x94\x20\x24\xD8\x60\x87\x60\x43\xF0\xF4\x3B\x2B\xE0\x93\x64\x33\xDC\x76\x0F\x1D"
                             "\x47\xB0\xFA\x3E\x8F\xB3\xF0\xF4\x13\x4C\xC1\x0F\x60\xA6\x6C\xA3\xAE\xC2\xD1\xEE"
                             "\x3C\xC3\x7D\x4F\xE7\x83\x19\xD4\x75\x8F\xBD\x1E\x15\x47\x99\xEC\x3B\xC7\x86\x38"
                             "\xEE\x9F\x61\x1C\x87\xD9\xDE\x3A\x16\xF7\x3F\x90\xA2\xA2\x1A\x41\x1F\x3C\x78\x3D"
                             "\xC7\xB8\xF1\xA6\x11\xDD\xF9\x8F\x0A\x3B\xC8\xF6\x9B\x0E\x98\x31\xF1\xDD\x3E\xC8"
                             "\x78\x99\x51\xF6\x75\x1F\x67\x43\xB4\x34\xF8\x72\x1F\x67\x6C\xAC\xEA\xAF\x8B\x67"
                             "\x78\xF0\x6C\x3A\x79\xF0\x87\x74\xEF\x1E\x02\xA3\xE7\x9D\x02\x27\x20\x36\x75\x1F"
                             "\x42\x1D\xE3\xC1\xEE\x3D\xC0\x89\xCA\x77\x99\x9D\x9D\xD1\x97\xF3\xAB\x4C\xEF\xE7"
                             "\x78\xF6\x85\x67\x74\xFB\x3F\x5E\x33\x3E\x1F\x67\x6F\x4C\xEF\xE7\x6C\xFB\x3F\x67"
                             "\xD9\xDB\x19\x7F\x3B\xC7\x80\x46\xC3\x74\x12\x30\xD0\x42\xE6\x78\x14\xF1\x4F\x98"
                             "\xF0\xA3\xE1\xC6\x40\x8D\x8D\x8C\xF9\xDD\x30\x77\x8F\x6E\x98\x47\x74\xC1\xDE\x47"
                             "\xB4\x14\x37\xA0\x42\xCB\xCF\x72\x61\x79\xA3\xDA\x09\x9C\xE1\x02\x1E\x60\x7B\x8D";
#define  HTTP_SCRIPT_ROOT       Decompress(HTTP_SCRIPT_ROOT_COMPRESSED,HTTP_SCRIPT_ROOT_SIZE).c_str()
#else
const size_t HTTP_SCRIPT_ROOT_SIZE = 486;
const char HTTP_SCRIPT_ROOT_COMPRESSED[] PROGMEM = "\x30\x2F\x83\xAD\xCE\x41\x59\xDD\x18\x77\x8F\x69\x9D\xFD\x59\xF0\xFB\x3E\xCF\x1A"
                             "\x60\xEE\x85\x67\x4B\xF8\xF0\xB1\xAF\xAB\xC7\x40\x9F\x0F\x50\xA3\xE1\xF0\xE4\x3B"
                             "\xC7\xB4\xAC\xF8\x30\xF0\x18\x4E\xFE\x55\xA3\x5E\x0B\xA3\xBA\x15\x1D\xE3\xC1\xEE"
                             "\xD3\x07\x74\xD8\x7F\x0F\x9C\x2C\x20\x8E\xF1\xED\x36\x1D\x2C\x76\x7E\x8E\xE9\xDE"
                             "\x3C\x1E\xE3\x61\xF3\x98\xFA\x23\x61\x0D\x20\x88\x55\x50\xC2\xFB\x35\x0B\x7E\xA3"
                             "\xBA\x77\x8F\x06\xC3\xA6\x77\xDD\x88\x65\xEA\xBA\x61\x8A\xBE\x1E\x67\xC0\x43\xDA"
                             "\x0E\xE9\xDE\x3D\xBA\x60\xEE\x9B\x0E\x9F\x76\x21\x91\x7A\xAE\x99\xF0\xF8\x73\x0F"
                             "\xD1\xFA\x23\x61\xD3\xD5\x74\x2F\xC7\xC3\xE1\xCA\x6C\x81\x07\x80\x7F\x1F\x0D\x87"
                             "\x4F\xBF\x0C\xCE\xF3\x2A\x2B\x66\xCA\x3A\x7D\x8C\x0A\xC3\x67\x74\xEC\x3D\xB4\x7B"
                             "\x8E\xC1\xE3\xA8\xF6\x1E\x95\x63\x82\x6B\xD4\x64\x13\x3E\x1F\x63\xFA\x25\x0A\x3C"
                             "\xCE\x46\xCF\xA3\xE8\xFB\x3F\x0F\x61\xDE\x20\x46\xC2\xBC\x08\x58\x57\xCF\xC3\xD2"
                             "\x85\x02\x4D\x71\xA0\x83\x5C\xEC\xA1\x47\xE1\xE9\x42\x02\x4D\x86\x08\x76\x04\x3F"
                             "\x0F\x43\xB2\xBE\x09\x36\x43\x3D\xC7\x60\xF1\xD4\x7B\x0F\xA3\xE8\xFB\x3F\x0F\x41"
                             "\x34\xCC\x10\xF6\x0A\x66\xCA\x3A\xEC\x2D\x1E\xE3\xCC\x37\xD4\xFE\x78\x31\x9D\x47"
                             "\x58\xFB\xD1\xE1\x54\x79\x9E\xC3\xBC\x78\x63\x8E\xE9\xF6\x11\xC8\x7D\x9D\xE3\xA1"
                             "\x6F\x73\xF9\x0A\x2A\x2B\x21\xA4\x11\xF3\xC7\x83\xDC\x7B\x8F\x06\xC3\xA6\x0C\x7C"
                             "\x77\x4F\xB2\x1E\x26\x54\x7D\x9D\x47\xD9\xD0\xED\x0D\x3E\x1C\x87\xD9\xDB\x2B\x3A"
                             "\xAB\xE2\xD9\xDE\x3C\x1B\x0E\x9E\x7C\x21\xDD\x3B\xC7\x80\xA8\xF9\xE7\x40\x89\xC7"
                             "\xB5\x9D\x47\xD0\x87\x78\xF0\x7B\x8D";
#define  HTTP_SCRIPT_ROOT       Decompress(HTTP_SCRIPT_ROOT_COMPRESSED,HTTP_SCRIPT_ROOT_SIZE).c_str()
#endif  // USE_SCRIPT_WEB_DISPLAY
#else
const char HTTP_SCRIPT_ROOT[] PROGMEM =
#ifdef USE_SCRIPT_WEB_DISPLAY
  "var rfsh=1;"
  "function la(p){"
    "var a='';"
    "if(la.arguments.length==1){"
      "a=p;"
      "clearTimeout(lt);"
    "}"
    "if(x!=null){x.abort();}"             // Abort if no response within 2 seconds (happens on restart 1)
    "x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(x.readyState==4&&x.status==200){"
        "var s=x.responseText.replace(/{t}/g,\"<table style='width:100%%'>\").replace(/{s}/g,\"<tr><th>\").replace(/{m}/g,\"</th><td>\").replace(/{e}/g,\"</td></tr>\").replace(/{c}/g,\"%%'><div style='text-align:center;font-weight:\");"
        "eb('l1').innerHTML=s;"
      "}"
    "};"
    "if (rfsh) {"
      "x.open('GET','.?m=1'+a,true);"       // ?m related to request->hasArg("m")
      "x.send();"
      "lt=setTimeout(la,%d);"               // Settings.web_refresh
    "}"
  "}"
  "function seva(par,ivar){"
    "la('&sv='+ivar+'_'+par);"
  "}"
  "function siva(par,ivar){"
    "rfsh=1;"
    "la('&sv='+ivar+'_'+par);"
    "rfsh=0;"
  "}"
  "function pr(f){"
    "if (f) {"
      "lt=setTimeout(la,%d);"
      "rfsh=1;"
    "} else {"
      "clearTimeout(lt);"
      "rfsh=0;"
    "}"
  "}";
#else  // USE_SCRIPT_WEB_DISPLAY
  "function la(p){"
    "var a='';"
    "if(la.arguments.length==1){"
      "a=p;"
      "clearTimeout(lt);"
    "}"
    "if(x!=null){x.abort();}"             // Abort if no response within 2 seconds (happens on restart 1)
    "x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(x.readyState==4&&x.status==200){"
        "var s=x.responseText.replace(/{t}/g,\"<table style='width:100%%'>\").replace(/{s}/g,\"<tr><th>\").replace(/{m}/g,\"</th><td>\").replace(/{e}/g,\"</td></tr>\").replace(/{c}/g,\"%%'><div style='text-align:center;font-weight:\");"
        "eb('l1').innerHTML=s;"
      "}"
    "};"
    "x.open('GET','.?m=1'+a,true);"       // ?m related to request->hasArg("m")
    "x.send();"
    "lt=setTimeout(la,%d);"               // Settings.web_refresh
  "}";
#endif  // USE_SCRIPT_WEB_DISPLAY
#endif //USE_UNISHOX_COMPRESSION

#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_SCRIPT_ROOT_PART2_SIZE = 222;
const char HTTP_SCRIPT_ROOT_PART2_COMPRESSED[] PROGMEM = "\x30\x2F\x83\xAD\xCE\x41\x06\x77\x4C\xCE\xAD\x3A\x86\x1D\xE3\xDB\xA6\x0E\xEB\x1C"
                             "\x77\x4F\xBF\x1F\x67\x78\xEF\x1E\xDD\x30\x77\x4C\xCF\x87\xC3\xEC\x51\xF6\x7F\x8F"
                             "\xF1\x99\xF0\xF8\x7D\x88\x7D\x9D\xE3\xDA\x67\x7F\x5E\x08\xF8\xC7\x1D\xD3\xEF\xC1"
                             "\x1C\xC3\xEC\xEF\x1D\x08\xCE\xC2\x16\xCF\x2A\x01\x85\x87\x9D\x3D\x46\x41\x33\xA0"
                             "\xEB\x0C\xD0\x7B\xF8\x2F\x84\x3E\x1F\x61\x6F\x3B\xF9\xD6\x3D\xFB\x13\x5F\x51\xDD"
                             "\xAC\x5F\xD1\xE1\x54\x75\x7C\x78\x71\xDD\x3E\xCE\xDF\x82\x3B\x67\xD9\xF4\x7D\x1D"
                             "\x40\x89\x14\x10\xE2\x9D\xE3\xA8\x57\x82\x3B\xA7\xD9\xDB\x04\x1D\x14\xE5\x10\x21"
                             "\xE8\xA7\x6C\xFB\x3A\x8E\x46\xCF\xA3\xE8\xEA\xD6\x7D\x1F\x47\x78\xEF\x1F\x67\x83"
                             "\xDC\x7B\x88\x2B\x3B\xA7\xD9\xFA\x3E\xCE\xD9\x99\xDB\xD3\xB6\x7D\x9F\x0F\xB3\xB6"
                             "\x30\xEF\x1E\x0F\x70\xF8\x47\x74\x2B\x3B\xC7\x83";
#define  HTTP_SCRIPT_ROOT_PART2       Decompress(HTTP_SCRIPT_ROOT_PART2_COMPRESSED,HTTP_SCRIPT_ROOT_PART2_SIZE).c_str()
#else
const char HTTP_SCRIPT_ROOT_PART2[] PROGMEM =
  "function lc(v,i,p){"
    "if(eb('s')){"                        // Check if Saturation is in DOM otherwise javascript fails on la()
      "if(v=='h'||v=='d'){"               // Hue or Brightness changed so change Saturation colors too
        "var sl=eb('sl4').value;"
        "eb('s').style.background='linear-gradient(to right,rgb('+sl+'%%,'+sl+'%%,'+sl+'%%),hsl('+eb('sl2').value+',100%%,50%%))';"
      "}"
    "}"
    "la('&'+v+i+'='+p);"
  "}"
  "wl(la);";
#endif //USE_UNISHOX_COMPRESSION

const char HTTP_SCRIPT_WIFI[] PROGMEM =
  "function c(l){"
    "eb('s1').value=l.innerText||l.textContent;"
    "eb('p1').focus();"
  "}";

const char HTTP_SCRIPT_RELOAD_TIME[] PROGMEM =
  "setTimeout(function(){location.href='.';},%d);";

#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_SCRIPT_CONSOL_SIZE = 853;
const char HTTP_SCRIPT_CONSOL_COMPRESSED[] PROGMEM = "\x33\xBF\xAF\x71\xF0\xE3\x3A\x8B\x44\x3E\x1C\x67\x82\x30\x2F\x83\xAD\xCE\x41\x1D"
                             "\xD1\x87\x78\xF6\x99\xDF\xD0\x67\x56\x1F\x0F\xB3\xEC\xEA\xA3\xC0\x61\x3B\xF9\x56"
                             "\x8D\x78\x2E\x8E\xE8\x54\x77\x8F\x14\x7C\x63\x8E\xE9\xF7\x47\x21\xF6\x77\x8F\x05"
                             "\xA6\x0E\xE8\xC3\xE1\xF0\xE4\x3B\xC7\xB4\x83\x3E\x31\xC7\x74\xFB\x0C\xE4\x3E\xCE"
                             "\xF1\xE0\xB0\xF8\x7D\x9F\xA0\xCE\x43\xE1\xF6\x76\xC9\xF0\x78\x23\x21\x65\xF2\xD2"
                             "\x0F\x06\x8C\xCE\x7D\x47\x74\x33\xA1\x9D\x84\x2D\x9D\xE3\xC0\x21\x45\x3E\x1F\x67"
                             "\xD9\xE2\x8E\x9E\x0F\xF8\x10\x45\x58\x30\xF8\x71\x11\xBD\x2A\x01\xF1\xEE\x3E\x02"
                             "\x35\x13\xC1\xEE\xD3\x07\x74\x11\xA6\x1F\x87\xCF\x71\xDE\x3D\xBA\x60\xEE\x9B\x0F"
                             "\xE1\xF3\x85\x84\x11\xDE\x3D\xA6\xC3\xA5\x8E\xCF\xD1\xDD\x3B\xC7\x83\xDC\x6C\x3E"
                             "\x73\x1F\x44\x6C\x21\xA4\x11\x0A\xAA\x18\x5F\x66\xA1\x6F\xD4\x77\x4E\xF1\xE0\xD8"
                             "\x74\xCE\xFB\xB1\x0C\xBD\x57\x4C\x31\x57\xC3\xCC\xF8\x08\x7C\x28\x1D\xD0\x41\xCA"
                             "\x8E\x9F\x76\x21\x91\x7A\xAE\x99\xF0\xF8\x73\x0F\xD1\xFA\x23\x61\xD3\xD5\x74\x2F"
                             "\xC7\xC3\xE1\xCA\x6C\x81\x07\x87\x03\x69\xD4\x21\xE0\x43\xE1\xB0\xE9\xF7\xE1\x99"
                             "\xDE\x65\x4C\xD9\x47\x4F\x0C\x0B\x68\xEE\x9D\x87\xB8\xE4\x3B\x0E\xF1\xE0\xB4\x43"
                             "\xE0\x87\x4F\x0A\xD3\x14\x77\x4E\xF1\xE3\x4C\x1D\xD0\x44\x92\x7C\x3E\x1C\x67\x78"
                             "\xF6\x95\x02\x2F\x0A\x27\xB8\xDA\x09\x38\x29\xB4\xE8\x13\xE1\xEA\x14\x7E\x02\x2E"
                             "\x06\x76\xCF\x86\xD3\xC1\xEE\x05\xDE\x1E\x4F\x71\xE0\xD8\x74\xC1\x8F\x8E\xE9\xF6"
                             "\x43\xC4\xCA\x8F\xB3\xA8\xFB\x0F\xC7\x68\x33\x94\x7C\x3E\xCE\xD9\x68\x87\x6F\x0E"
                             "\xAA\xF8\xB6\x77\x8F\x06\xC3\xA7\x9F\x08\x77\x4E\xF1\xE0\xF7\x05\x47\xCF\x3A\x04"
                             "\x4E\x4A\x4E\xA3\xE8\x43\xBC\x78\xFB\xA1\x7F\xE4\x62\xC2\xF3\x3C\x1E\xE1\xF0\x8E"
                             "\xE8\x47\x78\xF0\x67\x7F\x42\x83\x3E\x1E\xF1\xEF\x9D\x41\xF0\x23\xF2\xF4\x28\xEE"
                             "\x9D\xE3\xDA\x08\x7C\xA2\x9D\x2C\x41\x09\x99\xBE\xA2\x0B\x7D\x4F\x9F\xCE\xE9\xF6"
                             "\x68\xCC\x84\xC1\xFE\x3E\xCE\xA0\x44\xE2\xDD\x82\x0F\x12\xA3\x81\x13\x97\xB3\xA8"
                             "\x33\xE3\x3A\x1A\x33\x22\x0F\x04\x67\x8D\x30\x77\x4E\x5F\xCF\x87\xC2\x0C\xFF\x1F"
                             "\xE3\x98\xCF\x87\xC2\x0C\xEF\x1E\xD1\xC7\x4B\x17\x58\x1E\x0D\x18\x13\xA6\x7C\x3E"
                             "\xF0\xC1\x83\xEC\xF0\x7B\x8E\x5F\xCF\x87\xC2\x0C\xED\x1D\xD3\xB6\x76\xC3\xE3\xF0"
                             "\x50\x60\x85\xC4\x31\xFA\x3F\x47\x74\x3E\x3E\x02\x24\xB3\xBC\x75\x0E\x04\x2E\x2E"
                             "\x85\x06\x7B\xC1\xF1\xD6\x72\x1E\xF9\xFE\x3F\xC7\xD9\xF6\x77\x8F\x3C\x67\xC3\xE1"
                             "\x06\x76\x8E\xE9\xC6\x7E\x1D\x67\x59\x07\xC0\x83\x88\x1C\x64\x0A\x78\x41\xC9\x67"
                             "\xC3\xE1\x06\x7E\x8F\xD1\xDD\x04\x4C\xC4\xFC\x39\x11\xFA\x3F\x44\x28\x33\xA0\xCC"
                             "\x18\x77\x4E\xF1\xD4\x28\x33\xA0\xBE\x04\x1E\x44\x01\x0B\x1C\x3B\xC7\x50\x7C\x7C"
                             "\x38\xCE\xF1\xEE\x3B\xC7\x83\xDC\x43\xE1\x1D\xD1\x47\x78\xF0";
#define  HTTP_SCRIPT_CONSOL       Decompress(HTTP_SCRIPT_CONSOL_COMPRESSED,HTTP_SCRIPT_CONSOL_SIZE).c_str()
#else
const char HTTP_SCRIPT_CONSOL[] PROGMEM =
  "var sn=0,id=0;"                        // Scroll position, Get most of weblog initially
  "function l(p){"                        // Console log and command service
    "var c,o='',t;"
    "clearTimeout(lt);"
    "t=eb('t1');"
    "if(p==1){"
      "c=eb('c1');"                       // Console command id
      "o='&c1='+encodeURIComponent(c.value);"
      "c.value='';"
      "t.scrollTop=99999;"
      "sn=t.scrollTop;"
    "}"
    "if(t.scrollTop>=sn){"                // User scrolled back so no updates
      "if(x!=null){x.abort();}"           // Abort if no response within 2 seconds (happens on restart 1)
      "x=new XMLHttpRequest();"
      "x.onreadystatechange=function(){"
        "if(x.readyState==4&&x.status==200){"
          "var z,d;"
          "d=x.responseText.split(/}1/);"  // Field separator
          "id=d.shift();"
          "if(d.shift()==0){t.value='';}"
          "z=d.shift();"
          "if(z.length>0){t.value+=z;}"
          "t.scrollTop=99999;"
          "sn=t.scrollTop;"
        "}"
      "};"
      "x.open('GET','cs?c2='+id+o,true);"  // Related to request->hasArg("c2") and WebGetArg("c2", stmp, sizeof(stmp))
      "x.send();"
    "}"
    "lt=setTimeout(l,%d);"
    "return false;"
  "}"
  "wl(l);"                                // Load initial console text

  // Console command history
  "var hc=[],cn=0;"                       // hc = History commands, cn = Number of history being shown
  "function h(){"
//    "if(!(navigator.maxTouchPoints||'ontouchstart'in document.documentElement)){eb('c1').autocomplete='off';}"  // No touch so stop browser autocomplete
    "eb('c1').addEventListener('keydown',function(e){"
      "var b=eb('c1'),c=e.keyCode;"       // c1 = Console command id
      "if(38==c||40==c){b.autocomplete='off';}"  // ArrowUp or ArrowDown must be a keyboard so stop browser autocomplete
      "38==c?(++cn>hc.length&&(cn=hc.length),b.value=hc[cn-1]||''):"   // ArrowUp
      "40==c?(0>--cn&&(cn=0),b.value=hc[cn-1]||''):"                   // ArrowDown
      "13==c&&(hc.length>19&&hc.pop(),hc.unshift(b.value),cn=0)"       // Enter, 19 = Max number -1 of commands in history
    "});"
  "}"
  "wl(h);";                               // Add console command key eventlistener after name has been synced with id (= wl(jd))
#endif //USE_UNISHOX_COMPRESSION

const char HTTP_MODULE_TEMPLATE_REPLACE_INDEX[] PROGMEM =
  "}2%d'>%s (%d)}3";                       // }2 and }3 are used in below os.replace
const char HTTP_MODULE_TEMPLATE_REPLACE_NO_INDEX[] PROGMEM =
  "}2%d'>%s}3";                           // }2 and }3 are used in below os.replace

#if defined(USE_UNISHOX_COMPRESSION)
const size_t HTTP_SCRIPT_MODULE_TEMPLATE_SIZE = 602;
const char HTTP_SCRIPT_MODULE_TEMPLATE_COMPRESSED[] PROGMEM = "\x33\xBF\xAC\xF1\xD4\x2B\xC7\x83\x02\xF8\x3A\xDC\xE4\x1B\x3B\xBA\x75\x1A\x8E\xF1"
                             "\xED\x33\xBF\xAC\x3E\x09\x81\x8B\x1A\xFA\x8E\x81\xFD\xDD\x32\x61\x31\xAF\xA8\xEE"
                             "\x9F\x78\x32\xB7\x38\xFB\x3B\xC7\x8C\x3A\x53\x36\x51\x07\x9D\x4F\xA8\xF9\xA7\x83"
                             "\x51\xD2\xC6\x0C\x7C\x21\x06\x2B\x42\x10\xEE\xE1\xDE\x3C\x1E\xE0\x44\xCD\xB2\x8E"
                             "\xE8\xF1\xD5\xE0\x41\xCD\xAC\xF9\xE3\xF4\x71\x91\xB0\xC1\x86\x71\x9D\x44\x38\xF8"
                             "\x71\x9D\x44\x19\xD4\x30\xEA\x08\xEA\xA3\xE1\xAB\xC7\x74\xFB\x3C\x85\x1F\x67\x6C"
                             "\x78\xEF\x1D\x42\xCF\x9E\x3F\x47\x19\x1B\x0E\x37\x08\xC1\xE0\x23\xE5\x1D\x01\x07"
                             "\x4D\xF1\xD0\x27\xC3\xD4\x28\xF0\x63\x3E\x77\x74\xF8\x11\xE3\x4F\x1A\x75\x9D\x67"
                             "\x78\xF6\x8C\x04\x5B\xC7\xBD\xA7\x59\xC8\x7B\xE7\x42\x19\x7F\x7D\x45\xD8\x23\x3C"
                             "\x0C\x3A\x7D\x8D\xC3\x36\x08\x3B\x70\x24\xE0\x87\x78\xF0\x7B\x82\x3E\x0A\x04\xAC"
                             "\xC8\xE3\x3C\x16\x9E\x81\x1E\x34\xED\x9D\xB3\xBC\x7B\x43\x3E\x0A\xF1\xEF\x69\xEF"
                             "\x82\x17\x2A\x01\xE7\x8D\x30\x77\x6C\xF8\x7C\x0C\xEF\x1E\xD1\xC0\x89\x50\xE3\x70"
                             "\x8C\x1E\x07\x7D\xD9\xA1\xE0\xF7\x1E\xEF\x1F\x87\xE1\xF0\xE6\x90\x21\x64\x47\x21"
                             "\xE0\xB4\xF4\x3E\x0E\x04\x2C\x8D\x9D\xD3\xBB\xA7\xA1\xC8\xCE\xF1\xDA\x3B\xA7\xD9"
                             "\xDC\x3E\xCE\xD9\x69\xDE\x3C\xF4\xEA\xA3\xBC\x78\x3D\xCC\x71\xDD\x3E\xC5\x1F\x67"
                             "\x6C\x78\xEF\x1D\x0C\xEC\x21\x6C\xF8\x2C\xED\x9C\x87\x82\xA3\xA7\xA8\xC8\x26\x74"
                             "\x33\xDF\x68\xED\x0B\x68\xC8\xF8\x77\x47\x1F\x87\x19\xDE\x3B\x47\xD9\xF6\x79\x9F"
                             "\x64\x2B\x44\x11\xF1\xF6\x08\xDC\x58\xF8\xD0\xEE\xF8\xEA\x1E\x04\x3E\x42\xF3\xC7"
                             "\x4F\xB1\x81\x58\x6C\xEE\x9D\x87\xB8\xE5\x1D\x84\x3C\x75\x1E\xC3\xD0\x10\x78\x4B"
                             "\x40\x83\x9E\x9F\x67\xB0\xEF\x02\x35\xD3\x96\x76\x10\xF1\xD4\x7B\x0F\x43\xB0\x10"
                             "\x6F\x1F\x87\xB0\xEF\x1E\x18\xE3\xBA\x7D\x8F\x1F\x67\x6C\x78\xEF\x1D\x37\xB9\xFC"
                             "\x85\x15\x10\xD2\x08\xF9\x80\x8D\x48\x10\x72\x13\xBA\x3C\x7A\x1C\x48\xEF\x1D\xA2"
                             "\x04\x3E\x47\x4F\x3F\x1E\x34\xC0\x20\xD0\x3D\xA0\x85\xC9\xF9\xE0\xF7\x1E\xE3";
#define  HTTP_SCRIPT_MODULE_TEMPLATE       Decompress(HTTP_SCRIPT_MODULE_TEMPLATE_COMPRESSED,HTTP_SCRIPT_MODULE_TEMPLATE_SIZE).c_str()
#else
const char HTTP_SCRIPT_MODULE_TEMPLATE[] PROGMEM =
  "var os,hs;"
  "function ce(i,q){"                     // Create index select
    "var o=document.createElement('option');"
    "o.textContent=i;"
    "q.appendChild(o);"
  "}"
  "function ot(g,s){"                     // g = id and name, s = value
    "var a=s&0xffe0,b=0,c,p,l,t=qs('#h'+g),u=s&0x001f;"
    "l=t.options.length;"                 // Remove current options
    "for(i=l;i;i--){p=t.options[i-1].parentNode;p.removeChild(t.options[i-1]);}"
    "l=hs.length;"                        // Find max indexes for s
    "for(i=0;i<l;i++){c=hs[i]&0xffe0;if(a==c){b=hs[i]&0x001f;break;}}"
    "s>>=5;"                              // Add options
    "for(i=1;i<=b;i++){ce((i<10)?(' '+i):i,t);}"
    "eb('h'+g).value=u+1;"                // Set selected value
    "t.style.visibility=(b>0)?'':'hidden';"
  "}"
  "function sk(s,g){"                     // s = value, g = id and name
    "var o=os.replace(/}2/g,\"<option value='\").replace(/}3/g,\"</option>\");"
    "eb('g'+g).innerHTML=o;"
    "eb('g'+g).value=(g<99)?s&0xffe0:s;"
    "if(g<99){ot(g,s);}"
  "}";
#endif //USE_UNISHOX_COMPRESSION

#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_SCRIPT_TEMPLATE_SIZE = 303;
const char HTTP_SCRIPT_TEMPLATE_COMPRESSED[] PROGMEM = "\x30\x2F\x83\xAD\xCE\x41\x08\x77\x45\x9D\x46\x0E\xF1\xED\x33\xBF\xA3\x61\xF3\x98"
                             "\xFA\x23\x61\x0D\x20\x88\x55\x50\xC2\xFB\x35\x0B\x7E\xA3\xBA\x77\x8F\x06\xC3\xA6"
                             "\x77\xDD\x88\x65\xEA\xBA\x61\x8A\xBE\x1E\x67\xC0\x43\xC7\x4E\xE9\xDE\x3D\xBA\x60"
                             "\xEE\xD0\xAD\xF1\xD3\xEE\xC4\x32\x2F\x55\xD3\x3E\x1F\x0E\x61\xFA\x3F\x45\x42\xB7"
                             "\xC7\x4F\x55\xD0\xBF\x1F\x0F\x87\x29\xB3\xBC\x7B\x48\x10\x70\x43\xBC\x78\x3D\xC7"
                             "\xB8\xF0\x6C\x3A\x60\xC7\xC7\x74\xFB\x21\xE2\x65\x47\xD9\xD4\x2C\xEA\xAF\x8B\x67"
                             "\x78\xF0\x6C\x3A\x79\xF0\x87\x74\xEF\x1E\x0F\x71\x9D\xFD\x06\x78\x04\x4E\x2A\x01"
                             "\x4D\x87\x21\xDD\x21\xC0\x83\xBF\xE9\xD4\x6B\x3A\x87\x8E\xA3\x43\xAB\x0F\x18\x7C"
                             "\x1C\x74\xFB\xF0\xCC\xEF\x32\xA6\x6C\xA3\xA7\x86\x05\xB4\x77\x4E\xC3\xDC\x72\x1D"
                             "\x87\x78\xF0\x46\x87\xCC\x3A\x78\x56\x98\xA3\xBA\x77\x8F\x1A\x60\xEE\xB1\xC7\x74"
                             "\xFB\xF1\xC8\x7D\x9D\xE3\xA1\x19\xD8\x42\xD9\xF0\xF8\x7D\x9F\x67\x78\xF6\x82\x55"
                             "\x03\x43\xC1\xEE\x1E\x04\x5C\x44\x10\xB2\x93\xEC\xEA\x3E\xCE\xF1\xE3\x3C\x7C\x3D"
                             "\x86";
#define  HTTP_SCRIPT_TEMPLATE       Decompress(HTTP_SCRIPT_TEMPLATE_COMPRESSED,HTTP_SCRIPT_TEMPLATE_SIZE).c_str()
#else
const char HTTP_SCRIPT_TEMPLATE[] PROGMEM =
  "function ld(u,f){"
    "var x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(this.readyState==4&&this.status==200){"
        "f(this);"
      "}"
    "};"
    "x.open('GET',u,true);"
    "x.send();"
  "}"
  "var c;"                                // Need a global for BASE
  "function x1(b){"
    "var i,j,g,k,o;"
    "o=b.responseText.split(/}1/);"       // Field separator
    "k=o.shift();"                        // Template name
    "if(eb('s1').value==''){"
      "eb('s1').value=k;"                 // Set NAME if not yet set
    "}"
    "g=o.shift().split(',');"             // GPIO - Array separator
    "os=\"";                              // }2'0'>None (0)}3}2'17'>Button1 (17)}3...
#endif //USE_UNISHOX_COMPRESSION

const char HTTP_SCRIPT_TEMPLATE2[] PROGMEM =
    "j=0;"
    "for(i=0;i<" STR(MAX_USER_PINS) ";i++){"  // Supports 13 GPIOs
      "if(6==i){j=9;}"
      "if(8==i){j=12;}"
      "sk(g[i],j);"                       // Set GPIO
      "j++;"
    "}";
const char HTTP_SCRIPT_TEMPLATE3[] PROGMEM =
    "\";"
    "sk(g[13]," STR(ADC0_PIN) ");";       // Set ADC0

const char HTTP_SCRIPT_TEMPLATE4[] PROGMEM =
    "g=o.shift();"                        // FLAG
    "for(i=0;i<" STR(GPIO_FLAG_USED) ";i++){"
      "p=(g>>i)&1;"
      "eb('c'+i).checked=p;"              // Set FLAG checkboxes
    "}"
    "if(" STR(USER_MODULE) "==c){"
      "g=o.shift();"
      "eb('g99').value=g;"                // Set BASE for initial select
    "}"
  "}"
  "function st(t){"
    "c=t;"                                // Needed for initial BASE select
    "var a='tp?t='+t;"
    "ld(a,x1);"                           // ?t related to WebGetArg("t", stemp, sizeof(stemp));
  "}"
  "function sl(){"
    "os=\"";                              // }2'0'>Sonoff Basic (1)}3...
const char HTTP_SCRIPT_TEMPLATE5[] PROGMEM =
    "\";"
    "sk(" STR(WEMOS_MODULE) ",99);"       // 17 = WEMOS
    "st(" STR(USER_MODULE) ");"
  "}"
  "wl(sl);";

const char HTTP_SCRIPT_INFO_BEGIN[] PROGMEM =
  "function i(){"
    "var s,o=\"";
const char HTTP_SCRIPT_INFO_END[] PROGMEM =
    "\";"                                 // "}1" and "}2" means do not use "}x" in Information text
    "s=o.replace(/}1/g,\"</td></tr><tr><th>\").replace(/}2/g,\"</th><td>\");"
    "eb('i').innerHTML=s;"
  "}"
  "wl(i);";

#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_HEAD_LAST_SCRIPT_SIZE = 226;
const char HTTP_HEAD_LAST_SCRIPT_COMPRESSED[] PROGMEM = "\x30\x2F\x83\xAD\xCE\x46\xB1\x0E\xE9\xDE\x3D\xA6\x77\xF5\x47\xC3\x8C\xEA\x2D\x3E"
                             "\x09\x81\x8B\x1A\xFA\x8E\x86\xA1\x6F\xE6\x45\xE6\x13\x0E\xB3\xE5\x61\x04\x77\x4F"
                             "\xBD\xE1\x82\xE8\xEA\x1C\x2E\xAB\x38\xEA\xA6\x6C\xAB\xFB\xB3\xAB\xCC\x26\x1D\x1F"
                             "\x67\x78\xF0\x3E\x2B\x42\x67\x77\x4E\x81\x3E\x1E\xA1\x47\xE1\xF2\x8E\xF1\xED\xD3"
                             "\x07\x77\x4F\x7A\x8F\x7C\xEF\x1E\xDD\x3D\xEA\x3D\xF3\xDE\x3E\xFA\xC6\xB3\xEC\xF7"
                             "\xCF\x87\x77\x4F\x7A\x8F\x7C\xE8\x2A\x2B\xFC\x57\x55\xFD\x1C\x2E\x99\xDD\x3E\xF4"
                             "\x43\xEC\xEF\x1F\xA3\xF4\x77\x4F\xE0\x27\x57\xEB\x1A\xCF\xB3\xBC\x77\x8E\xF1\xDA"
                             "\x04\x1C\x87\x44\x3E\xCF\x7C\xF3\x04\x7C\xB0\xF0\x7B\xA8\xED\x9D\xB3\xC1\xEE\x3D"
                             "\xC3\xE1\x1D\xD3\x58\x87\x78\xF0\x7A\x1D\x9E\x0F\xFA\x32\x8F\xC3";
#define  HTTP_HEAD_LAST_SCRIPT       Decompress(HTTP_HEAD_LAST_SCRIPT_COMPRESSED,HTTP_HEAD_LAST_SCRIPT_SIZE).c_str()
#else
const char HTTP_HEAD_LAST_SCRIPT[] PROGMEM =
  "function jd(){"                        // Add label name='' based on provided id=''
    "var t=0,i=document.querySelectorAll('input,button,textarea,select');"
    "while(i.length>=t){"
      "if(i[t]){"
        "i[t]['name']=(i[t].hasAttribute('id')&&(!i[t].hasAttribute('name')))?i[t]['id']:i[t]['name'];"
      "}"
      "t++;"
    "}"
  "}"
  "wl(jd);"                               // Add name='' to any id='' in input,button,textarea,select
  "</script>";
#endif //USE_UNISHOX_COMPRESSION

#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_HEAD_STYLE1_SIZE = 591;
const char HTTP_HEAD_STYLE1_COMPRESSED[] PROGMEM = "\x3D\x3D\x46\x41\x33\xF0\x4D\x33\x3A\x8C\x6B\x08\x4F\x3A\x3A\xB7\x86\x0B\xA3\xAB"
                             "\xCC\x26\x1D\x1E\xD1\x96\x20\x9B\xC3\xC7\x99\xCD\x21\x86\xC3\xC1\x8C\xEA\x3A\xFD"
                             "\xA6\xD6\x79\x9C\x84\xC6\x9E\x0F\x70\x21\xE1\xA7\xB4\x75\x86\x68\x3D\xFC\x17\xC2"
                             "\x1E\x67\x91\xF4\x71\xF1\x1B\x0F\x07\xB8\x61\xED\x1B\x7F\x1E\xDE\x3C\xCE\x33\xA6"
                             "\x93\x1A\x8E\x33\xC1\xEE\x2D\xE1\x82\xE8\xF6\x8F\xE8\x94\x28\xF3\x39\x1B\x3E\x8F"
                             "\xA3\xC1\x0E\xC3\x61\xD7\xED\x36\xEF\x0F\x1E\x63\xB3\xE2\x3F\x9D\x63\xB0\xD8\x78"
                             "\x3A\xC7\xD8\xE3\x4D\xA3\xAC\x14\xAD\x0D\xC3\x68\x29\x57\x04\xCD\x84\x3C\x0B\x3E"
                             "\x08\x7B\x6E\xF0\xC1\x74\x7B\xD4\x64\x31\x9F\x03\x14\xC3\x34\x1D\x86\xC3\xDF\x04"
                             "\x1E\x11\x41\x06\x8F\xEC\x4D\xC3\xDF\x04\x3D\xF1\x8D\x3C\x02\x0F\x03\x87\x5F\xF4"
                             "\x78\x55\x1E\x67\x38\x86\x1B\x0F\x06\x6F\xF5\xA1\xD8\x47\x5D\x85\xA3\xDC\x79\x9D"
                             "\x67\x21\x0C\x04\x9C\xCF\xF7\xC3\xCC\x10\xF1\xE3\x89\x1F\x47\xD1\xE0\xF7\x10\x21"
                             "\x71\x3E\x09\x1C\x28\x82\xC7\x2A\x01\x54\xCD\x95\x7F\x76\x7B\x7E\xFD\xA6\xD6\x79"
                             "\x82\x1E\xA0\x78\x04\x2C\xC8\xE7\xCF\xA3\xE8\xF0\x42\x9E\x8F\x0A\xA3\xCC\xE5\xCF"
                             "\x90\xC3\x61\xE0\x11\xF8\xFA\xC3\x37\xF3\x01\x60\xF9\xE7\x62\xEB\x01\x6B\x45\x1D"
                             "\x82\x19\x1E\xDA\x66\xCA\x04\x2E\x0A\x83\x7D\x4F\xE0\x83\xC9\xE9\x8B\x1B\xA1\x19"
                             "\x1E\x66\x6F\xE2\x5F\x59\xD5\xEB\xEF\x1D\x7E\x7F\xD3\x2A\x01\x9B\x98\x1E\xEA\x10"
                             "\x11\x39\x7D\x38\xC8\x61\xB0\xF0\x7B\x8D";
#define  HTTP_HEAD_STYLE1       Decompress(HTTP_HEAD_STYLE1_COMPRESSED,HTTP_HEAD_STYLE1_SIZE).c_str()
#else
const char HTTP_HEAD_STYLE1[] PROGMEM =
  "<style>"
  "div,fieldset,input,select{padding:5px;font-size:1em;}"
  "fieldset{background:#%06x;}"  // COLOR_FORM, Also update HTTP_TIMER_STYLE
  "p{margin:0.5em 0;}"
  "input{width:100%%;box-sizing:border-box;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;background:#%06x;color:#%06x;}"  // COLOR_INPUT, COLOR_INPUT_TEXT
  "input[type=checkbox],input[type=radio]{width:1em;margin-right:6px;vertical-align:-1px;}"
  "input[type=range]{width:99%%;}"
  "select{width:100%%;background:#%06x;color:#%06x;}"  // COLOR_INPUT, COLOR_INPUT_TEXT
  "textarea{resize:vertical;width:98%%;height:318px;padding:5px;overflow:auto;background:#%06x;color:#%06x;}"  // COLOR_CONSOLE, COLOR_CONSOLE_TEXT
  "body{text-align:center;font-family:verdana,sans-serif;background:#%06x;}"  // COLOR_BACKGROUND
  "td{padding:0px;}";
#endif //USE_UNISHOX_COMPRESSION

#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_HEAD_STYLE2_SIZE = 478;
const char HTTP_HEAD_STYLE2_COMPRESSED[] PROGMEM = "\x1C\x2E\xAB\x38\xF6\x8E\xCF\x88\xFE\x79\x9C\x67\x82\x04\x18\xA7\x5F\xEC\x4D\x17"
                             "\xE3\xCC\xE3\x3A\x59\x7D\x8D\x3C\x0E\xB0\xCD\x07\xBF\x82\xF8\x43\xCC\xF2\x3E\x8E"
                             "\x3E\x23\x61\xE0\x3C\x0B\x3E\x08\x52\x02\xDE\x67\x58\xA7\xA3\xC2\xA8\xF3\x39\x47"
                             "\x4C\x2F\xB1\xA7\x83\x19\xD4\x75\xFB\x4D\xAC\xF3\x39\x0E\x94\x5F\x63\x4F\x03\xFA"
                             "\x25\x0A\x3C\xCE\x46\xCF\xA3\xE8\xF0\x75\x90\xFB\x1C\x69\xB4\x75\xD7\xEF\xBD\xB5"
                             "\xB9\xC7\x58\x82\xFF\x75\xB9\xC7\x99\xC6\x74\xC2\xF1\xE0\x15\x2A\x2B\x86\x2F\xFE"
                             "\xCF\x9E\x63\x33\x7A\x9F\xCF\x07\xB8\x10\x78\x18\x3C\xC5\x61\x9B\xF9\xED\x04\xCE"
                             "\x2A\x01\x0F\x71\xD0\x77\xD8\x80\xA7\x50\x15\xB1\x21\xEF\xF0\x29\xD4\x05\x4C\x4A"
                             "\xCF\x68\x23\xF0\xDF\x4C\xD9\x47\x58\x8C\x3C\x04\x2E\x06\xBB\x39\x9E\x0F\x71\xD0"
                             "\x61\xED\x30\x16\x5D\x1E\x61\x33\x14\x08\x38\x05\x85\xA3\xDC\x08\x33\x0F\x71\xD0"
                             "\xD4\x08\x56\xFF\xA3\xC2\x81\x22\xE0\x20\xCD\x3D\xC7\x4F\x82\x17\x20\x60\x8D\xC7"
                             "\xD3\x1A\x78\x19\x62\x09\xBC\x3C\x79\x9C\xA2\x18\x6C\x3C\x0D\xBF\x8F\x6F\x1E\x67"
                             "\x38\x86\x1B\x11\xCA\x21\x86\xC3\xC1\xEE";
#define  HTTP_HEAD_STYLE2       Decompress(HTTP_HEAD_STYLE2_COMPRESSED,HTTP_HEAD_STYLE2_SIZE).c_str()
#else
const char HTTP_HEAD_STYLE2[] PROGMEM =
  "button{border:0;border-radius:0.3rem;background:#%06x;color:#%06x;line-height:2.4rem;font-size:1.2rem;width:100%%;-webkit-transition-duration:0.4s;transition-duration:0.4s;cursor:pointer;}"  // COLOR_BUTTON, COLOR_BUTTON_TEXT
  "button:hover{background:#%06x;}"  // COLOR_BUTTON_HOVER
  ".bred{background:#%06x;}"  // COLOR_BUTTON_RESET
  ".bred:hover{background:#%06x;}"  // COLOR_BUTTON_RESET_HOVER
  ".bgrn{background:#%06x;}"  // COLOR_BUTTON_SAVE
  ".bgrn:hover{background:#%06x;}"  // COLOR_BUTTON_SAVE_HOVER
  "a{color:#%06x;text-decoration:none;}"  // COLOR_BUTTON
  ".p{float:left;text-align:left;}"
  ".q{float:right;text-align:right;}"
  ".r{border-radius:0.3em;padding:2px;margin:6px 2px;}";
#endif //USE_UNISHOX_COMPRESSION

#ifdef USE_ZIGBEE
// Styles used for Zigbee Web UI
// Battery icon from https://css.gg/battery
//
#ifdef USE_UNISHOX_COMPRESSION

const size_t HTTP_HEAD_STYLE_ZIGBEE_SIZE = 363;
const char HTTP_HEAD_STYLE_ZIGBEE_COMPRESSED[] PROGMEM = "\x3A\x0E\xA3\xDA\x3B\x0D\x87\x5F\xB4\xDB\xBC\x3C\x79\x8E\xCF\x88\xFE\x75\x8E\xC3"
                             "\x61\xE0\x66\x7B\x6B\x73\x8F\x3F\xB0\xAE\xB4\xCD\x9E\x04\xDF\x0C\x0A\xCC\x8F\x3D"
                             "\xE0\xB7\x99\xD6\x38\x2C\x0C\xD0\xF0\x3F\xA2\x50\xA3\xCC\xE5\x32\x18\x6C\x3C\x0A"
                             "\x7A\x3C\x2A\x2B\x8F\x33\x92\x88\x61\xB0\xF0\x08\x39\x29\xE6\x72\x88\x61\xB1\x7B"
                             "\x02\xD1\x01\x0A\x69\xD7\xFB\x13\x45\xF8\xF3\x39\x64\x30\xD8\x78\x1B\x7F\x1E\xDE"
                             "\x3A\xC2\x66\x28\xF3\x3A\xCE\x59\x0C\x36\x1E\xE3\xA0\xEA\x3C\xCF\x3B\x31\x4F\xE7"
                             "\x51\xD0\x75\x1E\x67\x98\xE6\x63\x3E\xCF\x68\x79\xD4\xFA\x8F\x33\xD8\x7B\x01\x13"
                             "\x5E\x04\x1D\x5C\x16\xB8\x14\xB1\xDE\xC0\x85\xD3\x04\x3D\xD0\xE7\x10\xC3\x61\xE0"
                             "\x75\x86\x68\x3D\xFC\x17\xC2\x1E\x61\x8B\xFF\xDF\x51\x07\x81\x67\xCF\x15\x83\x0F"
                             "\x33\x90\x81\x0F\x5F\x04\x2D\x53\xFA\x3C\x2A\x2B\x8F\x33\xAC\xE6\x10\x22\x70\x54"
                             "\x08\xFC\x0C\x82\x0F\x0A\x67\x30\x81\x23\x81\x23\xDA\x08\x34\x4C\xEF\xE7\x74\xEB"
                             "\x3A\xC7\x04\x75\x1C\x98\x43\x0D\x87\x78\xF0\x13\x31\x47\x99\xC8\x43\x0D\x87\xB8";

// Raw: .bt{box-sizing:border-box;position:relative;display:inline-block;width:20px;height:12px;border:2px solid;border-radius:3px;margin-left:-3px}.bt::after,.bt::before{content:"";display:block;box-sizing:border-box;position:absolute;height:6px;background:currentColor;top:1px}.bt::before{right:-4px;border-radius:3px;width:4px}.bt::after{width:var(--bl,14px);left:1px}
// Successfully compressed from 363 to 240 bytes (-33.9%)
#define  HTTP_HEAD_STYLE_ZIGBEE       Decompress(HTTP_HEAD_STYLE_ZIGBEE_COMPRESSED,HTTP_HEAD_STYLE_ZIGBEE_SIZE).c_str()
#else // USE_UNISHOX_COMPRESSION
const char HTTP_HEAD_STYLE_ZIGBEE[] PROGMEM =
  ".bt{box-sizing:border-box;position:relative;display:inline-block;width:20px;height:12px;border:2px solid;border-radius:3px;margin-left:-3px}"
  ".bt::after,.bt::before{content:\"\";display:block;box-sizing:border-box;position:absolute;height:6px;background:currentColor;top:1px}"
  ".bt::before{right:-4px;border-radius:3px;width:4px}"
  ".bt::after{width:var(--bl,14px);left:1px}";
#endif // USE_UNISHOX_COMPRESSION
#endif // USE_ZIGBEE

const char HTTP_HEAD_STYLE3[] PROGMEM =
  "</style>"
  "<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/@mdi/font@5.8.55/css/materialdesignicons.min.css\">"
  "<script type=\"text/javascript\" src=\"https://cdnjs.cloudflare.com/ajax/libs/ace/1.4.12/ace.min.js\"></script>"
  "<script type=\"text/javascript\" src=\"https://cdnjs.cloudflare.com/ajax/libs/yamljs/0.3.0/yaml.min.js\"></script>"
  "</head>"
  "<body>"
  "<div style='text-align:left;display:inline-block;color:#%06x;min-width:340px;'>"  // COLOR_TEXT
#ifdef FIRMWARE_MINIMAL
  "<div style='text-align:center;color:#%06x;'><h3>" D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "</h3></div>"  // COLOR_TEXT_WARNING
#endif
  "<div style='text-align:center;color:#%06x;'><noscript>" D_NOSCRIPT "<br></noscript>" // COLOR_TITLE
// #ifdef LANGUAGE_MODULE_NAME
//   "<h3>" D_MODULE " %s</h3>"
// #else
//   "<h3>%s " D_MODULE "</h3>"
// #endif
  "<h3>%s</h3>";

const char HTTP_MSG_SLIDER_GRADIENT[] PROGMEM =
  "<div id='%s' class='r' style='background-image:linear-gradient(to right,%s,%s);'>"
  "<input id='sl%d' type='range' min='%d' max='%d' value='%d' onchange='lc(\"%c\",%d,value)'>"
  "</div>";
const char HTTP_MSG_SLIDER_SHUTTER[] PROGMEM =
  "<div><span class='p'>" D_CLOSE "</span><span class='q'>" D_OPEN "</span></div>"
  "<div><input type='range' min='0' max='100' value='%d' onchange='lc(\"u\",%d,value)'></div>";

const char HTTP_MSG_RSTRT[] PROGMEM =
  "<br><div style='text-align:center;'>" D_DEVICE_WILL_RESTART "</div><br>";

const char HTTP_FORM_LOGIN[] PROGMEM =
  "<fieldset>"
  "<form method='post' action='/'>"
  "<p><b>" D_USER "</b><br><input name='USER1' placeholder='" D_USER "'></p>"
  "<p><b>" D_PASSWORD "</b><br><input name='PASS1' type='password' placeholder='" D_PASSWORD "'></p>"
  "<br>"
  "<button>" D_OK "</button>"
  "</form></fieldset>";

const char HTTP_FORM_TEMPLATE[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_TEMPLATE_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='tp'>";
const char HTTP_FORM_TEMPLATE_FLAG[] PROGMEM =
  "<p></p>"  // Keep close so do not use <br>
  "<fieldset><legend><b>&nbsp;" D_TEMPLATE_FLAGS "&nbsp;</b></legend><p>"
//  "<label><input id='c0' name='c0' type='checkbox'><b>" D_OPTION_TEXT "</b></label><br>"
  "</p></fieldset>";

const char HTTP_FORM_MODULE[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_MODULE_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='md'>"
  "<p></p><b>" D_MODULE_TYPE "</b> (%s)<br><select id='g99'></select><br>"
  "<br><table>";

const char HTTP_FORM_WIFI[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_WIFI_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='wi'>"
  "<p><b>" D_AP1_SSID "</b> (" WIFI_STA_SSID1 ")<br><input id='s1' placeholder=\"" WIFI_STA_SSID1 "\" value=\"%s\"></p>"  // Need \" instead of ' to be able to use ' in text (#8489)
  "<p><label><b>" D_AP1_PASSWORD "</b><input type='checkbox' onclick='sp(\"p1\")'></label><br><input id='p1' type='password' placeholder=\"" D_AP1_PASSWORD "\" value=\"" D_ASTERISK_PWD "\"></p>"
  "<p><b>" D_AP2_SSID "</b> (" WIFI_STA_SSID2 ")<br><input id='s2' placeholder=\"" WIFI_STA_SSID2 "\" value=\"%s\"></p>"
  "<p><label><b>" D_AP2_PASSWORD "</b><input type='checkbox' onclick='sp(\"p2\")'></label><br><input id='p2' type='password' placeholder=\"" D_AP2_PASSWORD "\" value=\"" D_ASTERISK_PWD "\"></p>"
  "<p><b>" D_HOSTNAME "</b> (%s)<br><input id='h' placeholder=\"%s\" value=\"%s\"></p>"
  "<p><b>" D_CORS_DOMAIN "</b><input id='c' placeholder=\"" CORS_DOMAIN "\" value=\"%s\"></p>";

const char HTTP_FORM_LOG1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_LOGGING_PARAMETERS "&nbsp;</b>"
  "</legend><form method='get' action='lg'>";
const char HTTP_FORM_LOG2[] PROGMEM =
  "<p><b>" D_SYSLOG_HOST "</b> (" SYS_LOG_HOST ")<br><input id='lh' placeholder=\"" SYS_LOG_HOST "\" value=\"%s\"></p>"
  "<p><b>" D_SYSLOG_PORT "</b> (" STR(SYS_LOG_PORT) ")<br><input id='lp' placeholder='" STR(SYS_LOG_PORT) "' value='%d'></p>"
  "<p><b>" D_TELEMETRY_PERIOD "</b> (" STR(TELE_PERIOD) ")<br><input id='lt' placeholder='" STR(TELE_PERIOD) "' value='%d'></p>";

const char HTTP_FORM_OTHER[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_OTHER_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='co'>"
  "<p></p>"
  "<label><b>" D_WEB_ADMIN_USERNAME "</b></label><br><input id='wu' placeholder=\"\" value=\"%s\"><br>"
  "<br>"
  "<label><b>" D_WEB_ADMIN_PASSWORD "</b><input type='checkbox' onclick='sp(\"wp\")'></label><br><input id='wp' type='password' placeholder=\"" D_WEB_ADMIN_PASSWORD "\" value=\"" D_ASTERISK_PWD "\"><br>"
  "<br>"
  "<label><b>" D_DEVICE_NAME "</b> (%s)</label><br><input id='dn' placeholder=\"\" value=\"%s\"><br>"
  "<br>";

const char HTTP_FORM_END[] PROGMEM =
  "<br>"
  "<button name='save' type='submit' class='button bgrn'>" D_SAVE "</button>"
  "</form></fieldset>";

const char HTTP_FORM_RST[] PROGMEM =
  "<div id='f1' style='display:block;'>"
  "<fieldset><legend><b>&nbsp;" D_RESTORE_CONFIGURATION "&nbsp;</b></legend>";
const char HTTP_FORM_UPG[] PROGMEM =
  "<div id='f1' style='display:block;'>"
  "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_WEBSERVER "&nbsp;</b></legend>"
  "<form method='get' action='u1'>"
  "<br><b>" D_OTA_URL "</b><br><input id='o' placeholder=\"OTA_URL\" value=\"%s\"><br>"
  "<br><button type='submit'>" D_START_UPGRADE "</button></form>"
  "</fieldset><br><br>"
  "<fieldset><legend><b>&nbsp;" D_UPGRADE_BY_FILE_UPLOAD "&nbsp;</b></legend>";
const char HTTP_FORM_RST_UPG[] PROGMEM =
  "<form method='post' action='u2' enctype='multipart/form-data'>"
  "<br><input type='file' name='u2'><br>"
  "<br><button type='submit' onclick='eb(\"f1\").style.display=\"none\";eb(\"f2\").style.display=\"block\";this.form.submit();'>" D_START " %s</button></form>"
  "</fieldset>"
  "</div>"
  "<div id='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED " ...</b></div>";

const char HTTP_FORM_CMND[] PROGMEM =
  "<br><textarea readonly id='t1' cols='340' wrap='off'></textarea><br><br>"
  "<form method='get' onsubmit='return l(1);'>"
  "<input id='c1' placeholder='" D_ENTER_COMMAND "' autofocus><br>"
  //  "<br><button type='submit'>Send command</button>"
  "</form>";

const char HTTP_TABLE100[] PROGMEM =
  "<table style='width:100%%'>";

const char HTTP_COUNTER[] PROGMEM =
  "<br><div id='t' style='text-align:center;'></div>";

const char HTTP_EDITOR[] PROGMEM =
            "<div class=\"navbar\">"
            "<span class=\"mdi mdi-close ic btn\" onclick=\"rC()\" style=\"color:#753535;\"></span>"
            "<span id=\"cas\" class=\"btnG tabb\"></span>"
            "<span class=\"mdi mdi-plus ic btn\" onclick=\"aC()\" style=\"color:#357535;float: right;\"></span>"
            "</div>"
            "<pre id=\"ed\" style=\"margin: auto;\"></pre>"
            "<span id=\"screen\" class='screen'></span>";

const char HTTP_END[] PROGMEM =
  "<div style='text-align:right;font-size:11px;'><hr/><a href='https://bit.ly/tasmota' target='_blank' style='color:#aaa;'>" PROJECT " %s " D_BY " " PROJECT_OWNER "</a></div>"
  "</div>"
  "</body>"
  "</html>";

const char HTTP_DEVICE_CONTROL[] PROGMEM = "<td style='width:%d%%'><button onclick='la(\"&o=%d\");'>%s%s</button></td>";  // ?o is related to WebGetArg("o", tmp, sizeof(tmp));
const char HTTP_DEVICE_STATE[] PROGMEM = "<td style='width:%d{c}%s;font-size:%dpx'>%s</div></td>";  // {c} = %'><div style='text-align:center;font-weight:

enum ButtonTitle {
  BUTTON_RESTART, BUTTON_RESET_CONFIGURATION,
  BUTTON_MAIN, BUTTON_CONFIGURATION, BUTTON_INFORMATION, BUTTON_FIRMWARE_UPGRADE, BUTTON_CONSOLE,
  BUTTON_MODULE, BUTTON_WIFI, BUTTON_CARDS, BUTTON_LOGGING, BUTTON_OTHER, BUTTON_TEMPLATE, BUTTON_BACKUP, BUTTON_RESTORE };
const char kButtonTitle[] PROGMEM =
  D_RESTART "|" D_RESET_CONFIGURATION "|"
  D_MAIN_MENU "|" D_CONFIGURATION "|" D_INFORMATION "|" D_FIRMWARE_UPGRADE "|" D_CONSOLE "|"
  D_CONFIGURE_MODULE "|" D_CONFIGURE_WIFI"|" D_CONFIGURE_CARDS"|" D_CONFIGURE_LOGGING "|" D_CONFIGURE_OTHER "|" D_CONFIGURE_TEMPLATE "|" D_BACKUP_CONFIGURATION "|" D_RESTORE_CONFIGURATION;
const char kButtonAction[] PROGMEM =
  ".|rt|"
  ".|cn|in|up|cs|"
  "md|wi|ca|lg|co|tp|dl|rs";
const char kButtonConfirm[] PROGMEM = D_CONFIRM_RESTART "|" D_CONFIRM_RESET_CONFIGURATION;

enum CTypes { CT_HTML, CT_PLAIN, CT_XML, CT_JSON, CT_STREAM };
const char kContentTypes[] PROGMEM = "text/html|text/plain|text/xml|application/json|application/octet-stream";

const char kLoggingOptions[] PROGMEM = D_SERIAL_LOG_LEVEL "|" D_WEB_LOG_LEVEL "|" D_SYS_LOG_LEVEL;
const char kLoggingLevels[] PROGMEM = D_NONE "|" D_ERROR "|" D_INFO "|" D_DEBUG "|" D_MORE_DEBUG;

const char kEmulationOptions[] PROGMEM = D_NONE "|" D_BELKIN_WEMO "|" D_HUE_BRIDGE;

const char kUploadErrors[] PROGMEM =
  D_UPLOAD_ERR_1 "|" D_UPLOAD_ERR_2 "|" D_UPLOAD_ERR_3 "|" D_UPLOAD_ERR_4 "|" D_UPLOAD_ERR_5 "|" D_UPLOAD_ERR_6 "|" D_UPLOAD_ERR_7 "|" D_UPLOAD_ERR_8 "|" D_UPLOAD_ERR_9
#ifdef USE_RF_FLASH
  "|" D_UPLOAD_ERR_10 "|" D_UPLOAD_ERR_11 "|" D_UPLOAD_ERR_12 "|" D_UPLOAD_ERR_13
#endif
  "|" D_UPLOAD_ERR_14
  ;

const uint16_t DNS_PORT = 53;
enum HttpOptions {HTTP_OFF, HTTP_USER, HTTP_ADMIN, HTTP_MANAGER, HTTP_MANAGER_RESET_ONLY};

DNSServer *DnsServer;
AsyncWebServer *webServer;

struct WEB {
  String chunk_buffer = "";                         // Could be max 2 * CHUNKED_BUFFER_SIZE
  bool reset_web_log_flag = false;                  // Reset web console log
  uint8_t state = HTTP_OFF;
  uint8_t upload_error = 0;
  uint8_t upload_file_type;
  uint8_t upload_progress_dot_count;
  uint8_t config_block_count = 0;
  uint8_t config_xor_on = 0;
  uint8_t config_xor_on_set = 0x5A;
} Web;

fs::File file;

static bool WifiIsInManagerMode() {
  return (HTTP_MANAGER == Web.state || HTTP_MANAGER_RESET_ONLY == Web.state);
}

// void ShowWebSource(uint32_t source) {
//   if ((source > 0) && (source < SRC_MAX)) {
//     char stemp1[20];
//     Log::log(LOG_LEVEL_DEBUG, PSTR("SRC: %s from %s"), Support::getTextIndexed(stemp1, sizeof(stemp1), source, kCommandSource), webServer->client().remoteIP().toString().c_str());
//   }
// }

// void ExecuteWebCommand(char* svalue, uint32_t source)
// {
//   ShowWebSource(source);
//   TasmotaGlobal.last_source = source;
//   ExecuteCommand(svalue, SRC_IGNORE);
// }

// replace the series of `webServer->on()` with a table in PROGMEM
typedef struct webServerDispatch_t {
  char uri[3];   // the prefix "/" is added automatically
  uint8_t method;
  void (*handler)(AsyncWebServerRequest *request);
} webServerDispatch_t;

const webServerDispatch_t webServerDispatch[] PROGMEM = {
  { "",   HTTP_ANY, HandleRoot },
  { "up", HTTP_ANY, HandleUpgradeFirmware },
  { "u1", HTTP_ANY, HandleUpgradeFirmwareStart },   // OTA
  { "u2", HTTP_OPTIONS, HandlePreflightRequest },
  { "u3", HTTP_ANY, HandleUploadDone },
  { "cs", HTTP_GET, HandleConsole },
  { "cs", HTTP_OPTIONS, HandlePreflightRequest },
  { "cm", HTTP_ANY, HandleHttpCommand },
#ifndef FIRMWARE_MINIMAL
  { "cn", HTTP_ANY, HandleConfiguration },
//  { "md", HTTP_ANY, HandleModuleConfiguration },
  { "wi", HTTP_ANY, HandleWifiConfiguration },
//  { "ca", HTTP_ANY, HandleCardsConfiguration },
  { "lg", HTTP_ANY, HandleLoggingConfiguration },
//  { "tp", HTTP_ANY, HandleTemplateConfiguration },
  { "co", HTTP_ANY, HandleOtherConfiguration },
  { "dl", HTTP_ANY, HandleBackupConfiguration },
  { "rs", HTTP_ANY, HandleRestoreConfiguration },
  { "rt", HTTP_ANY, HandleResetConfiguration },
  { "in", HTTP_ANY, HandleInformation },
#endif  // Not FIRMWARE_MINIMAL
};

void startwebServer(int type, IPAddress ipweb) {
  if (!Web.state) {
    if (!webServer) {
      webServer = new AsyncWebServer((HTTP_MANAGER == type || HTTP_MANAGER_RESET_ONLY == type) ? 80 : WEB_PORT);
      // call `webServer->on()` on each entry
      for (uint32_t i=0; i<ARRAY_SIZE(webServerDispatch); i++) {
        const webServerDispatch_t & line = webServerDispatch[i];
        // copy uri in RAM and prefix with '/'
        char uri[4];
        uri[0] = '/';
        uri[1] = pgm_read_byte(&line.uri[0]);
        uri[2] = pgm_read_byte(&line.uri[1]);
        uri[3] = '\0';
        // register
        webServer->on(uri, line.method, line.handler);
      }
      webServer->onNotFound(HandleNotFound);
      webServer->serveStatic("/cards.json", SPIFFS, "/Main.json");
      webServer->serveStatic("/cards.min.js", SPIFFS, "/www/cards.min.js");
      webServer->serveStatic("/sw-on.jpg", SPIFFS, "/img/sw-on.jpg");
      webServer->serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
      #ifndef FIRMWARE_MINIMAL
      webServer->on("/ca", HTTP_ANY, HandleCardsConfiguration, [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
        }, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
          if(!index){
            file = SPIFFS.open("/Main.json", "w");
            Serial.printf("BodyStart: %u B\n", total);
          }
          for(size_t i=0; i<len; i++){
            file.write(data[i]);
            Serial.write(data[i]);
          }
          if(index + len == total){
            file.close();
            Serial.printf("BodyEnd: %u B\n", total);
          }
        });
      #endif
      webServer->on("/u2", HTTP_POST, HandleUploadDone, HandleUploadLoop);
#ifndef FIRMWARE_MINIMAL
      //TODO: Useful if others neeed to add handlers
      // XdrvCall(FUNC_WEB_ADD_HANDLER);
      // XsnsCall(FUNC_WEB_ADD_HANDLER);
#endif  // Not FIRMWARE_MINIMAL
    }
    Web.reset_web_log_flag = false;

    webServer->begin(); // Web server start
  }
  if (Web.state != type) {
    Log::log(LOG_LEVEL_INFO, D_LOG_HTTP D_WEBSERVER_ACTIVE_ON " %s " D_WITH_IP_ADDRESS " %s",
      settings.getHostname(), ipweb.toString().c_str());
    globalParams.setHttpEnabled(true);
  }
  if (type) { Web.state = type; }
}

void StopwebServer(void) {
    if (Web.state) {
        webServer->end();
        Web.state = HTTP_OFF;
        Log::log(LOG_LEVEL_INFO, D_LOG_HTTP D_WEBSERVER_STOPPED);
    }
}

void WifiManagerBegin(bool reset_only) {
  // setup AP
  if (globalParams.isWifiUp()) {
    wifiManager.setMode(WIFI_AP_STA);
    Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION));
  } else {
    wifiManager.setMode(WIFI_AP);
    Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT));
  }

  StopwebServer();
  globalParams.setWifiApMode(true);
  DnsServer = new DNSServer();

  int channel = WIFI_SOFT_AP_CHANNEL;
  if ((channel < 1) || (channel > 13)) { channel = 1; }
  Log::log(LOG_LEVEL_DEBUG, D_LOG_WIFI "Ap SSID is %s\n AP password is %s\n", settings.getApWifiSsid(), settings.getApWifiPassword());
  // bool softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0, int max_connection = 4);
  WiFi.softAP(settings.getApWifiSsid(), settings.getApWifiPassword(), channel, 0, 1);
  delay(500); // Without delay I've seen the IP address blank
  /* Setup the DNS server redirecting all the domains to the apIP */
  DnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  DnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

  startwebServer((reset_only ? HTTP_MANAGER_RESET_ONLY : HTTP_MANAGER), WiFi.softAPIP());
}

void Webserver_loop() {
  if (DnsServer) { DnsServer->processNextRequest(); }
}

/*********************************************************************************************/

bool WebAuthenticate(AsyncWebServerRequest *request) {
  if (strlen(settings.getWebPassword()) && (HTTP_MANAGER_RESET_ONLY != Web.state)) {
    return request->authenticate(settings.getWebUsername(), settings.getWebPassword(), WEB_USER_REALM, false);
  } else {
    return true;
  }
}

bool HttpCheckPriviledgedAccess(AsyncWebServerRequest *request, bool autorequestauth = true) {
  if (HTTP_USER == Web.state) {
    HandleRoot(request);
    return false;
  }
  if (autorequestauth && !WebAuthenticate(request)) {
    request->requestAuthentication(WEB_USER_REALM, false);
    return false;
  }
  return true;
}

void HttpHeaderCors(AsyncResponseStream *response) {
  if (strlen(settings.getWifiCorsHeader())) {
    response->addHeader(F("Access-Control-Allow-Origin"), settings.getWifiCorsHeader());
  }
}

void WSHeaderSend(AsyncResponseStream *response) {
  response->addHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));
  response->addHeader(F("Pragma"), F("no-cache"));
  response->addHeader(F("Expires"), F("-1"));
  HttpHeaderCors(response);
}

/**********************************************************************************************
* HTTP Content Page handler
**********************************************************************************************/

// void WSSend(int code, int ctype, const String& content) {
//   char ct[25];  // strlen("application/octet-stream") +1 = Longest Content type string
//   webServer->send(code, Support::getTextIndexed(ct, sizeof(ct), ctype, kContentTypes), content);
// }

/**********************************************************************************************
* HTTP Content Chunk handler
**********************************************************************************************/

AsyncResponseStream * WSContentBegin(AsyncWebServerRequest *request, int code, const char * ctype) {
  AsyncResponseStream * response = request->beginResponseStream(ctype);
  WSHeaderSend(response);
  response->setContentLength(((size_t) -1));
  return response;
}

// // Low level sendContent for all core versions
// void _WSContentSend(AsyncWebServerRequest *request, const String& content) {
//   size_t len = content.length();
//   //request->send(content.c_str());

// #ifdef USE_DEBUG_DRIVER
//   ShowFreeMem(PSTR("WSContentSend"));
// #endif
//   //Log::log(LOG_LEVEL_DEBUG, PSTR("WEB: Chunk size %d/%d"), len, sizeof(TasmotaGlobal.mqtt_data));
// }

// void WSContentFlush(void)
// {
//   if (Web.chunk_buffer.length() > 0) {
//     _WSContentSend(Web.chunk_buffer);                  // Flush chunk buffer
//     Web.chunk_buffer = "";
//   }
// }

// void _WSContentSendBuffer(void) {
//   int len = strlen(TasmotaGlobal.mqtt_data);

//   if (0 == len) {                                  // No content
//     return;
//   }
//   else if (len == sizeof(TasmotaGlobal.mqtt_data)) {
//     Log::log(LOG_LEVEL_INFO, PSTR("HTP: Content too large"));
//   }
//   else if (len < CHUNKED_BUFFER_SIZE) {            // Append chunk buffer with small content
//     Web.chunk_buffer += TasmotaGlobal.mqtt_data;
//     len = Web.chunk_buffer.length();
//   }

//   if (len >= CHUNKED_BUFFER_SIZE) {                // Either content or chunk buffer is oversize
//     WSContentFlush();                              // Send chunk buffer before possible content oversize
//   }
//   if (strlen(TasmotaGlobal.mqtt_data) >= CHUNKED_BUFFER_SIZE) {  // Content is oversize
//     _WSContentSend(TasmotaGlobal.mqtt_data);                     // Send content
//   }
// }


// // Content send snprintf_P char data
// void response->print(const char* formatP, ...)      {
//   // This uses char strings. Be aware of sending %% if % is needed
//   va_list arg;
//   va_start(arg, formatP);
//   int len = vsnprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), formatP, arg);
//   va_end(arg);

// #ifdef DEBUG_TASMOTA_CORE
//   if (len > (sizeof(TasmotaGlobal.mqtt_data) -1)) {
//     TasmotaGlobal.mqtt_data[33] = '\0';
//     Log::log(LOG_LEVEL_DEBUG, PSTR("ERROR: response->print size %d > mqtt_data size %d. Start of data [%s...]"), len, sizeof(TasmotaGlobal.mqtt_data), TasmotaGlobal.mqtt_data);
//   }
// #endif

//   _WSContentSendBuffer();
// }

// // Content send snprintf_P char data checked for decimal separator
// void response->printD(const char* formatP, ...)   {
//   // This uses char strings. Be aware of sending %% if % is needed
//   va_list arg;
//   va_start(arg, formatP);
//   int len = vsnprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), formatP, arg);
//   va_end(arg);

// #ifdef DEBUG_TASMOTA_CORE
//   if (len > (sizeof(TasmotaGlobal.mqtt_data) -1)) {
//     TasmotaGlobal.mqtt_data[33] = '\0';
//     Log::log(LOG_LEVEL_DEBUG, PSTR("ERROR: response->printD size %d > mqtt_data size %d. Start of data [%s...]"), len, sizeof(TasmotaGlobal.mqtt_data), TasmotaGlobal.mqtt_data);
//   }
// #endif

//   if (D_DECIMAL_SEPARATOR[0] != '.') {
//     for (uint32_t i = 0; i < len; i++) {
//       if ('.' == TasmotaGlobal.mqtt_data[i]) {
//         TasmotaGlobal.mqtt_data[i] = D_DECIMAL_SEPARATOR[0];
//       }
//     }
//   }

//   _WSContentSendBuffer();
// }

void requestAuthentication(AsyncWebServerRequest *request) {
  if (strlen(settings.getWebPassword()) && !request->authenticate(settings.getWebUsername(), settings.getWebPassword())) {
    request->requestAuthentication(WEB_USER_REALM, false);
  }
}

void WSContentSendStyle(AsyncResponseStream * response) {
  if (WifiIsInManagerMode()) {
    if (wifiManager.getConfigCounter()) {
      response->print(HTTP_SCRIPT_COUNTER);
    }
  }
  response->print(HTTP_HEAD_LAST_SCRIPT);
  response->print(HTTP_SCRIPT_CARDS);
  WebTheme * theme = settings.getWebTheme();
  response->printf(HTTP_HEAD_STYLE1, theme->colorForm.color, theme->colorInput.color, theme->colorInputText.color, theme->colorInput.color,
                  theme->colorInputText.color, theme->colorConsole.color, theme->colorConsoleText.color, theme->colorBackground.color);
  response->printf(HTTP_HEAD_STYLE2, theme->colorButton.color, theme->colorButtonText.color, theme->colorButtonHover.color,
                  theme->colorButtonReset.color, theme->colorButtonResetHover.color, theme->colorButtonSave.color, theme->colorButtonSaveHover.color,
                  theme->colorButton.color);
  response->print(HTTP_STYLE_CARDS);
#ifdef USE_ZIGBEE
  response->print(HTTP_HEAD_STYLE_ZIGBEE);
#endif // USE_ZIGBEE
//   if (formatP != nullptr) {
//     // This uses char strings. Be aware of sending %% if % is needed
//     va_list arg;
//     va_start(arg, formatP);
//     int len = vsnprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), formatP, arg);
//     va_end(arg);

// #ifdef DEBUG_TASMOTA_CORE
//   if (len > (sizeof(TasmotaGlobal.mqtt_data) -1)) {
//     TasmotaGlobal.mqtt_data[33] = '\0';
//     Log::log(LOG_LEVEL_DEBUG, PSTR("ERROR: WSContentSendStyle_P size %d > mqtt_data size %d. Start of data [%s...]"), len, sizeof(TasmotaGlobal.mqtt_data), TasmotaGlobal.mqtt_data);
//   }
// #endif

//     _WSContentSendBuffer();
//   }
  response->printf(HTTP_HEAD_STYLE3, theme->colorText.color,
#ifdef FIRMWARE_MINIMAL
  theme->colorTextWarning.color,
#endif
    theme->colorTitle.color,
    settings.getDeviceName());
  if (settings.getShowIpGui()) {                // SetOption53 - Show hostanme and IP address in GUI main menu
    bool lip = (static_cast<uint32_t>(WiFi.localIP()) != 0);
    bool sip = (static_cast<uint32_t>(WiFi.softAPIP()) != 0);
    response->printf(PSTR("<h4>%s%s (%s%s%s)</h4>"),    // tasmota.local (192.168.2.12, 192.168.4.1)
      settings.getHostname(),
      "",
      (lip) ? WiFi.localIP().toString().c_str() : "",
      (lip && sip) ? ", " : "",
      (sip) ? WiFi.softAPIP().toString().c_str() : "");
  }
  response->print(PSTR("</div>"));
}

void WSContentButton(AsyncResponseStream * response, uint32_t title_index) {
  char action[4];
  char title[100];  // Large to accomodate UTF-16 as used by Russian

  if (title_index <= BUTTON_RESET_CONFIGURATION) {
    char confirm[100];
    response->printf(PSTR("<p><form action='%s' method='get' onsubmit='return confirm(\"%s\");'><button name='%s' class='button bred'>%s</button></form></p>"),
      Support::Support::getTextIndexed(action, sizeof(action), title_index, kButtonAction),
      Support::Support::getTextIndexed(confirm, sizeof(confirm), title_index, kButtonConfirm),
      (!title_index) ? "rst" : "non",
      Support::Support::getTextIndexed(title, sizeof(title), title_index, kButtonTitle));
  } else {
    response->printf(PSTR("<p><form action='%s' method='get'><button>%s</button></form></p>"),
      Support::Support::getTextIndexed(action, sizeof(action), title_index, kButtonAction),
      Support::Support::getTextIndexed(title, sizeof(title), title_index, kButtonTitle));
  }
}

void WSContentSpaceButton(AsyncResponseStream * response, uint32_t title_index) {
  response->print(PSTR("<div></div>"));            // 5px padding
  WSContentButton(response, title_index);
}

void WSContentStop(AsyncResponseStream * response) {
  if (WifiIsInManagerMode()) {
    if (wifiManager.getConfigCounter()) {
      response->print(HTTP_COUNTER);
    }
  }
  response->printf(HTTP_END, settings.getVersionString().c_str());
}

/*********************************************************************************************/

void WebRestart(AsyncWebServerRequest *request, uint32_t type) {
  // type 0 = restart
  // type 1 = restart after config change
  // type 2 = restart after config change with possible ip address change too
  Log::log(LOG_LEVEL_DEBUG, D_LOG_HTTP D_RESTART);

  bool reset_only = (HTTP_MANAGER_RESET_ONLY == Web.state);
  if (!reset_only)
    requestAuthentication(request);
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), (type) ? S_SAVE_CONFIGURATION : S_RESTART);
  response->printf(HTTP_SCRIPT_RELOAD_TIME, HTTP_RESTART_RECONNECT_TIME);
  WSContentSendStyle(response);
  if (type) {
    response->print(PSTR("<div style='text-align:center;'><b>" D_CONFIGURATION_SAVED "</b><br>"));
    if (2 == type) {
      response->print(PSTR("<br>" D_TRYING_TO_CONNECT "<br>"));
    }
    response->print(PSTR("</div>"));
  }
  response->print(HTTP_MSG_RSTRT);
  if (HTTP_MANAGER == Web.state || reset_only) {
    Web.state = HTTP_ADMIN;
  } else {
    WSContentSpaceButton(response, BUTTON_MAIN);
  }
  WSContentStop(response);
  request->send(response);
  globalParams.setRestartFlag(2);
}

/*********************************************************************************************/

void HandleWifiLogin(AsyncWebServerRequest *request) {
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE,  settings.getDeviceName(), S_CONFIGURE_WIFI); // false means show page no matter if the client has or has not credentials
  WSContentSendStyle(response);
  response->print(HTTP_FORM_LOGIN);

  if (HTTP_MANAGER_RESET_ONLY == Web.state) {
    WSContentSpaceButton(response, BUTTON_RESTART);
#ifndef FIRMWARE_MINIMAL
    WSContentSpaceButton(response, BUTTON_RESET_CONFIGURATION);
#endif  // FIRMWARE_MINIMAL
  }

  WSContentStop(response);
  request->send(response);
}

#ifdef USE_LIGHT
void WebSliderColdWarm(void)
{
  response->print(HTTP_MSG_SLIDER_GRADIENT,  // Cold Warm
    "a",             // a - Unique HTML id
    "#eff", "#f81",  // 6500k in RGB (White) to 2500k in RGB (Warm Yellow)
    1,               // sl1
    153, 500,        // Range color temperature
    LightGetColorTemp(),
    't', 0);         // t0 - Value id releated to lc("t0", value) and WebGetArg("t0", tmp, sizeof(tmp));
}
#endif  // USE_LIGHT

void HandleRoot(AsyncWebServerRequest *request) {
  Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP "Root (%s)"), request->url().c_str());
  if (CaptivePortal(request)) { return; }  // If captive portal redirect instead of displaying the page.

  if (request->hasArg("rst")) {
    WebRestart(request, 0);
    return;
  }

  if (WifiIsInManagerMode()) {
#ifndef FIRMWARE_MINIMAL
    if (strlen(settings.getWebPassword()) && !(request->hasArg("USER1")) && !(request->hasArg("PASS1")) && HTTP_MANAGER_RESET_ONLY != Web.state) {
      HandleWifiLogin(request);
    } else {
      if (!strlen(settings.getWebPassword()) || (((request->arg("USER1") == WEB_USERNAME ) && (request->arg("PASS1") == String(settings.getWebPassword()) )) || HTTP_MANAGER_RESET_ONLY == Web.state)) {
        HandleWifiConfiguration(request);
      } else {
        HandleWifiLogin(request);
      }
    }
#endif  // Not FIRMWARE_MINIMAL
    return;
  }

  Log::log(LOG_LEVEL_DEBUG, D_LOG_HTTP D_MAIN_MENU);
  requestAuthentication(request);
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), S_MAIN_MENU);
  WSContentSendStyle(response);
// #ifndef FIRMWARE_MINIMAL
//   XdrvCall(FUNC_WEB_ADD_MAIN_BUTTON);
//   XsnsCall(FUNC_WEB_ADD_MAIN_BUTTON);
// #endif  // Not FIRMWARE_MINIMAL

  if (HTTP_ADMIN == Web.state) {
#ifdef FIRMWARE_MINIMAL
    WSContentSpaceButton(response, BUTTON_FIRMWARE_UPGRADE);
#else
    WSContentSpaceButton(response, BUTTON_CONFIGURATION);
    WSContentButton(response, BUTTON_INFORMATION);
    WSContentButton(response, BUTTON_FIRMWARE_UPGRADE);
#endif  // Not FIRMWARE_MINIMAL
    WSContentButton(response, BUTTON_CONSOLE);
    WSContentButton(response, BUTTON_RESTART);
  }
  WSContentStop(response);
  request->send(response);
}

/*-------------------------------------------------------------------------------------------*/

#ifndef FIRMWARE_MINIMAL

void HandleConfiguration(AsyncWebServerRequest *request) {
  if (!HttpCheckPriviledgedAccess(request)) { return; }

  Log::log(LOG_LEVEL_DEBUG, D_LOG_HTTP D_CONFIGURATION);
  requestAuthentication(request);
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), S_CONFIGURATION);
  WSContentSendStyle(response);

  //WSContentButton(response, BUTTON_MODULE);
  WSContentButton(response, BUTTON_WIFI);
  WSContentButton(response, BUTTON_CARDS);

  // XdrvCall(FUNC_WEB_ADD_BUTTON);
  // XsnsCall(FUNC_WEB_ADD_BUTTON);

  WSContentButton(response, BUTTON_LOGGING);
  WSContentButton(response, BUTTON_OTHER);
  //WSContentButton(response, BUTTON_TEMPLATE);

  WSContentSpaceButton(response, BUTTON_RESET_CONFIGURATION);
  WSContentButton(response, BUTTON_BACKUP);
  WSContentButton(response, BUTTON_RESTORE);

  WSContentSpaceButton(response, BUTTON_MAIN);
  WSContentStop(response);
  request->send(response);
}

/*-------------------------------------------------------------------------------------------*/

// void WSContentSendNiceLists(uint32_t option) {
//   char stemp[30];                                             // Template number and Sensor name
//   for (uint32_t i = 0; i < ARRAY_SIZE(kGpioNiceList); i++) {  // GPIO: }2'0'>None (0)}3}2'17'>Button1 (17)}3...
//     if (option && (1 == i)) {
//       response->print(HTTP_MODULE_TEMPLATE_REPLACE_NO_INDEX, AGPIO(GPIO_USER), D_SENSOR_USER);  // }2'255'>User}3
//     }
//     uint32_t ridx = pgm_read_word(kGpioNiceList + i) & 0xFFE0;
//     uint32_t midx = BGPIO(ridx);
//     response->print(HTTP_MODULE_TEMPLATE_REPLACE_NO_INDEX, ridx, Support::getTextIndexed(stemp, sizeof(stemp), midx, kSensorNames));
//   }
//   response->print(PSTR("\";"));

//   response->print(PSTR("hs=["));
//   uint32_t midx;
//   bool first_done = false;
//   for (uint32_t i = 0; i < ARRAY_SIZE(kGpioNiceList); i++) {  // hs=[36,68,100,132,168,200,232,264,292,324,356,388,421,453];
//     midx = pgm_read_word(kGpioNiceList + i);
//     if (midx & 0x001F) {
//       if (first_done) { response->print(PSTR(",")); }
//       response->print(PSTR("%d"), midx);
//       first_done = true;
//     }
//   }
// #ifdef ESP8266
// #ifdef USE_ADC
//   for (uint32_t i = 0; i < ARRAY_SIZE(kAdcNiceList); i++) {   // hs=[36,68,100,132,168,200,232,264,292,324,356,388,421,453];
//     midx = pgm_read_word(kAdcNiceList + i);
//     if (midx & 0x001F) {
//       if (first_done) { response->print(PSTR(",")); }
//       response->print(PSTR("%d"), midx);
//       first_done = true;
//     }
//   }
// #endif  // USE_ADC
// #endif  // ESP8266
//   response->print(PSTR("];"));
// }

#ifdef ESP8266
#ifdef USE_ADC
void WSContentSendAdcNiceList(uint32_t option) {
  char stemp[30];                                             // Template number and Sensor name
  response->print(PSTR("os=\""));
  for (uint32_t i = 0; i < ARRAY_SIZE(kAdcNiceList); i++) {   // GPIO: }2'0'>None}3}2'17'>Analog}3...
    if (option && (1 == i)) {
      response->print(HTTP_MODULE_TEMPLATE_REPLACE_NO_INDEX, AGPIO(GPIO_USER), D_SENSOR_USER);  // }2'15'>User}3
    }
    uint32_t ridx = pgm_read_word(kAdcNiceList + i) & 0xFFE0;
    uint32_t midx = BGPIO(ridx);
    response->print(HTTP_MODULE_TEMPLATE_REPLACE_NO_INDEX, ridx, Support::getTextIndexed(stemp, sizeof(stemp), midx, kSensorNames));
  }
}
#endif  // USE_ADC
#endif  // ESP8266

/*-------------------------------------------------------------------------------------------*/

// void HandleTemplateConfiguration(AsyncWebServerRequest *request) {
//   if (!HttpCheckPriviledgedAccess(request)) { return; }

//   if (request->hasArg("save")) {
//     TemplateSaveSettings();
//     WebRestart(request, 1);
//     return;
//   }

//   char stemp[30];                                           // Template number and Sensor name

//   WebGetArg("t", stemp, sizeof(stemp));                     // 0 - 69 Template number
//   if (strlen(stemp)) {
//     uint32_t module = atoi(stemp);
//     uint32_t module_save = Settings.module;
//     Settings.module = module;
//     myio cmodule;
//     ModuleGpios(&cmodule);
//     gpio_flag flag = ModuleFlag();
//     Settings.module = module_save;

//     WSContentBegin(200, CT_PLAIN);
//     response->print(PSTR("%s}1"), AnyModuleName(module).c_str());  // NAME: Generic
//     for (uint32_t i = 0; i < ARRAY_SIZE(cmodule.io); i++) {        // 17,148,29,149,7,255,255,255,138,255,139,255,255
//       if (!FlashPin(i)) {
//         response->print(PSTR("%s%d"), (i>0)?",":"", cmodule.io[i]);
//       }
//     }
//     response->print(PSTR("}1%d}1%d"), flag, Settings.user_template_base);  // FLAG: 1  BASE: 17
//     WSContentEnd(response);
//     return;
//   }

//   Log::log(LOG_LEVEL_DEBUG, D_LOG_HTTP  D_CONFIGURE_TEMPLATE);
//   requestAuthentication(request);
//   AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
//   response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), HTTP_SCRIPT_MODULE_TEMPLATE);
//   response->print(HTTP_SCRIPT_MODULE_TEMPLATE);

//   response->print(HTTP_SCRIPT_TEMPLATE);

//   //WSContentSendNiceLists(1);

//   response->print(HTTP_SCRIPT_TEMPLATE2);

// #ifdef ESP8266
// #ifdef USE_ADC
//   WSContentSendAdcNiceList(1);
//   response->print(HTTP_SCRIPT_TEMPLATE3);
// #endif  // USE_ADC
// #endif  // ESP8266

//   response->print(HTTP_SCRIPT_TEMPLATE4);
//   for (uint32_t i = 0; i < sizeof(kModuleNiceList); i++) {  // "}2'%d'>%s (%d)}3" - "}2'0'>Sonoff Basic (1)}3"
//     uint32_t midx = pgm_read_byte(kModuleNiceList + i);
//     response->print(HTTP_MODULE_TEMPLATE_REPLACE_INDEX, midx, AnyModuleName(midx).c_str(), midx +1);
//   }
//   response->print(HTTP_SCRIPT_TEMPLATE5);

//   WSContentSendStyle(response);
//   response->print(HTTP_FORM_TEMPLATE);
//   response->print(HTTP_TABLE100);
//   response->print(PSTR("<tr><td><b>" D_TEMPLATE_NAME "</b></td><td style='width:200px'><input id='s1' placeholder='" D_TEMPLATE_NAME "'></td></tr>"
//                        "<tr><td><b>" D_BASE_TYPE "</b></td><td><select id='g99' onchange='st(this.value)'></select></td></tr>"
//                        "</table>"
//                        "<hr/>"));
//   response->print(HTTP_TABLE100);
//   for (uint32_t i = 0; i < MAX_GPIO_PIN; i++) {
//     if (!FlashPin(i)) {
//       response->print(PSTR("<tr><td><b><font color='#%06x'>" D_GPIO "%d</font></b></td><td%s><select id='g%d' onchange='ot(%d,this.value)'></select></td>"),
//         ((9==i)||(10==i)) ? WebColor(COL_TEXT_WARNING) : WebColor(COL_TEXT), i, (0==i) ? " style='width:150px'" : "", i, i);
//       response->print(PSTR("<td style='width:50px'><select id='h%d'></select></td></tr>"), i);
//     }
//   }
//   response->print(PSTR("</table>"));

//   gpio_flag flag = ModuleFlag();
//   if (flag.data) {
//     response->print(HTTP_FORM_TEMPLATE_FLAG);
//   }

//   response->print(HTTP_FORM_END);
//   WSContentSpaceButton(response, BUTTON_CONFIGURATION);
//   WSContentStop(response);
//   request->send(response);
// }

// uint16_t WebGetGpioArg(uint32_t i) {
//   char webindex[5];                                         // WebGetArg name
//   snprintf_P(webindex, sizeof(webindex), PSTR("g%d"), i);
//   char tmp[8];                                              // WebGetArg numbers only
//   WebGetArg(webindex, tmp, sizeof(tmp));                    // GPIO
//   uint32_t gpio = (!strlen(tmp)) ? 0 : atoi(tmp);
//   char webindex2[5];                                        // WebGetArg name
//   snprintf_P(webindex2, sizeof(webindex2), PSTR("h%d"), i);
//   char tmp2[8];                                             // WebGetArg numbers only
//   WebGetArg(webindex2, tmp2, sizeof(tmp2));
//   uint32_t value2 = (!strlen(tmp2)) ? 0 : atoi(tmp2) -1;
//   gpio += value2;
//   return gpio;
// }

// void TemplateSaveSettings(void) {
//   char tmp[TOPSZ];                                          // WebGetArg NAME and GPIO/BASE/FLAG byte value
//   char svalue[300];                                         // Template command string

//   WebGetArg("s1", tmp, sizeof(tmp));                        // NAME
//   snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_TEMPLATE " {\"" D_JSON_NAME "\":\"%s\",\"" D_JSON_GPIO "\":["), tmp);

//   uint32_t j = 0;
//   for (uint32_t i = 0; i < ARRAY_SIZE(Settings.user_template.gp.io); i++) {
//     if (6 == i) { j = 9; }
//     if (8 == i) { j = 12; }
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s%s%d"), svalue, (i>0)?",":"", WebGetGpioArg(j));
//     j++;
//   }

//   uint32_t flag = 0;
//   char webindex[5];                                         // WebGetArg name
//   for (uint32_t i = 0; i < GPIO_FLAG_USED; i++) {
//     snprintf_P(webindex, sizeof(webindex), PSTR("c%d"), i);
//     uint32_t state = request->hasArg(webindex) << i;      // FLAG
//     flag += state;
//   }
//   WebGetArg("g99", tmp, sizeof(tmp));                       // BASE
//   uint32_t base = atoi(tmp) +1;

//   snprintf_P(svalue, sizeof(svalue), PSTR("%s],\"" D_JSON_FLAG "\":%d,\"" D_JSON_BASE "\":%d}"), svalue, flag, base);
//   ExecuteWebCommand(svalue, SRC_WEBGUI);
// }

/*-------------------------------------------------------------------------------------------*/

// void HandleModuleConfiguration(AsyncWebServerRequest *request) {
//   if (!HttpCheckPriviledgedAccess(request)) { return; }

//   if (request->hasArg("save")) {
//     ModuleSaveSettings();
//     WebRestart(1);
//     return;
//   }

//   Log::log(LOG_LEVEL_DEBUG, D_LOG_HTTP  D_CONFIGURE_MODULE);

//   char stemp[30];  // Sensor name
//   uint32_t midx;
//   myio cmodule;
//   ModuleGpios(&cmodule);
//   requestAuthentication(request);
//   AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
//   response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), S_CONFIGURE_MODULE);
//   response->print(HTTP_SCRIPT_MODULE_TEMPLATE);

//   response->print(PSTR("function sl(){os=\""));
//   uint32_t vidx = 0;
//   for (uint32_t i = 0; i <= sizeof(kModuleNiceList); i++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//     if (0 == i) {
//       midx = USER_MODULE;
//       vidx = 0;
//     } else {
//       midx = pgm_read_byte(kModuleNiceList + i -1);
//       vidx = midx +1;
//     }
//     response->print(HTTP_MODULE_TEMPLATE_REPLACE_INDEX, midx, AnyModuleName(midx).c_str(), vidx);
//   }
//   response->print(PSTR("\";sk(%d,99);os=\""), Settings.module);

//   //WSContentSendNiceLists(0);

//   for (uint32_t i = 0; i < ARRAY_SIZE(cmodule.io); i++) {
//     if (ValidGPIO(i, cmodule.io[i])) {
//       response->print(PSTR("sk(%d,%d);"), TasmotaGlobal.my_module.io[i], i);  // g0 - g17
//     }
//   }

// #ifdef ESP8266
// #ifdef USE_ADC
//   WSContentSendAdcNiceList(0);
//   response->print(PSTR("\";sk(%d," STR(ADC0_PIN) ");"), Settings.my_gp.io[(sizeof(myio) / 2) -1]);
// #endif  // USE_ADC
// #endif  // ESP8266

//   response->print(PSTR("}wl(sl);"));

//   WSContentSendStyle(response);
//   response->print(HTTP_FORM_MODULE, AnyModuleName(MODULE).c_str());
//   for (uint32_t i = 0; i < ARRAY_SIZE(cmodule.io); i++) {
//     if (ValidGPIO(i, cmodule.io[i])) {
//       snprintf_P(stemp, 3, PINS_WEMOS +i*2);
//       response->print(PSTR("<tr><td style='width:116px'>%s <b>" D_GPIO "%d</b></td><td style='width:150px'><select id='g%d' onchange='ot(%d,this.value)'></select></td>"),
//         (WEMOS==TasmotaGlobal.module_type)?stemp:"", i, i, i);
//       response->print(PSTR("<td style='width:50px'><select id='h%d'></select></td></tr>"), i);
//     }
//   }
//   response->print(PSTR("</table>"));
//   response->print(HTTP_FORM_END);
//   WSContentSpaceButton(response, BUTTON_CONFIGURATION);
//   WSContentStop(response);
//   request->send(response);
// }

// void ModuleSaveSettings(void) {
//   char tmp[8];         // WebGetArg numbers only

//   WebGetArg("g99", tmp, sizeof(tmp));
//   uint32_t new_module = (!strlen(tmp)) ? MODULE : atoi(tmp);
//   Settings.last_module = Settings.module;
//   Settings.module = new_module;
//   SetModuleType();
//   myio cmodule;
//   ModuleGpios(&cmodule);
//   String gpios = "";
//   for (uint32_t i = 0; i < ARRAY_SIZE(cmodule.io); i++) {
//     if (Settings.last_module != new_module) {
//       Settings.my_gp.io[i] = GPIO_NONE;
//     } else {
//       if (ValidGPIO(i, cmodule.io[i])) {
//         Settings.my_gp.io[i] = WebGetGpioArg(i);
//         gpios += F(", " D_GPIO ); gpios += String(i); gpios += F(" "); gpios += String(Settings.my_gp.io[i]);
//       }
//     }
//   }

//   Log::log(LOG_LEVEL_INFO, PSTR(D_LOG_MODULE "%s " D_CMND_MODULE "%s"), ModuleName().c_str(), gpios.c_str());
// }

/*-------------------------------------------------------------------------------------------*/

const char kUnescapeCode[] = "&><\"\'\\";
const char kEscapeCode[] PROGMEM = "&amp;|&gt;|&lt;|&quot;|&apos;|&#92;";

String HtmlEscape(const String unescaped) {
  char escaped[10];
  size_t ulen = unescaped.length();
  String result = "";
  for (size_t i = 0; i < ulen; i++) {
    char c = unescaped[i];
    char *p = strchr(kUnescapeCode, c);
    if (p != nullptr) {
      result += Support::Support::getTextIndexed(escaped, sizeof(escaped), p - kUnescapeCode, kEscapeCode);
    } else {
      result += c;
    }
  }
  return result;
}

// Indexed by enum wl_enc_type in file wl_definitions.h starting from -1
const char kEncryptionType[] PROGMEM = "|||" D_WPA_PSK "||" D_WPA2_PSK "|" D_WEP "||" D_NONE "|" D_AUTO;

void HandleWifiConfiguration(AsyncWebServerRequest *request) {
  if (!HttpCheckPriviledgedAccess(request, !WifiIsInManagerMode())) { return; }

  Log::log(LOG_LEVEL_DEBUG, D_LOG_HTTP D_CONFIGURE_WIFI);

  if (request->hasArg("save") && HTTP_MANAGER_RESET_ONLY != Web.state) {
    WifiSaveSettings(request);
    WebRestart(request, 2);
    return;
  }
  if (!WifiIsInManagerMode())
    requestAuthentication(request);
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), S_CONFIGURE_WIFI);
  response->print(HTTP_SCRIPT_WIFI);
  WSContentSendStyle(response);

  if (HTTP_MANAGER_RESET_ONLY != Web.state) {
    if (request->hasArg("scan")) {
      Serial.println("Printing the scan");
      int n = WiFi.scanNetworks();
      Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_SCAN_DONE));

      if (0 == n) {
        Log::log(LOG_LEVEL_DEBUG, D_LOG_WIFI D_NO_NETWORKS_FOUND);
        response->print(S_NO_NETWORKS_FOUND);
        response->print(PSTR(". " D_REFRESH_TO_SCAN_AGAIN "."));
      } else {
        //sort networks
        int indices[n];
        for (uint32_t i = 0; i < n; i++) {
          indices[i] = i;
        }

        // RSSI SORT
        for (uint32_t i = 0; i < n; i++) {
          for (uint32_t j = i + 1; j < n; j++) {
            if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
              std::swap(indices[i], indices[j]);
            }
          }
        }

        // remove duplicates ( must be RSSI sorted )
        String cssid;
        for (uint32_t i = 0; i < n; i++) {
          if (-1 == indices[i]) { continue; }
          cssid = WiFi.SSID(indices[i]);
          uint32_t cschn = WiFi.channel(indices[i]);
          for (uint32_t j = i + 1; j < n; j++) {
            if ((cssid == WiFi.SSID(indices[j])) && (cschn == WiFi.channel(indices[j]))) {
              Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_DUPLICATE_ACCESSPOINT " %s"), WiFi.SSID(indices[j]).c_str());
              indices[j] = -1;  // set dup aps to index -1
            }
          }
        }

        //display networks in page
        for (uint32_t i = 0; i < n; i++) {
          if (-1 == indices[i]) { continue; }  // skip dups
          int32_t rssi = WiFi.RSSI(indices[i]);
          Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_WIFI D_SSID " %s, " D_BSSID " %s, " D_CHANNEL " %d, " D_RSSI " %d"),
            WiFi.SSID(indices[i]).c_str(), WiFi.BSSIDstr(indices[i]).c_str(), WiFi.channel(indices[i]), rssi);
          int quality = WifiManager::getRssiAsQuality(rssi);
          int auth = WiFi.encryptionType(indices[i]);
          char encryption[20];
          response->printf(PSTR("<div><a href='#p' onclick='c(this)'>%s</a>&nbsp;(%d)&nbsp<span class='q'>%s %d%% (%d dBm)</span></div>"),
            HtmlEscape(WiFi.SSID(indices[i])).c_str(),
            WiFi.channel(indices[i]),
            Support::getTextIndexed(encryption, sizeof(encryption), auth +1, kEncryptionType),
            quality, rssi
          );
          delay(0);

        }
        response->print(PSTR("<br>"));
      }
    } else {
      response->print(PSTR("<div><a href='/wi?scan='>" D_SCAN_FOR_WIFI_NETWORKS "</a></div><br>"));
    }

    // As WIFI_HOSTNAME may contain %s-%04d it cannot be part of HTTP_FORM_WIFI where it will exception
    response->printf(HTTP_FORM_WIFI, settings.getStaWifiSsid1(), settings.getStaWifiSsid2(), WIFI_HOSTNAME, WIFI_HOSTNAME, settings.getHostnameRaw(), settings.getWifiCorsHeader());
    response->print(HTTP_FORM_END);
  }

  if (WifiIsInManagerMode()) {
#ifndef FIRMWARE_MINIMAL
    WSContentSpaceButton(response, BUTTON_RESTORE);
    WSContentButton(response, BUTTON_RESET_CONFIGURATION);
#endif  // FIRMWARE_MINIMAL
    WSContentSpaceButton(response, BUTTON_RESTART);
  } else {
    WSContentSpaceButton(response, BUTTON_CONFIGURATION);
  }
  WSContentStop(response);
  request->send(response);
}

void WifiSaveSettings(AsyncWebServerRequest *request) {
  settings.setStaWifiCredentials(request->arg("s1"), request->arg("p1"),
                request->arg("s2"), request->arg("p2"));
  settings.setWifiCorsHeader(request->arg("c").c_str());
  Log::log(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CMND_HOSTNAME " %s, " D_CMND_SSID "1 %s, " D_CMND_SSID "2 %s, " D_CMND_CORS " %s"),
    settings.getHostname(), settings.getStaWifiSsid1(), settings.getStaWifiSsid2(), settings.getWifiCorsHeader());
  settings.save();
}


void HandleCardsConfiguration (AsyncWebServerRequest *request) {
  if (!HttpCheckPriviledgedAccess(request)) { return; }

  Log::log(LOG_LEVEL_DEBUG, D_LOG_HTTP  D_CONFIGURE_LOGGING);

  if (request->hasArg("save")) {
    //CardsSaveSettings(request);
    WebRestart(request, 0);
    return;
  }
  requestAuthentication(request);
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), S_CONFIGURE_CARDS);
  WSContentSendStyle(response);
  response->print(HTTP_EDITOR);
  response->print("<p></p><button id='sv' onclick='svC()'>" D_SAVE "</button>");
  WSContentButton(response, BUTTON_CONFIGURATION);
  WSContentStop(response);
  request->send(response);
}

/*-------------------------------------------------------------------------------------------*/

void HandleLoggingConfiguration(AsyncWebServerRequest *request)
{
  if (!HttpCheckPriviledgedAccess(request)) { return; }

  Log::log(LOG_LEVEL_DEBUG, D_LOG_HTTP D_CONFIGURE_LOGGING);

  if (request->hasArg("save")) {
    LoggingSaveSettings(request);
    HandleConfiguration(request);
    return;
  }
  requestAuthentication(request);
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), S_CONFIGURE_LOGGING);
  WSContentSendStyle(response);
  response->print(HTTP_FORM_LOG1);
  char stemp1[45];
  char stemp2[32];
  uint8_t dlevel[4] = { LOG_LEVEL_INFO, LOG_LEVEL_INFO, LOG_LEVEL_NONE, LOG_LEVEL_NONE };
  for (uint32_t idx = 0; idx < 3; idx++) {
    uint32_t llevel = (0==idx)?settings.getSerialLogLevel():(1==idx)?settings.getWebLogLevel():settings.getSysLogLevel();
    response->printf(PSTR("<p><b>%s</b> (%s)<br><select id='l%d'>"),
      Support::getTextIndexed(stemp1, sizeof(stemp1), idx, kLoggingOptions),
      Support::getTextIndexed(stemp2, sizeof(stemp2), dlevel[idx], kLoggingLevels),
      idx);
    for (uint32_t i = LOG_LEVEL_NONE; i <= LOG_LEVEL_DEBUG_MORE; i++) {
      response->printf(PSTR("<option%s value='%d'>%d %s</option>"),
        (i == llevel) ? " selected" : "", i, i,
        Support::getTextIndexed(stemp1, sizeof(stemp1), i, kLoggingLevels));
    }
    response->print(PSTR("</select></p>"));
  }
  response->printf(HTTP_FORM_LOG2, settings.getSysLogHostname(), settings.getSysLogHostPort(), settings.getSysLogTelePeriod());
  response->print(HTTP_FORM_END);
  WSContentButton(response, BUTTON_CONFIGURATION);
  WSContentStop(response);
  request->send(response);
}

void LoggingSaveSettings(AsyncWebServerRequest *request) {
  const char * tmp = request->arg("l0").c_str();
  settings.setSerialLogLevel((!strlen(tmp)) ? SERIAL_LOG_LEVEL : atoi(tmp));
  tmp = request->arg("l1").c_str();
  settings.setWebLogLevel((!strlen(tmp)) ? WEB_LOG_LEVEL : atoi(tmp));
  tmp = request->arg("l2").c_str();
  settings.setSysLogLevel((!strlen(tmp)) ? SYS_LOG_LEVEL : atoi(tmp));
  tmp = request->arg("lh").c_str();
  settings.setSysLogHostname((!strlen(tmp)) ? SYS_LOG_HOST : tmp);
  tmp = request->arg("lp").c_str();
  settings.setSysLogHostPort((!strlen(tmp)) ? SYS_LOG_PORT : atoi(tmp));
  tmp = request->arg("lt").c_str();
  settings.setSysLogTelePeriod((!strlen(tmp)) ? TELE_PERIOD : atoi(tmp));
  if ((settings.getSysLogTelePeriod() > 0) && (settings.getSysLogTelePeriod() < 10)) {
    settings.setSysLogTelePeriod(10);   // Do not allow periods < 10 seconds
  }
  Log::log(LOG_LEVEL_INFO, PSTR(D_LOG_LOG D_CMND_SERIALLOG " %d, " D_CMND_WEBLOG " %d, " D_CMND_SYSLOG " %d, " D_CMND_LOGHOST " %s, " D_CMND_LOGPORT " %d, " D_CMND_TELEPERIOD " %d"),
    settings.getSerialLogLevel(), settings.getWebLogLevel(), settings.getSysLogTelePeriod(), settings.getSysLogHostname(), settings.getSysLogHostPort(), settings.getSysLogTelePeriod());
}

/*-------------------------------------------------------------------------------------------*/

void HandleOtherConfiguration(AsyncWebServerRequest *request) {
  if (!HttpCheckPriviledgedAccess(request)) { return; }

  Log::log(LOG_LEVEL_DEBUG, D_LOG_HTTP D_CONFIGURE_OTHER);

  if (request->hasArg("save")) {
    OtherSaveSettings(request);
    WebRestart(request, 1);
    return;
  }
  requestAuthentication(request);
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), S_CONFIGURE_OTHER);
  WSContentSendStyle(response);

  //char stemp[strlen(TasmotaGlobal.mqtt_data) +1];
  // strlcpy(stemp, TasmotaGlobal.mqtt_data, sizeof(stemp));  // Get JSON template
  response->printf(HTTP_FORM_OTHER, 
    settings.getWebUsername(),
    "",
    settings.getDeviceName());

  response->print(HTTP_FORM_END);
  WSContentSpaceButton(response, BUTTON_CONFIGURATION);
  WSContentStop(response);
  request->send(response);
}

void OtherSaveSettings(AsyncWebServerRequest *request) {
  const char * tmp = request->arg("dn").c_str();
  settings.setDeviceName(tmp);
  Serial.println(String(tmp));
  tmp = request->arg("wp").c_str();
  settings.setWebPassword(tmp);
  tmp = request->arg("wu").c_str();
  settings.setWebUsername(tmp);
}

/*-------------------------------------------------------------------------------------------*/

void HandleBackupConfiguration(AsyncWebServerRequest *request) {
  if (!HttpCheckPriviledgedAccess(request)) { return; }

  Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_BACKUP_CONFIGURATION));
  AsyncResponseStream *response = WSContentBegin(request, 200, "application/octet-stream");
  response->setContentLength(sizeof(Settings));

  char attachment[150];

  char hostname[MAX_HOSTNAME_SIZE+1];
  snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=Config_%s_%s.dmp"), Support::noAlNumToUnderscore(hostname, settings.getHostname()), settings.getVersionString().c_str());

  response->addHeader(F("Content-Disposition"), attachment);
  uint32_t crc32 = settings.getCrc32();
  settings.updateCrc32();
  char * settingsBuffer = (char *)malloc(sizeof(Settings));
  if (settingsBuffer != nullptr) {
    memcpy(settingsBuffer, &settings, sizeof(Settings));
    if (Web.config_xor_on_set) {
      for (uint32_t i = 2; i < sizeof(Settings); i++) {
        settingsBuffer[i] ^= (Web.config_xor_on_set +i);
      }
    }
    response->write((const char*)settingsBuffer, sizeof(Settings));
    free(settingsBuffer);
  } else {
    memset(settingsBuffer, 0, sizeof(Settings));
    response->write((const char*)settingsBuffer, sizeof(Settings));
  }
  settings.setCrc32(crc32);   // Restore crc in case savedata = 0 to make sure settings will be noted as changed
}

/*-------------------------------------------------------------------------------------------*/

void HandleResetConfiguration(AsyncWebServerRequest *request) {
  if (!HttpCheckPriviledgedAccess(request, !WifiIsInManagerMode())) { return; }

  Log::log(LOG_LEVEL_DEBUG, D_LOG_HTTP D_RESET_CONFIGURATION);
  if (!WifiIsInManagerMode())
    requestAuthentication(request);
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), S_RESET_CONFIGURATION);
  WSContentSendStyle(response);
  response->print(PSTR("<div style='text-align:center;'>" D_CONFIGURATION_RESET "</div>"));
  response->print(HTTP_MSG_RSTRT);
  WSContentSpaceButton(response, BUTTON_MAIN);
  WSContentStop(response);
  request->send(response);
  settings.setDefault();
  globalParams.setRestartFlag(2);
}

void HandleRestoreConfiguration(AsyncWebServerRequest *request) {
  if (!HttpCheckPriviledgedAccess(request)) { return; }

  Log::log(LOG_LEVEL_DEBUG, D_LOG_HTTP D_RESTORE_CONFIGURATION);
  requestAuthentication(request);
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), S_RESTORE_CONFIGURATION);
  WSContentSendStyle(response);
  response->print(HTTP_FORM_RST);
  response->printf(HTTP_FORM_RST_UPG, D_RESTORE);
  if (WifiIsInManagerMode()) {
    WSContentSpaceButton(response, BUTTON_MAIN);
  } else {
    WSContentSpaceButton(response, BUTTON_CONFIGURATION);
  }
  WSContentStop(response);
  request->send(response);

  Web.upload_error = 0;
  Web.upload_file_type = UPL_SETTINGS;
}

/*-------------------------------------------------------------------------------------------*/

void HandleInformation(AsyncWebServerRequest *request) {
  if (!HttpCheckPriviledgedAccess(request)) { return; }

  Log::log(LOG_LEVEL_DEBUG, D_LOG_HTTP D_INFORMATION);

  int freeMem = ESP.getMaxAllocHeap();
  requestAuthentication(request);
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), S_INFORMATION);
  // Save 1k of code space replacing table html with javascript replace codes
  // }1 = </td></tr><tr><th>
  // }2 = </th><td>
  response->print(HTTP_SCRIPT_INFO_BEGIN);
  response->print(PSTR("<table style='width:100%%'><tr><th>"));
  response->printf(PSTR(D_PROGRAM_VERSION "}2%s%s"), settings.getVersionString().c_str(), PROJECT);
  response->printf(PSTR("}1" D_BUILD_DATE_AND_TIME "}2%s"), Support::getBuildDateAndTime().c_str());
  //response->printf(PSTR("}1" D_CORE_AND_SDK_VERSION "}2" ARDUINO_CORE_RELEASE "/%s"), ESP.getSdkVersion());
  response->printf(PSTR("}1" D_UPTIME "}2%d"), globalParams.getUptime());
#ifdef ESP8266
  response->print(PSTR("}1" D_FLASH_WRITE_COUNT "}2%d at 0x%X"), Settings.save_flag, GetSettingsAddress());
#else
  response->printf(PSTR("}1" D_FLASH_WRITE_COUNT "}2%d"), settings.getSaveCount());
#endif
  response->printf(PSTR("}1" D_BOOT_COUNT "}2%d"), settings.getBootCount());
  //response->printf(PSTR("}1" D_RESTART_REASON "}2%s"), ESP.getResetReason().c_str());
  response->print(PSTR("}1}2&nbsp;"));  // Empty line
  int32_t rssi = WiFi.RSSI();
  response->printf(PSTR("}1" D_AP "%d " D_SSID " (" D_RSSI ")}2%s (%d%%, %d dBm)"), settings.getActiveSta() +1, HtmlEscape(settings.getStaWifiActiveSsid()).c_str(), WifiManager::getRssiAsQuality(rssi), rssi);
  response->printf(PSTR("}1" D_HOSTNAME "}2%s"), settings.getHostname());
  if (static_cast<uint32_t>(WiFi.localIP()) != 0) {
    response->printf(PSTR("}1" D_MAC_ADDRESS "}2%s"), WiFi.macAddress().c_str());
    response->printf(PSTR("}1" D_IP_ADDRESS " (wifi)}2%s"), WiFi.localIP().toString().c_str());
    response->printf(PSTR("}1<hr/>}2<hr/>"));
  }
  if (globalParams.isWifiUp()) {
    response->printf(PSTR("}1" D_GATEWAY "}2%s"), IPAddress(settings.getStaGatewayIpAddress()).toString().c_str());
    response->printf(PSTR("}1" D_SUBNET_MASK "}2%s"), IPAddress(settings.getStaSubnetIpAddress()).toString().c_str());
    response->printf(PSTR("}1" D_DNS_SERVER "}2%s"), IPAddress(settings.getStaDnsIpAddress()).toString().c_str());
  }
  if ((WiFi.getMode() >= WIFI_AP) && (static_cast<uint32_t>(WiFi.softAPIP()) != 0)) {
    response->print(PSTR("}1<hr/>}2<hr/>"));
    response->printf(PSTR("}1" D_MAC_ADDRESS "}2%s"), WiFi.softAPmacAddress().c_str());
    response->printf(PSTR("}1" D_IP_ADDRESS " (AP)}2%s"), WiFi.softAPIP().toString().c_str());
    response->printf(PSTR("}1" D_GATEWAY "}2%s"), WiFi.softAPIP().toString().c_str());
  }
  response->print(PSTR("}1}2&nbsp;"));  // Empty line

// #ifdef USE_DISCOVERY
//   response->printf(PSTR("}1" D_MDNS_DISCOVERY "}2%s"), (Settings.flag3.mdns_enabled) ? D_ENABLED : D_DISABLED);  // SetOption55 - Control mDNS service
//   if (Settings.flag3.mdns_enabled) {  // SetOption55 - Control mDNS service
// #ifdef webServer_ADVERTISE
//     response->print(PSTR("}1" D_MDNS_ADVERTISE "}2" D_WEB_SERVER));
// #else
//     response->print(PSTR("}1" D_MDNS_ADVERTISE "}2" D_DISABLED));
// #endif // webServer_ADVERTISE
//   }
// #else
//   response->print(PSTR("}1" D_MDNS_DISCOVERY "}2" D_DISABLED));
// #endif // USE_DISCOVERY
  response->printf(PSTR("}1" D_ESP_CHIP_ID "}2%d"), ((uint32_t)ESP.getEfuseMac()));
  //response->printf(PSTR("}1" D_FLASH_CHIP_SIZE "}2%dkB"), ESP.getFlashChipRealSize() / 1024);
  response->printf(PSTR("}1" D_PROGRAM_FLASH_SIZE "}2%dkB"), ESP.getFlashChipSize() / 1024);
  response->printf(PSTR("}1" D_PROGRAM_SIZE "}2%dkB"), Support::getSketchSize() / 1024);
  response->printf(PSTR("}1" D_FREE_PROGRAM_SPACE "}2%dkB"), ESP.getFreeSketchSpace() / 1024);
  response->printf(PSTR("}1" D_FREE_MEMORY "}2%dkB"), freeMem / 1024);
#ifdef ESP32
  if (psramFound()) {
    response->printf(PSTR("}1" D_PSR_MAX_MEMORY "}2%dkB"), ESP.getPsramSize() / 1024);
    response->printf(PSTR("}1" D_PSR_FREE_MEMORY "}2%dkB"), ESP.getFreePsram() / 1024);
  }
#endif
  response->print(PSTR("</td></tr></table>"));

  response->print(HTTP_SCRIPT_INFO_END);
  WSContentSendStyle(response);
  // response->print(PSTR("<fieldset><legend><b>&nbsp;Information&nbsp;</b></legend>"));
  response->print(PSTR("<style>td{padding:0px 5px;}</style>"
                       "<div id='i' name='i'></div>"));
  //   response->print(PSTR("</fieldset>"));
  WSContentSpaceButton(response, BUTTON_MAIN);
  WSContentStop(response);
  request->send(response);
}
#endif  // Not FIRMWARE_MINIMAL

/*-------------------------------------------------------------------------------------------*/

void HandleUpgradeFirmware(AsyncWebServerRequest *request)
{
  if (!HttpCheckPriviledgedAccess(request)) { return; }

  Log::log(LOG_LEVEL_DEBUG, D_LOG_HTTP D_FIRMWARE_UPGRADE);
  requestAuthentication(request);
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), PSTR(S_FIRMWARE_UPGRADE));
  WSContentSendStyle(response);
  response->printf(HTTP_FORM_UPG, settings.getOtaUrl());
  response->printf(HTTP_FORM_RST_UPG, D_UPGRADE);
  WSContentSpaceButton(response, BUTTON_MAIN);
  WSContentStop(response);
  request->send(response);

  Web.upload_error = 0;
  Web.upload_file_type = UPL_TASMOTA;
}

void HandleUpgradeFirmwareStart(AsyncWebServerRequest *request)
{
  if (!HttpCheckPriviledgedAccess(request)) { return; }

  //char command[TOPSZ + 10];  // OtaUrl

  Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPGRADE_STARTED));
  wifiManager.getConfigCounter();

  //char otaurl[TOPSZ];
  //WebGetArg("o", otaurl, sizeof(otaurl));
  // if (strlen(otaurl)) {
  //   snprintf_P(command, sizeof(command), PSTR(D_CMND_OTAURL " %s"), otaurl);
  //   ExecuteWebCommand(command, SRC_WEBGUI);
  // }
  requestAuthentication(request);
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), S_INFORMATION);
  response->printf(HTTP_SCRIPT_RELOAD_TIME, HTTP_OTA_RESTART_RECONNECT_TIME);
  WSContentSendStyle(response);
  response->print(PSTR("<div style='text-align:center;'><b>" D_UPGRADE_STARTED " ...</b></div>"));
  response->print(HTTP_MSG_RSTRT);
  WSContentSpaceButton(response, BUTTON_MAIN);
  WSContentStop(response);
  request->send(response);

  //snprintf_P(command, sizeof(command), PSTR(D_CMND_UPGRADE " 1"));
  //ExecuteWebCommand(command, SRC_WEBGUI);
}


static uint8_t * upload_buffer = nullptr;
void HandleUploadDone(AsyncWebServerRequest *request) {
  if (!HttpCheckPriviledgedAccess(request)) { return; }
  Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_UPLOAD_DONE));

  char error[100];
  wifiManager.getConfigCounter();
  globalParams.setRestartFlag(0);
  requestAuthentication(request);
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), S_INFORMATION);
  if (!Web.upload_error) {
    uint32_t javascript_settimeout = HTTP_OTA_RESTART_RECONNECT_TIME;
    response->printf(HTTP_SCRIPT_RELOAD_TIME, javascript_settimeout);  // Refesh main web ui after OTA upgrade
  }
  WSContentSendStyle(response);
  WebTheme * theme = settings.getWebTheme();
  response->print(PSTR("<div style='text-align:center;'><b>" D_UPLOAD " <font color='#"));
  if (Web.upload_error) {
    response->printf(PSTR("%06x'>" D_FAILED "</font></b><br><br>"), theme->colorTextWarning.color);
    if ((Web.upload_error < 10) || (14 == Web.upload_error)) {
      if (14 == Web.upload_error) { Web.upload_error = 10; }
      Support::getTextIndexed(error, sizeof(error), Web.upload_error -1, kUploadErrors);
    } else {
      snprintf_P(error, sizeof(error), PSTR(D_UPLOAD_ERROR_CODE " %d"), Web.upload_error);
    }
    response->print(error);
    Log::log(LOG_LEVEL_DEBUG, PSTR("UPL: %s"), error);
  } else {
    response->printf(PSTR("%06x'>" D_SUCCESSFUL "</font></b><br>"), theme->colorTextSuccess.color);
    globalParams.setRestartFlag(2);  // Always restart to re-enable disabled features during update
    if (globalParams.getRestartFlag()) {
      response->print(HTTP_MSG_RSTRT);
      //ShowWebSource(SRC_WEBGUI);
    }
  }
  Support::bufferFree(upload_buffer);
  response->print(PSTR("</div><br>"));
  WSContentSpaceButton(response, BUTTON_MAIN);
  WSContentStop(response);
  request->send(response);
}
uint32_t totalSize = 0;
void HandleUploadLoop(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
  // Based on ESP8266HTTPUpdateServer.cpp uses ESP8266webServer Parsing.cpp and Cores Updater.cpp (Update)
  bool _serialoutput = (LOG_LEVEL_DEBUG <= settings.getSerialLogLevel());

  if (HTTP_USER == Web.state) { return; }
  if (Web.upload_error) {
    if (UPL_TASMOTA == Web.upload_file_type) { Update.end(); }
    return;
  }
  // ***** Step1: Start upload file
  if (!index) {
    totalSize = 0;
    //globalParams.setRestartFlag(60);
    // if (0 == upload.filename.c_str()[0]) {
    //   Web.upload_error = 1;  // No file selected
    //   return;
    // }
    settings.save();  // Free flash for upload
    // Log::log(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD D_FILE " %s ..."), upload.filename.c_str());
    if (UPL_SETTINGS == Web.upload_file_type) {
      // Support::bufferAlloc(upload_buffer);
      // if (!upload_buffer) {
      //   Web.upload_error = 2;  // Not enough space
      //   return;
      // }
    } else {
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (!Update.begin(maxSketchSpace)) {         //start with max available size
        Web.upload_error = 2;  // Not enough space
        Update.printError(Serial);
        return;
      }
    }

    Web.upload_progress_dot_count = 0;
  }
  if (!Web.upload_error && len) { // ***** Step2: Write upload file
    totalSize += len;
    // if (0 == upload.totalSize) {
    //   if (UPL_SETTINGS == Web.upload_file_type) {
    //     // Web.config_block_count = 0;
    //   } else {
    //     {
    //       if ((upload.buf[0] != 0xE9) && (upload.buf[0] != 0x1F)) {  // 0x1F is gzipped 0xE9
    //         Web.upload_error = 3;  // Magic byte is not 0xE9
    //         return;
    //       }
    //       if (0xE9 == upload.buf[0]) {
    //         uint32_t bin_flash_size = ESP.magicFlashChipSize((upload.buf[3] & 0xf0) >> 4);
    //         if (bin_flash_size > (1 << ((spi_flash_get_id() >> 16) & 0xFF))) {
    //           Web.upload_error = 4;  // Program flash size is larger than real flash size
    //           return;
    //         }
    //       }
    //     }
    //     Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPLOAD "File type %d"), Web.upload_file_type);
    //   }
    // }
    if (UPL_SETTINGS == Web.upload_file_type) {
      // if (!Web.upload_error) {
      //   if (upload.currentSize > (sizeof(Settings) - (Web.config_block_count * HTTP_UPLOAD_BUFLEN))) {
      //     Web.upload_error = 9;  // File too large
      //     return;
      //   }
      //   memcpy(upload_buffer + (Web.config_block_count * HTTP_UPLOAD_BUFLEN), upload.buf, upload.currentSize);
      //   Web.config_block_count++;
      // }
    } else {  // firmware
      if (!Web.upload_error && (Update.write(data, len) != len)) {
        Update.printError(Serial);
        Web.upload_error = 5;  // Upload buffer miscompare
        return;
      }
      if (_serialoutput) {
        Serial.printf(".");
        Web.upload_progress_dot_count++;
        if (!(Web.upload_progress_dot_count % 80)) { Serial.println(); }
      }
    }
  }

  // ***** Step3: Finish upload file
  if(!Web.upload_error && final) {
    if (_serialoutput && (Web.upload_progress_dot_count % 80)) {
      Serial.println();
    }
    if (UPL_SETTINGS == Web.upload_file_type) {
      // if (Web.config_xor_on_set) {
      //   for (uint32_t i = 2; i < sizeof(Settings); i++) {
      //     upload_buffer[i] ^= (Web.config_xor_on_set +i);
      //   }
      // }
      // bool valid_settings = false;
      // unsigned long buffer_version = upload_buffer[11] << 24 | upload_buffer[10] << 16 | upload_buffer[9] << 8 | upload_buffer[8];
      // if (buffer_version > 0x06000000) {
      //   uint32_t buffer_size = upload_buffer[3] << 8 | upload_buffer[2];
      //   if (buffer_version > 0x0606000A) {
      //     uint32_t buffer_crc32 = upload_buffer[4095] << 24 | upload_buffer[4094] << 16 | upload_buffer[4093] << 8 | upload_buffer[4092];
      //     valid_settings = (GetCfgCrc32(upload_buffer, buffer_size -4) == buffer_crc32);
      //   } else {
      //     uint16_t buffer_crc16 = upload_buffer[15] << 8 | upload_buffer[14];
      //     valid_settings = (GetCfgCrc16(upload_buffer, buffer_size) == buffer_crc16);
      //   }
      // } else {
      //   valid_settings = (upload_buffer[0] == CONFIG_FILE_SIGN);
      // }

      // if (valid_settings) {
      //   valid_settings = (1 == upload_buffer[0xF36]);  // Settings.config_version
      // }

      // if (valid_settings) {
      //   // SettingsDefaultSet2();
      //   memcpy((char*)&settings +16, upload_buffer +16, sizeof(Settings) -16);
      //   Settings.version = buffer_version;  // Restore version and auto upgrade after restart
      //   if (upload_buffer != nullptr) {
      //     Support::bufferFree(upload_buffer);
      //     upload_buffer = nullptr;
      //   }
      // } else {
      //   Web.upload_error = 8;  // File invalid
      //   return;
      // }
    } else {
      if (!Update.end(true)) { // true to set the size to the current progress
        if (_serialoutput) { Update.printError(Serial); }
        Web.upload_error = 6;  // Upload failed. Enable logging 3
        return;
      }
      // if (!VersionCompatible()) {
      //   Web.upload_error = 14;  // Not compatible
      //   return;
      // }
    }
    if (!Web.upload_error) {
      Log::log(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD D_SUCCESSFUL " %u bytes"), totalSize);
    }
  // } else if (UPLOAD_FILE_ABORTED == upload.status) {
  //   globalParams.setRestartFlag(0);
  //   Web.upload_error = 7;  // Upload aborted
  //   if (UPL_TASMOTA == Web.upload_file_type) { Update.end(); }
  }
  delay(0);
}

/*-------------------------------------------------------------------------------------------*/

void HandlePreflightRequest(AsyncWebServerRequest *request) {
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  HttpHeaderCors(response);
  response->addHeader(F("Access-Control-Allow-Methods"), F("GET, POST"));
  response->addHeader(F("Access-Control-Allow-Headers"), F("authorization"));
  request->send(response);
}

/*-------------------------------------------------------------------------------------------*/

void HandleHttpCommand(AsyncWebServerRequest *request) {
  if (!HttpCheckPriviledgedAccess(request, false)) { return; }

  Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_COMMAND));

  if (strlen(settings.getWebPassword())) {
    const char * tmp1 = request->arg("user").c_str();
    const char * tmp2 = request->arg("password").c_str();
    if (!(!strcmp(tmp1, settings.getWebUsername()) && !strcmp(tmp2, settings.getWebPassword()))) {
      AsyncResponseStream *response = WSContentBegin(request, 401, "application/json");
      response->print(PSTR("{\"" D_RSLT_WARNING "\":\"" D_NEED_USER_AND_PASSWORD "\"}"));
      request->send(response);
      return;
    }
  }
  AsyncResponseStream *response = WSContentBegin(request, 200, "application/json");
  // uint32_t curridx = TasmotaGlobal.web_log_index;
  // String svalue = request->arg("cmnd");
  // if (svalue.length() && (svalue.length() < MQTT_MAX_PACKET_SIZE)) {
  //   ExecuteWebCommand((char*)svalue.c_str(), SRC_WEBCOMMAND);
  //   if (TasmotaGlobal.web_log_index != curridx) {
  //     uint32_t counter = curridx;
  //     response->print(PSTR("{"));
  //     bool cflg = false;
  //     do {
  //       char* tmp;
  //       size_t len;
  //       GetLog(counter, &tmp, &len);
  //       if (len) {
  //         // [14:49:36 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}] > [{"POWER":"OFF"}]
  //         char* JSON = (char*)memchr(tmp, '{', len);
  //         if (JSON) { // Is it a JSON message (and not only [15:26:08 MQT: stat/wemos5/POWER = O])
  //           size_t JSONlen = len - (JSON - tmp);
  //           if (JSONlen > sizeof(TasmotaGlobal.mqtt_data)) { JSONlen = sizeof(TasmotaGlobal.mqtt_data); }
  //           char stemp[JSONlen];
  //           strlcpy(stemp, JSON +1, JSONlen -2);
  //           response->printf(PSTR("%s%s"), (cflg) ? "," : "", stemp);
  //           cflg = true;
  //         }
  //       }
  //       counter++;
  //       counter &= 0xFF;
  //       if (!counter) counter++;  // Skip 0 as it is not allowed
  //     } while (counter != TasmotaGlobal.web_log_index);
  //     response->print(PSTR("}"));
  //   } else {
  //     response->print(PSTR("{\"" D_RSLT_WARNING "\":\"" D_ENABLE_WEBLOG_FOR_RESPONSE "\"}"));
  //   }
  // } else {
  //   response->print(PSTR("{\"" D_RSLT_WARNING "\":\"" D_ENTER_COMMAND " cmnd=\"}"));
  // }
  request->send(response);
}

/*-------------------------------------------------------------------------------------------*/

void HandleConsole(AsyncWebServerRequest *request)
{
  if (!HttpCheckPriviledgedAccess(request)) { return; }

  if (request->hasArg("c2")) {      // Console refresh requested
    HandleConsoleRefresh(request);
    return;
  }

  Log::log(LOG_LEVEL_DEBUG, D_LOG_HTTP D_CONSOLE);
  requestAuthentication(request);
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/html");
  response->printf(HTTP_HEADER1, S_HTML_LANGUAGE, settings.getDeviceName(), (char *)S_CONSOLE);
  response->printf(HTTP_SCRIPT_CONSOL, settings.getWebRefreshInterval());
  WSContentSendStyle(response);
  response->print(HTTP_FORM_CMND);
  WSContentSpaceButton(response, BUTTON_MAIN);
  WSContentStop(response);
  request->send(response);
}

void HandleConsoleRefresh(AsyncWebServerRequest *request) {
  bool cflg = true;
  uint32_t counter = 0;                // Initial start, should never be 0 again

  String svalue = request->arg("c1");
  // if (svalue.length() && (svalue.length() < MQTT_MAX_PACKET_SIZE)) {
  //   Log::log(LOG_LEVEL_INFO, PSTR(D_LOG_COMMAND "%s"), svalue.c_str());
  //   //ExecuteWebCommand((char*)svalue.c_str(), SRC_WEBCONSOLE);
  // }

  const char * stmp = request->arg("c2").c_str();
  if (strlen(stmp)) { counter = atoi(stmp); }
  AsyncResponseStream *response = WSContentBegin(request, 200, "text/plain");
  response->printf("%d}1%d}1", globalParams.getWebLogIndex(), Web.reset_web_log_flag);
  if (!Web.reset_web_log_flag) {
    counter = 0;
    Web.reset_web_log_flag = true;
  }
  if (counter != globalParams.getWebLogIndex()) {
    if (!counter) {
      counter = globalParams.getWebLogIndex();
      cflg = false;
    }
    do {
      char* tmp;
      size_t len;
      Log::getWebLog(counter, &tmp, &len);
      if (len) {
        char stemp[len +1];
        strlcpy(stemp, tmp, len);
        response->printf(PSTR("%s%s"), (cflg) ? "\n" : "", stemp);
        cflg = true;
      }
      counter++;
      counter &= 0xFF;
      if (!counter) { counter++; }  // Skip log index 0 as it is not allowed
    } while (counter != globalParams.getWebLogIndex());
  }
  response->print("}11");
  request->send(response);
}

/********************************************************************************************/

void HandleNotFound(AsyncWebServerRequest *request) {
  Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP "Not found (%s)"), request->url().c_str());

  if (CaptivePortal(request)) { return; }  // If captive portal redirect instead of displaying the error page.
  {
    AsyncResponseStream *response = WSContentBegin(request, 404, "text/plain");
    response->printf(PSTR(D_FILE_NOT_FOUND "\n\nURI: %s\nMethod: %s\nArguments: %d\n"), request->url().c_str(), (request->method() == HTTP_GET) ? "GET" : "POST", request->args());
    for (uint32_t i = 0; i < request->args(); i++) {
      response->printf(PSTR(" %s: %s\n"), request->argName(i).c_str(), request->arg(i).c_str());
    }
    request->send(response);
  }
}

/* Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
bool CaptivePortal(AsyncWebServerRequest *request) {
  // Possible hostHeader: connectivitycheck.gstatic.com or 192.168.4.1
  if ((WifiIsInManagerMode()) && 0) {//!ValidIpAddress(request->hostHeader().c_str())) {
    Log::log(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_REDIRECTED));
    AsyncResponseStream *response = WSContentBegin(request, 302, "text/plain");
    response->addHeader(F("Location"), String("http://") + request->client()->localIP().toString());
    request->send(response);
    //webServer->client().stop();  // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

/*********************************************************************************************/

// String UrlEncode(const String& text) {
//   const char hex[] = "0123456789ABCDEF";

// 	String encoded = "";
// 	int len = text.length();
// 	int i = 0;
// 	while (i < len)	{
// 		char decodedChar = text.charAt(i++);

// /*
//     if (('a' <= decodedChar && decodedChar <= 'z') ||
//         ('A' <= decodedChar && decodedChar <= 'Z') ||
//         ('0' <= decodedChar && decodedChar <= '9') ||
//         ('=' == decodedChar)) {
//       encoded += decodedChar;
// 		} else {
//       encoded += '%';
// 			encoded += hex[decodedChar >> 4];
// 			encoded += hex[decodedChar & 0xF];
//     }
// */
//     if ((' ' == decodedChar) || ('+' == decodedChar)) {
//       encoded += '%';
// 			encoded += hex[decodedChar >> 4];
// 			encoded += hex[decodedChar & 0xF];
//     } else {
//       encoded += decodedChar;
//     }

// 	}
// 	return encoded;
// }

// int WebSend(char *buffer) {
//   // [tasmota] POWER1 ON                                               --> Sends http://tasmota/cm?cmnd=POWER1 ON
//   // [192.168.178.86:80,admin:joker] POWER1 ON                        --> Sends http://hostname:80/cm?user=admin&password=joker&cmnd=POWER1 ON
//   // [tasmota] /any/link/starting/with/a/slash.php?log=123             --> Sends http://tasmota/any/link/starting/with/a/slash.php?log=123
//   // [tasmota,admin:joker] /any/link/starting/with/a/slash.php?log=123 --> Sends http://tasmota/any/link/starting/with/a/slash.php?log=123

//   char *host;
//   char *user;
//   char *password;
//   char *command;
//   int status = 1;                             // Wrong parameters

//                                               // buffer = |  [  192.168.178.86  :  80  ,  admin  :  joker  ]    POWER1 ON   |
//   host = strtok_r(buffer, "]", &command);     // host = |  [  192.168.178.86  :  80  ,  admin  :  joker  |, command = |    POWER1 ON   |
//   if (host && command) {
//     RemoveSpace(host);                        // host = |[192.168.178.86:80,admin:joker|
//     host++;                                   // host = |192.168.178.86:80,admin:joker| - Skip [
//     host = strtok_r(host, ",", &user);        // host = |192.168.178.86:80|, user = |admin:joker|
//     String url = F("http://");                // url = |http://|
//     url += host;                              // url = |http://192.168.178.86:80|

//     command = Trim(command);                  // command = |POWER1 ON| or |/any/link/starting/with/a/slash.php?log=123|
//     if (command[0] != '/') {
//       url += F("/cm?");                       // url = |http://192.168.178.86/cm?|
//       if (user) {
//         user = strtok_r(user, ":", &password);  // user = |admin|, password = |joker|
//         if (user && password) {
//           char userpass[200];
//           snprintf_P(userpass, sizeof(userpass), PSTR("user=%s&password=%s&"), user, password);
//           url += userpass;                    // url = |http://192.168.178.86/cm?user=admin&password=joker&|
//         }
//       }
//       url += F("cmnd=");                      // url = |http://192.168.178.86/cm?cmnd=| or |http://192.168.178.86/cm?user=admin&password=joker&cmnd=|
//     }
//     url += command;                           // url = |http://192.168.178.86/cm?cmnd=POWER1 ON|

//     Log::log(LOG_LEVEL_DEBUG, PSTR("WEB: Uri |%s|"), url.c_str());

//     WiFiClient http_client;
//     HTTPClient http;
//     if (http.begin(http_client, UrlEncode(url))) {  // UrlEncode(url) = |http://192.168.178.86/cm?cmnd=POWER1%20ON|
//       int http_code = http.GET();             // Start connection and send HTTP header
//       if (http_code > 0) {                    // http_code will be negative on error
//         if (http_code == HTTP_CODE_OK || http_code == HTTP_CODE_MOVED_PERMANENTLY) {
// #ifdef USE_WEBSEND_RESPONSE
//           // Return received data to the user - Adds 900+ bytes to the code
//           const char* read = http.getString().c_str();  // File found at server - may need lot of ram or trigger out of memory!
//           uint32_t j = 0;
//           char text = '.';
//           while (text != '\0') {
//             text = *read++;
//             if (text > 31) {                  // Remove control characters like linefeed
//               TasmotaGlobal.mqtt_data[j++] = text;
//               if (j == sizeof(TasmotaGlobal.mqtt_data) -2) { break; }
//             }
//           }
//           TasmotaGlobal.mqtt_data[j] = '\0';
//           MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_WEBSEND));
// #ifdef USE_SCRIPT
// extern uint8_t tasm_cmd_activ;
//           // recursive call must be possible in this case
//           tasm_cmd_activ=0;
//           XdrvRulesProcess();
// #endif  // USE_SCRIPT
// #endif  // USE_WEBSEND_RESPONSE
//         }
//         status = 0;                           // No error - Done
//       } else {
//         status = 2;                           // Connection failed
//       }
//       http.end();                             // Clean up connection data
//     } else {
//       status = 3;                             // Host not found or connection error
//     }
//   }
//   return status;
// }

// bool JsonWebColor(const char* dataBuf) {
//   // Default (Dark theme)
//   // {"WebColor":["#eaeaea","#252525","#4f4f4f","#000","#ddd","#65c115","#1f1f1f","#ff5661","#008000","#faffff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#faffff","#999","#eaeaea"]}
//   // Default pre v7 (Light theme)
//   // {"WebColor":["#000","#fff","#f2f2f2","#000","#fff","#000","#fff","#f00","#008000","#fff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#fff","#999","#000"]}	  // {"WebColor":["#000000","#ffffff","#f2f2f2","#000000","#ffffff","#000000","#ffffff","#ff0000","#008000","#ffffff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#ffffff","#999999","#000000"]}

//   JsonParser parser((char*) dataBuf);
//   JsonParserObject root = parser.getRootObject();
//   JsonParserArray arr = root[PSTR(D_CMND_WEBCOLOR)].getArray();
//   if (arr) {  // if arr is valid, i.e. json is valid, the key D_CMND_WEBCOLOR was found and the token is an arra
//     uint32_t i = 0;
//     for (auto color : arr) {
//       if (i < COL_LAST) {
//         WebHexCode(i, color.getStr());
//       } else {
//         break;
//       }
//       i++;
//     }
//   }
//   return true;
// }

// const char kWebSendStatus[] PROGMEM = D_JSON_DONE "|" D_JSON_WRONG_PARAMETERS "|" D_JSON_CONNECT_FAILED "|" D_JSON_HOST_NOT_FOUND "|" D_JSON_MEMORY_ERROR;

// const char kWebCommands[] PROGMEM = "|"  // No prefix
// #ifdef USE_EMULATION
//   D_CMND_EMULATION "|"
// #endif
// #ifdef USE_SENDMAIL
//   D_CMND_SENDMAIL "|"
// #endif
//   D_CMND_WEBSERVER "|" D_CMND_WEBPASSWORD "|" D_CMND_WEBLOG "|" D_CMND_WEBREFRESH "|" D_CMND_WEBSEND "|" D_CMND_WEBCOLOR "|"
//   D_CMND_WEBSENSOR "|" D_CMND_WEBBUTTON "|" D_CMND_CORS;

// void (* const WebCommand[])(void) PROGMEM = {
// #ifdef USE_EMULATION
//   &CmndEmulation,
// #endif
// #ifdef USE_SENDMAIL
//   &CmndSendmail,
// #endif
//   &CmndWebServer, &CmndWebPassword, &CmndWeblog, &CmndWebRefresh, &CmndWebSend, &CmndWebColor,
//   &CmndWebSensor, &CmndWebButton, &CmndCors };

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#ifdef USE_EMULATION
void CmndEmulation(void)
{
#if defined(USE_EMULATION_WEMO) || defined(USE_EMULATION_HUE)
#if defined(USE_EMULATION_WEMO) && defined(USE_EMULATION_HUE)
  if ((XdrvMailbox.payload >= EMUL_NONE) && (XdrvMailbox.payload < EMUL_MAX)) {
#else
#ifndef USE_EMULATION_WEMO
  if ((EMUL_NONE == XdrvMailbox.payload) || (EMUL_HUE == XdrvMailbox.payload)) {
#endif
#ifndef USE_EMULATION_HUE
  if ((EMUL_NONE == XdrvMailbox.payload) || (EMUL_WEMO == XdrvMailbox.payload)) {
#endif
#endif
    Settings.flag2.emulation = XdrvMailbox.payload;
    globalParams.setRestartFlag(2);
  }
#endif
  ResponseCmndNumber(Settings.flag2.emulation);
}
#endif  // USE_EMULATION

#ifdef USE_SENDMAIL
void CmndSendmail(void)
{
  if (XdrvMailbox.data_len > 0) {
    uint8_t result = SendMail(XdrvMailbox.data);
    char stemp1[20];
    ResponseCmndChar(Support::getTextIndexed(stemp1, sizeof(stemp1), result, kWebSendStatus));
  }
}
#endif  // USE_SENDMAIL


// void CmndwebServer(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
//     Settings.webServer = XdrvMailbox.payload;
//   }
//   if (Settings.webServer) {
//     Response_P(PSTR("{\"" D_CMND_WEBSERVER "\":\"" D_JSON_ACTIVE_FOR " %s " D_JSON_ON_DEVICE " %s " D_JSON_WITH_IP_ADDRESS " %s\"}"),
//       (2 == Settings.webServer) ? D_ADMIN : D_USER, NetworkHostname(), NetworkAddress().toString().c_str());
//   } else {
//     ResponseCmndStateText(0);
//   }
// }

// void CmndWebPassword(void)
// {
//   if (XdrvMailbox.data_len > 0) {
//     SettingsUpdateText(SET_WEBPWD, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? WEB_PASSWORD : XdrvMailbox.data);
//     ResponseCmndChar(SettingsText(SET_WEBPWD));
//   } else {
//     Response_P(S_JSON_COMMAND_ASTERISK, XdrvMailbox.command);
//   }
// }

// void CmndWeblog(void)
// {
//   if ((XdrvMailbox.payload >= LOG_LEVEL_NONE) && (XdrvMailbox.payload <= LOG_LEVEL_DEBUG_MORE)) {
//     Settings.weblog_level = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.weblog_level);
// }

// void CmndWebRefresh(void)
// {
//   if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload <= 10000)) {
//     Settings.web_refresh = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.web_refresh);
// }

// void CmndWebSend(void)
// {
//   if (XdrvMailbox.data_len > 0) {
//     uint32_t result = WebSend(XdrvMailbox.data);
//     char stemp1[20];
//     ResponseCmndChar(Support::getTextIndexed(stemp1, sizeof(stemp1), result, kWebSendStatus));
//   }
// }

// void CmndWebColor(void) {
//   if (XdrvMailbox.data_len > 0) {
//     if (strstr(XdrvMailbox.data, "{") == nullptr) {  // If no JSON it must be parameter
//       if ((XdrvMailbox.data_len > 3) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= COL_LAST)) {
//         WebHexCode(XdrvMailbox.index -1, XdrvMailbox.data);
//       }
//       else if (0 == XdrvMailbox.payload) {
//         SettingsDefaultWebColor();
//       }
//     }
//     else {
//       JsonWebColor(XdrvMailbox.data);
//     }
//   }
//   Response_P(PSTR("{\"" D_CMND_WEBCOLOR "\":["));
//   for (uint32_t i = 0; i < COL_LAST; i++) {
//     if (i) { ResponseAppend_P(PSTR(",")); }
//     ResponseAppend_P(PSTR("\"#%06x\""), WebColor(i));
//   }
//   ResponseAppend_P(PSTR("]}"));
// }

// void CmndWebSensor(void) {
//   if (XdrvMailbox.index < MAX_XSNS_DRIVERS) {
//     if (XdrvMailbox.payload >= 0) {
//       bitWrite(Settings.sensors[XdrvMailbox.index / 32], XdrvMailbox.index % 32, XdrvMailbox.payload &1);
//     }
//   }
//   Response_P(PSTR("{\"" D_CMND_WEBSENSOR "\":"));
//   XsnsSensorState();
//   ResponseJsonEnd();
// }

// void CmndWebButton(void)
// {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_BUTTON_TEXT)) {
//     if (!XdrvMailbox.usridx) {
//       ResponseCmndAll(SET_BUTTON1, MAX_BUTTON_TEXT);
//     } else {
//       if (XdrvMailbox.data_len > 0) {
//         SettingsUpdateText(SET_BUTTON1 + XdrvMailbox.index -1, ('"' == XdrvMailbox.data[0]) ? "" : XdrvMailbox.data);
//       }
//       ResponseCmndIdxChar(SettingsText(SET_BUTTON1 + XdrvMailbox.index -1));
//     }
//   }
// }

// void CmndCors(void)
// {
//   if (XdrvMailbox.data_len > 0) {
//     SettingsUpdateText(SET_CORS, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? WEB_PASSWORD : XdrvMailbox.data);
//   }
//   ResponseCmndChar(SettingsText(SET_CORS));
// }

// /*********************************************************************************************
//  * Interface
// \*********************************************************************************************/

// bool Xdrv01(uint8_t function)
// {
//   bool result = false;

//   switch (function) {
//     case FUNC_LOOP:
//       PollDnswebServer();
// #ifdef USE_EMULATION
//       if (Settings.flag2.emulation) { PollUdp(); }
// #endif  // USE_EMULATION
//       break;
//     case FUNC_COMMAND:
//       result = DecodeCommand(kWebCommands, WebCommand);
//       break;
//   }
//   return result;
// }
#endif  // USE_webServer
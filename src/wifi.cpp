/**
 *  @filename   :   main.cpp
 *  @brief      :   WiFi Nixie Clock, Wifi and OTA Control
 *  @author     :   Kevin Kessler
 *
 * Copyright (C) 2024 Kevin Kessler
 */

#include <WiFiManager.h>
#include <ElegantOTA.h>
#include "NixieClock.h"

WiFiManager wm;
WebServer webServer;
const char *hostname="NixieClock";

extern ntpConf nixieConf;

void wmCallback(WiFiManager *w) {
  displayError(WIFI_CONFIG);
}

void wifiSetup() {
  WiFi.setHostname(hostname);

  wm.setAPCallback(wmCallback);
  wm.setConfigPortalTimeout(180);
  wm.autoConnect("NixieClockAP");
  
  if(!WiFi.isConnected())
    wm.reboot();
}

void otaSetup() {
  webServer.on("/", [](){
    String message="<!DOCTYPE html><html>\n<head><title>";
    message += hostname;
    message += "</title></head>\n<body><center><H1>";
    message += hostname;
    message += " Configuration</H1></center><br>\n<center>";
    message += "<a href=\"/update\">Upload Firmware</a></center>";
    message += "<center><a href=\"/resetwifi\">Reset Wifi Settings</a></center>";

    message += "<form action=\"/ntpServer\"><center>NTP Server <input type=\"text\" name=\"ntpserver\" value=\"";
    message += nixieConf.ntpServer;
    message += "\"><input type=\"submit\" value=\"Submit\"></form>";

    message += "<form action=\"/tzInfo\"><center>NTP Server <input type=\"text\" name=\"tzinfo\" value=\"";
    message += nixieConf.tzInfo;
    message += "\"><input type=\"submit\" value=\"Submit\"></form>";
    message += "<center>Valid Posix Time zone Strings can be found here: <a href=\"https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv\"> https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv</a>";
    message +="</body></html>";
    webServer.send(200,"text/html",message);
  });

  webServer.on("/resetwifi", []() {
    wm.resetSettings();
    webServer.send(200,"text/plain","OK");
    wm.reboot();
  });

  webServer.on("/ntpServer", []() {
    String message="<!DOCTYPE html><html>\n<head><title>";
    message += hostname;
    message += "</title></head>\n<body><center><H1>";
    message += hostname;
    message += " Configuration</H1></center><br>\n<center>";
    message += "<br><center></h2> Save of NTP Server ";
    message += webServer.arg(0);
    message += " successful </h2></center></body></html>";

    bool retval=true;
    if(strcmp(webServer.argName(0).c_str(),"ntpserver")==0) {
        Serial.printf("New NTP Server %s\n",webServer.arg(0));
        retval=writeEEPROM((char *)webServer.arg(0).c_str(),nixieConf.tzInfo);
    } else {
        retval == false;
    }
    if(retval)
        webServer.send(200,"text/html",message);
    else
        webServer.send(500,"text/plain","Error occured saving EEPROM or parameter name incorrect");
  });

  webServer.on("/tzInfo", []() {
    String message="<!DOCTYPE html><html>\n<head><title>";
    message += hostname;
    message += "</title></head>\n<body><center><H1>";
    message += hostname;
    message += " Configuration</H1></center><br>\n<center>";
    message += "<br><center></h2> Save of Posix Time Zone ";
    message += webServer.arg(0);
    message += " successful </h2></center></body></html>";

    bool retval=true;
    if(strcmp(webServer.argName(0).c_str(),"tzinfo")==0) {
        Serial.printf("New NTP Server %s\n",webServer.arg(0));
        retval = writeEEPROM(nixieConf.ntpServer,(char *)webServer.arg(0).c_str());
    } else {
        retval == false;
    }
    if(retval)
        webServer.send(200,"text/html",message);
    else
        webServer.send(500,"text/plain","Error occured saving EEPROM or parameter name incorrect");
  });

  ElegantOTA.begin(&webServer);
  webServer.begin();

} 


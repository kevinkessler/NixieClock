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
    message += "<center><a href=\"/resetwifi\">Reset Wifi Settings</a></center></body></html>";
    webServer.send(200,"text/html",message);
  });

  webServer.on("/resetwifi", []() {
    wm.resetSettings();
    webServer.send(200,"text/plain","OK");
    wm.reboot();
  });

  ElegantOTA.begin(&webServer);
  webServer.begin();

} 


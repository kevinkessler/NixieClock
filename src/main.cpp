/**
 *  @filename   :   main.cpp
 *  @brief      :   WiFi Nixie Clock
 *  @author     :   Kevin Kessler
 *
 * Copyright (C) 2024 Kevin Kessler
 */

#include <Arduino.h>
#include <ElegantOTA.h>
#include "NixieClock.h"

extern int8_t errorFlag;

const char* NTP_SERVER = "pool.ntp.org";
// Posix Time Zone String - https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
const char* TZ_INFO    = "EST5EDT,M3.2.0,M11.1.0"; 
time_t now;
tm timeinfo;

extern WebServer webServer;

bool getNTPtime(uint16_t sec) {
  {
    uint32_t start = millis();
    do {
      time(&now);
      localtime_r(&now, &timeinfo);
      //Serial.print(".");
      delay(10);
    } while (((millis() - start) <= (1000 * sec)) && (timeinfo.tm_year < (2016 - 1900)));
    if (timeinfo.tm_year <= (2016 - 1900)) return false;  // the NTP call was not successful
    //Serial.print("now ");  Serial.println(now);
    char time_output[30];
    strftime(time_output, 30, "%a  %d-%m-%y %T", localtime(&now));
    //Serial.println(time_output);
    //Serial.println();
  }
  return true;
}


void showTime(tm localTime) {
  Serial.print(localTime.tm_mday);
  Serial.print('/');
  Serial.print(localTime.tm_mon + 1);
  Serial.print('/');
  Serial.print(localTime.tm_year - 100);
  Serial.print('-');
  Serial.print(localTime.tm_hour);
  Serial.print(':');
  Serial.print(localTime.tm_min);
  Serial.print(':');
  Serial.print(localTime.tm_sec);
  Serial.print(" Day of Week ");
  if (localTime.tm_wday == 0)   Serial.println(7);
  else Serial.println(localTime.tm_wday);
}

 
void ntpSetup() {
  configTime(0, 0, NTP_SERVER);

  setenv("TZ", TZ_INFO, 1);

  if (getNTPtime(1)) {  // wait up to 10sec to sync
  } else {
    //Serial.println("Time not set");
    //ESP.restart();
  }
}


void setup() {
  sleep(3);
  Serial.begin(115200);
  Serial.println("Begin");

  //initNixies();


  wifiSetup();

  otaSetup();

  //Serial.println("Output");
  //ntpSetup();
  //Serial.println("SetupOver");

}

uint8_t toggle=0;
unsigned long curTime=0;
void loop() {

/*  if(!getNTPtime(10))
    Serial.println("Fail");
  showTime(timeinfo);
//  Serial.println(mcp.digitalRead(1));

  allOff(&mcp);
  uint8_t dig=timeinfo.tm_sec - ((timeinfo.tm_sec/10) * 10);
  Serial.println(dig);
  firstDigit(&mcp,dig);*/

  if(curTime==0)
    curTime=millis();

  if(curTime < millis() - 1000) {
    if(errorFlag == -1)
    {
      allOff(0);
      allOff(1);
      allOff(2);

      uint8_t num=toggle * 10 + toggle;
      displayNums(0, num);
      displayNums(1, num);
      displayNums(2, num);
      Serial.println(toggle);  
      if(++toggle>10)
        toggle=0;
      curTime = millis();
    } else {
      allOff(0);
      allOff(1);
      allOff(2);

      if(toggle) {
        displayNums(0, errorFlag);
        displayNums(1, errorFlag);
        displayNums(2, errorFlag);
        toggle = 0; 
      } else {
        toggle = 1;
      }
    }
  }
 //   allOff(&mcp);
  //mcp.digitalWrite(1,toggle);
  /*allOff(&mcp);
  firstDigit(&mcp,toggle);
  toggle++;
  if(toggle==10)
    toggle=0;
  Serial.println(toggle);*/

  webServer.handleClient();
  ElegantOTA.loop();

}


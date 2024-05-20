/**
 *  @filename   :   time.cpp
 *  @brief      :   WiFi Nixie Clock, time realted functions
 *  @author     :   Kevin Kessler
 *
 * Copyright (C) 2024 Kevin Kessler
 */

#include <Arduino.h>
#include "NixieClock.h"

time_t now;
tm timeinfo;

extern ntpConf nixieConf;
extern int8_t errorFlag;

bool getNTPtime(uint16_t sec) {
  {
    uint32_t start = millis();
    do {
      time(&now);
      localtime_r(&now, &timeinfo);
      delay(10);
      Serial.print(".");
    } while (((millis() - start) <= (1000 * sec)) && (timeinfo.tm_year < (2016 - 1900)));
    if (timeinfo.tm_year <= (2016 - 1900)) return false;  // the NTP call was not successful
    errorFlag = NIXIE_NO_ERROR;
    char time_output[30];
    strftime(time_output, 30, "%a  %d-%m-%y %T", localtime(&now));

  }
  return true;
}

uint8_t getHour(void) {
    uint8_t hour = timeinfo.tm_hour;
    if(hour > 12)
        hour-=12;

    return hour;
}

uint8_t getMin(void) {
    return timeinfo.tm_min;
}

uint8_t getSec(void) {
    return timeinfo.tm_sec;
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
  configTime(0, 0, nixieConf.ntpServer);

  setenv("TZ", nixieConf.tzInfo, 1);

  if (getNTPtime(1)) {  // wait up to 1 sec to sync
  } else {
    Serial.println("Time not set");
    errorFlag = NTP_FAIL;
  }
}
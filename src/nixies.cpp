/**
 *  @filename   :   nixies.cpp
 *  @brief      :   WiFi Nixie Clock, GPIO Expander controller
 *  @author     :   Kevin Kessler
 *
 * Copyright (C) 2024 Kevin Kessler
 */

#include <Adafruit_MCP23X17.h>
#include <Wire.h>
#include "NixieClock.h"

Adafruit_MCP23X17 mcp0,mcp1,mcp2;
Adafruit_MCP23X17 *ios[3];

int8_t errorFlag=-1;

void displayError(uint8_t err) {
  Serial.println(err);
  errorFlag=err;
  
  /*allOff(&mcp0);
  allOff(&mcp1);
  allOff(&mcp2);

  displayNums(&mcp0, errorFlag);
  displayNums(&mcp1, errorFlag);
  displayNums(&mcp2, errorFlag);*/

}

void allOff(Adafruit_MCP23X17 *m) {
  return;

  m->digitalWrite(0,LOW);
  m->digitalWrite(1,LOW);
  m->digitalWrite(2,LOW);
  m->digitalWrite(3,LOW);
  m->digitalWrite(4,LOW);
  m->digitalWrite(5,LOW);
  m->digitalWrite(6,LOW);
  m->digitalWrite(7,LOW);
  m->digitalWrite(8,LOW);
  m->digitalWrite(9,LOW);
  m->digitalWrite(10,LOW);
  m->digitalWrite(11,LOW);
  m->digitalWrite(12,LOW);
  m->digitalWrite(13,LOW);
  m->digitalWrite(14,LOW);
  m->digitalWrite(15,LOW);
  m->digitalWrite(16,LOW);
}

void allOff(uint8_t idx) {
    allOff(ios[idx]);
}

void allOn(Adafruit_MCP23X17 *m) {
  m->digitalWrite(0,HIGH);
  m->digitalWrite(1,HIGH);
  m->digitalWrite(2,HIGH);
  m->digitalWrite(3,HIGH);
  m->digitalWrite(4,HIGH);
  m->digitalWrite(5,HIGH);
  m->digitalWrite(6,HIGH);
  m->digitalWrite(7,HIGH);
  m->digitalWrite(8,HIGH);
  m->digitalWrite(9,HIGH);
  m->digitalWrite(10,HIGH);
  m->digitalWrite(11,HIGH);
  m->digitalWrite(12,HIGH);
  m->digitalWrite(13,HIGH);
  m->digitalWrite(14,HIGH);
  m->digitalWrite(15,HIGH);
  m->digitalWrite(16,HIGH);
}

bool displayNums(Adafruit_MCP23X17 *m, uint8_t dig) {
  Serial.println(dig);
  return true;

  if (dig > 99)
      return false;

  uint8_t dig1=dig/10;
  uint8_t dig2 = dig - dig1*10;
  printf("Nums %u,%u\n",dig1,dig2);
  allOff(m);

  m->digitalWrite(dig1,HIGH);

  switch(dig2) {
      case 0:
        m->digitalWrite(10,HIGH);
        break;
      case 1:
        m->digitalWrite(11,HIGH);
        break;
      case 2:
        m->digitalWrite(PIN_E,HIGH);
        m->digitalWrite(PIN_A0,LOW);
        m->digitalWrite(PIN_A1,LOW);
        m->digitalWrite(PIN_A2,LOW);
        break;
      case 3:
        m->digitalWrite(PIN_E,HIGH);
        m->digitalWrite(PIN_A0,HIGH);
        m->digitalWrite(PIN_A1,LOW);
        m->digitalWrite(PIN_A2,LOW);
        break;
      case 4:
        m->digitalWrite(PIN_E,HIGH);
        m->digitalWrite(PIN_A0,LOW);
        m->digitalWrite(PIN_A1,HIGH);
        m->digitalWrite(PIN_A2,LOW);
        break;
      case 5:
        m->digitalWrite(PIN_E,HIGH);
        m->digitalWrite(PIN_A0,HIGH);
        m->digitalWrite(PIN_A1,HIGH);
        m->digitalWrite(PIN_A2,LOW);
        break;
      case 6:
        m->digitalWrite(PIN_E,HIGH);
        m->digitalWrite(PIN_A0,LOW);
        m->digitalWrite(PIN_A1,LOW);
        m->digitalWrite(PIN_A2,HIGH);
        break;
      case 7:
        m->digitalWrite(PIN_E,HIGH);
        m->digitalWrite(PIN_A0,HIGH);
        m->digitalWrite(PIN_A1,LOW);
        m->digitalWrite(PIN_A2,HIGH);
        break;
      case 8:
        m->digitalWrite(PIN_E,HIGH);
        m->digitalWrite(PIN_A0,LOW);
        m->digitalWrite(PIN_A1,HIGH);
        m->digitalWrite(PIN_A2,HIGH);
        break;
      case 9:
        m->digitalWrite(PIN_E,HIGH);
        m->digitalWrite(PIN_A0,HIGH);
        m->digitalWrite(PIN_A1,HIGH);
        m->digitalWrite(PIN_A2,HIGH);
        break;      
      default:
        allOff(m);
    }

    return true;
}

bool displayNums(uint8_t idx, uint8_t dig) {
    return displayNums(ios[idx], dig);
}

void mcpOutput(Adafruit_MCP23X17 *m) {
  m->pinMode(0, OUTPUT);
  m->pinMode(1, OUTPUT);
  m->pinMode(2, OUTPUT);
  m->pinMode(3, OUTPUT);
  m->pinMode(4, OUTPUT);
  m->pinMode(5, OUTPUT);
  m->pinMode(6, OUTPUT);
  m->pinMode(7, OUTPUT);
  m->pinMode(8, OUTPUT);
  m->pinMode(9, OUTPUT);
  m->pinMode(10, OUTPUT);
  m->pinMode(11, OUTPUT);
  m->pinMode(12, OUTPUT);
  m->pinMode(13, OUTPUT);
  m->pinMode(14, OUTPUT);
  m->pinMode(15, OUTPUT);
  m->pinMode(16, OUTPUT);

}

void initNixies() {
  uint8_t retval=Wire.begin(10,8,100000);

  if (!mcp0.begin_I2C(0x20,&Wire)) {

    Serial.println("Error.");
    while (1) {
      Serial.printf("Broke 0 %u\n",retval);
      sleep(1);
    }
  }

  if (!mcp1.begin_I2C(0x21,&Wire)) {

    Serial.println("Error.");
    while (1) {
      Serial.printf("Broke 1 %u\n",retval);
      sleep(1);
    }
  }
  
  if (!mcp2.begin_I2C(0x22,&Wire)) {

    Serial.println("Error.");
    while (1) {
      Serial.printf("Broke 2 %u\n",retval);
      sleep(1);
    }
  }
    
  ios[0]=&mcp0;
  ios[1]=&mcp1;
  ios[2]=&mcp2;

  mcpOutput(&mcp0);
  mcpOutput(&mcp1);
  mcpOutput(&mcp2);

  allOff(&mcp0);
  allOff(&mcp1);
  allOff(&mcp2);
}


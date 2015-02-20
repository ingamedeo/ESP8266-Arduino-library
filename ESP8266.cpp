/*
  ESP8266.h - Library for ESP8266.
  Created by Tommaso and Amedeo, February 18, 2015.
  Released under GNU GPL v3.
*/

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "ESP8266.h"
#include <avr/wdt.h>

#define MAX_CYCLES 300

  ESP8266::ESP8266(uint8_t rx, uint8_t tx) {
  	_espSerial = new SoftwareSerial(rx, tx);

    /* Char array init */
    _ipdSt[0] = '+'; _ipdSt[1] = 'I'; _ipdSt[2] = 'P'; _ipdSt[3] = 'D'; _ipdSt[4] = ',';
    _getSt[0] = 'G'; _getSt[1] = 'E'; _getSt[2] = 'T';
    _postSt[0] = 'P'; _postSt[1] = 'O'; _postSt[2] = 'S'; _postSt[3] = 'T';
    _okSt[0] = 'O'; _okSt[1] = 'K';
  }

  bool ESP8266::begin(int BAUDRATE, int PORT) {
  //Init serial connection with ESP8266
  	_espSerial->begin(BAUDRATE);
  	_espSerial->println("AT+RST");
  	delay(2000);
  _espSerial->flush(); //Flush weird stuff
  delay(100);
  _espSerial->println("AT+CIPMUX=1");
  if (!waitResponse(_okSt)) {
  	wdt_enable(WDTO_15MS);
  }
  	delay(100);
  _espSerial->print("AT+CIPSERVER=1,");
  _espSerial->println(PORT);
  if (!waitResponse(_okSt)) {
  	wdt_enable(WDTO_15MS);
  }
  delay(100);
}

int ESP8266::getRequest() {

  //Search for +IPD,
  while (_espSerial->available()) {
    if (search(_ipdSt)) {
      count = 0;
      char id = _espSerial->read();

      while (_espSerial->available()) {
        if (search(getSt)) {
          count = 0;
          skip(3);
          _paramsLastRead = 0;
          while (_espSerial->available()) {
            add(_espSerial->read());
          }
        }
      }
    }

  }

  return -1;
}

char* ESP8266::getRequestParams(int id) {

}

void ESP8266::sendAnswer(int id, char*) {

}

void ESP8266::skip(int count) {
  int i;
  for (i = 0; i < count; i++) {
    _espSerial.read();
  }
}

char* ESP8266::serialRead(int len) {

}

bool ESP8266::search(char* text) {
	char c;
	bool orRes = false;
	int i;
	int len = strlen(text); //Get text length
  //Read just one char from serial buffer
  c = _espSerial->read();
  /* For each char in text */
  for (i = 0; i < len; i++) {
  	if (c == text[i]) {
  		orRes = true;
  	} 
  }
  if (orRes == true) {
  	_count++;
  } 
  else {
  	if (_count > 0) {
  		_count--;
  	}
  }
  if (_count==len) {
  	return true;
  } 
  else {
  	return false;
  }
}

bool ESP8266::waitResponse(char* text) {
int timeout = 0;
_count = 0;
while (!_espSerial->available() && timeout <= MAX_CYCLES) { //Wait until we get some data in...
	delay(10);
	timeout++;
} 
while (timeout <= MAX_CYCLES) { //We got sth
	while (_espSerial->available() && timeout <= MAX_CYCLES) {
		if (search(text)) {
			return true;
		} else {
			delay(10);
			timeout++;
		}
	}
	delay(10);
	timeout++;
}
return false;
}

void ESP8266::add(char ch) {
  _paramsLastRead++;
  if (_paramsLastRead==1) {
    _params = (char*) malloc(_paramsLastRead * sizeof(char));
    strcpy(_params, ch);
  } else {
    _params = (char*) realloc(_params, _paramsLastRead * sizeof(char));
    strcat(_params, ch);
  }
}
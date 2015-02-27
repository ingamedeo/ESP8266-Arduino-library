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
    _httpSt[0] = ' '; _httpSt[1] = 'H'; _httpSt[2] = 'T'; _httpSt[3] = 'T'; _httpSt[4] = 'P'; _httpSt[5] = '/';
    _okSt[0] = 'O'; _okSt[1] = 'K';
    
    clearArray();
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

int8_t ESP8266::getRequest() {

  //Search for +IPD,
  while (_espSerial->available()) {
    if (search(_ipdSt, false)) {
      _count = 0;
      char id = _espSerial->read();
	//search for GET
      while (_espSerial->available()) {
        if (search(_getSt, false)) {
          _count = 0;
          skip(3);

	//Search for  HTTP/
          _paramsLastRead = 0;
          clearArray();
          while (_espSerial->available()) {
            if(search(_httpSt, true)) {
		//id=2 HTTP/
		uint8_t i;
		for (i = 1; i < 7; i++) {
			_params[_paramsLastRead - i] = 0;
		}

		//Break the whiles
             _espSerial->flush();
             return id;
           }
         }
       }
     }
   }

 }

 return -1;
}

char* ESP8266::getRequestParams() {
	return _params;
}

void ESP8266::sendAnswer(int8_t id, char* response) {
	_espSerial->print("AT+CIPSEND=");
	_espSerial->print((char) id);
	_espSerial->print(",");
	_espSerial->println(strlen(response)+1);
  delay(100);
  _espSerial->print(response);
  _espSerial->println('\0');
  delay(100);
  _espSerial->print("AT+CIPCLOSE=");
  _espSerial->println((char) id);
}


void ESP8266::skip(uint16_t count) {
  uint16_t i;
  for (i = 0; i < count; i++) {
    _espSerial->read();
  }
}

bool ESP8266::search(char* text, bool allocate) {
	char c;
	bool orRes = false;
	uint16_t i;
	uint16_t len = strlen(text); //Get text length
  //Read just one char from serial buffer
  c = _espSerial->read();
//allocate the char if requested
  if (allocate) {
   _params[_paramsLastRead] = c;
   _paramsLastRead++;
 }
  /* For each char in text */
 for (i = 0; i < len; i++) {
   if (c == text[i]) {
    orRes = true;
    _count++;
    break;
  }
}
if (!orRes && _count > 0) {
 _count--;
}
if (_count==len) {
 return true;
} 
else {
 return false;
}
}

bool ESP8266::waitResponse(char* text) {
  uint16_t timeout = 0;
  _count = 0;
while (!_espSerial->available() && timeout <= MAX_CYCLES) { //Wait until we get some data in...
	delay(10);
	timeout++;
} 
while (timeout <= MAX_CYCLES) { //We got sth
	while (_espSerial->available() && timeout <= MAX_CYCLES) {
		if (search(text, false)) {
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

void ESP8266::clearArray() {
  uint16_t i;
  for (i = 0; i < strlen(_params); ++i) {
    _params[i] = 0;
  }
}

/*
  ESP8266.h - Library for ESP8266.
  Created by Tommaso and Amedeo, February 18, 2015.
  Released under GNU GPL v3.
*/

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "ESP8266.h"
#include <avr/wdt.h>

  ESP8266::ESP8266(byte rx, byte tx) {
  	_espSerial = SoftwareSerial(rx, tx);
  }

  bool ESP8266::begin(char* BAUDRATE, char* PORT) {
  //Init serial connection with ESP8266
  	_espSerial.begin(9600);
  	_espSerial.println("AT+RST");
  	delay(2000);
  _espSerial.flush(); //Flush weird stuff
  delay(100);
  _espSerial.println("AT+CIPMUX=1");
  if (!waitResponse()) {
  	wdt_enable(WDTO_15MS);
  }
  	delay(100);
  _espSerial.println("AT+CIPSERVER=1,8080");
  if (!waitResponse()) {
  	wdt_enable(WDTO_15MS);
  }
  delay(100);
}

int ESP8266::getRequest() {

}

char* ESP8266::getRequestParams(int id) {

}

void ESP8266::sendAnswer(int id, char*) {

}

void ESP8266::skip(int count) {

}

char* ESP8266::serialRead(int len) {

}

bool ESP8266::search(char* text) {
	char c;
	bool orRes = false;
	int i;
	int len = strlen(text); //Get text length
  //Read just one char from serial buffer
  c = espSerial.read();
  /* For each char in text */
  for (i = 0; i < len; i++) {
  	if (c == text[i]) {
  		orRes = true;
  	} 
  }
  if (orRes == true) {
  	count++;
  } 
  else {
  	if (count > 0) {
  		count--;
  	}
  }
  if (count==len) {
  	return true;
  } 
  else {
  	return false;
  }
}

bool ESP8266::waitResponse(char* text) {
int timeout = 0;
_count = 0;
while (!_espSerial.available() && timeout <= 300) { //Wait until we get some data in...
	delay(10);
	timeout++;
} 
while (timeout <= 300) { //We got sth
	while (_espSerial.available() && timeout <= 300) {
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
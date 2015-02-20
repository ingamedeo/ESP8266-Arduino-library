/*
  ESP8266.h - Library for ESP8266.
  Created by Tommaso and Amedeo, February 18, 2015.
  Released under GNU GPL v3.
*/

#ifndef ESP8266_h
#define ESP8266_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class ESP8266 {

public:
	ESP8266(byte rx, byte tx);
	bool begin(char* BAUDRATE, char* PORT);
	int getRequest(); //(get id, save params)
	char* getRequestParams(int id); //(get params)
	void sendAnswer(int id, char*);
private:
	void skip(int count);
	char* serialRead(int len);
	bool search(char* text);
	bool waitResponse(char* text);
	char _ipdSt[6] = "+IPD,";
	char _getSt[4] = "GET";
	char _postSt[5] = "POST";
	char _okSt[3] = "OK";
	int _resultLen = 0;
	char* _result;
	int _count = 0;
	SoftwareSerial _espSerial;
};

#endif
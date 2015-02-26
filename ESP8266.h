/*
  ESP8266.h - Library for ESP8266.
  Created by Tommaso and Amedeo, February 18, 2015.
  Released under GNU GPL v3.
*/

#ifndef ESP8266_h
#define ESP8266_h

#include "Arduino.h"
#include <SoftwareSerial.h>

#define PARAMS_MAX_LEN 50

class ESP8266 {

public:
	ESP8266(uint8_t rx, uint8_t tx);
	bool begin(int BAUDRATE, int PORT);
	int getRequest(); //(get id, save params)
	char* getRequestParams(); //(get params)
	bool sendAnswer(int id, char* response);
private:
	void skip(int count);
	bool search(char* text, bool allocate);
	bool waitResponse(char* text);
	void clearArray();
	char _ipdSt[6];
	char _getSt[4];
	char _postSt[5];
	char _httpSt[7];
	char _okSt[3];
	char _arrSt[2];
	int _resultLen = 0;
	char* _result;
	char _params[PARAMS_MAX_LEN];
	int _count = 0;
	int _paramsLastRead = 0;
	SoftwareSerial* _espSerial;
};

#endif

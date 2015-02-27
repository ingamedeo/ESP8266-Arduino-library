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
	int8_t getRequest(); //(get id, save params)
	char* getRequestParams(); //(get params)
	void sendAnswer(int8_t id, char* response);
private:
	void skip(uint16_t count);
	bool search(char* text, bool allocate);
	bool waitResponse(char* text);
	void clearArray();
	char _ipdSt[6];
	char _getSt[4];
	char _postSt[5];
	char _httpSt[7];
	char _okSt[3];
	uint16_t _resultLen = 0;
	char* _result;
	char _params[PARAMS_MAX_LEN];
	uint16_t _count = 0;
	uint16_t _paramsLastRead = 0;
	SoftwareSerial* _espSerial;
};

#endif

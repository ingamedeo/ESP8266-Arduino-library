#include "ESP8266.h"
#include "SoftwareSerial.h"
//SoftwareSerial dbgSerial(4,5);

ESP8266 esp(2,3);

void setup() {
  Serial.begin(9600);
  esp.begin(9600, 80);
}

void loop() {
 int reqID = esp.getRequest();
 if(reqID != -1) {
   Serial.println((char) reqID);
   Serial.println(esp.getRequestParams());
   bool check = esp.sendAnswer(reqID, "Hello");
   if (check) {
     Serial.println("Okay");
   } else {
    Serial.println("Nope"); 
   }
  } else {
   Serial.println("."); 
  }
  
  

}

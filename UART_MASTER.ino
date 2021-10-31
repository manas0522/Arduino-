#include<SoftwareSerial.h>
SoftwareSerial master(3,4); //Rx, Tx
void setup() {
  Serial.begin(9600);
  master.begin(115200);
} 
void loop() {
   if(Serial.available()){
     String message = Serial.readString();
     Serial.println("> " + message);
     master.print("< " + message);
   }
   if(master.available())
  {
    Serial.println(master.readString());
  }
}

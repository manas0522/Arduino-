#include <Wire.h>

#define eeprom 0x50 //defines the base address of the EEPROM

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data ) {
Wire.beginTransmission(deviceaddress);
Wire.write((int)(eeaddress >> 8)); //writes the MSB
Wire.write((int)(eeaddress & 0xFF)); //writes the LSB
Wire.write(data);
Wire.endTransmission();
}

byte readEEPROM(int deviceaddress, unsigned int eeaddress ) {
byte rdata = 0xFF;
Wire.beginTransmission(deviceaddress);
Wire.write((int)(eeaddress >> 8)); //writes the MSB
Wire.write((int)(eeaddress & 0xFF)); //writes the LSB
Wire.endTransmission();
Wire.requestFrom(deviceaddress,1);
if (Wire.available())
rdata = Wire.read();
return rdata;
}


void setup(void){
int i;
 
Wire.begin(); //creates a Wire object
Serial.begin(9600);

unsigned int address = 10; //first address of the EEPROM
Serial.println("writing..");


//writeEEPROM(eeprom, address, 267); // Writes 22222 to the EEPRO(10)


Serial.println(readEEPROM(eeprom, address));


}
void loop(){
//there's nothing in the loop() function because we don't want the arduino to repeatedly write the same thing to the EEPROM over and over. We just want a one-time write, so the loop() function is avoided with EEPROMs.
}

//defines the writeEEPROM function

//defines the readEEPROM function

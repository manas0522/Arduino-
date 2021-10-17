void setup() {
  Serial.begin(9600);
  //Serial.setTimeout(2000);
  pinMode(2,OUTPUT);
}

void loop() {
  if(Serial.available())
  {
    char s = Serial.read();
    if (s == 's') {
      Serial.println("Good night Giong to bed for 30 secs.");
      ESP.deepSleep(30e6);// 30x10^6 micro Seconds. = 30Seconds.
    }
  }
  else
  {
    digitalWrite(D4,1);
    delay(500);
    digitalWrite(D4,0);
    delay(500);
  }
}

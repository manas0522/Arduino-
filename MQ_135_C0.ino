
// THIS PARTICULAR CODE IS VALID FOR 'CO', BECAUSE FOR DIFFERENT GASES m,b CHANGES.



float m = -0.25; //Slope 
float b = 0.70; //Y-Intercept 
float R0 = 11.820; //Sensor Resistance in fresh air from previous code{ROUGHLY TAKEN}

void setup() {
  Serial.begin(9600); //Baud rate
 
}
void loop(){
   float sensor_volt; //Define variable for sensor voltage 
  float RS_gas; //Define variable for sensor resistance  
  float ratio; //Define variable for ratio
  float sensorValue = analogRead(A0); //Read analog values of sensor  
  sensor_volt = sensorValue*(5.0/1023.0); //Convert analog values to voltage 
  RS_gas = ((5.0*10.0)/sensor_volt)-10.0; //Get value of RS in a gas {WHERE VCC DENOTES 5.0V, AND RL DENOTES 10.0K}
  ratio = RS_gas/R0;  // Get ratio RS_gas/RS_air
 
  double ppm_log = (log10(ratio)-b)/m; //Get ppm value in linear scale according to the the ratio value  
  double ppm = pow(10, ppm_log); //Convert ppm value to log scale 
  double percentage = ppm/10000; //Convert to percentage 

 Serial.print(ppm);
 Serial.println();
  
}

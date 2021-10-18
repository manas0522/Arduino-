#include <Scheduler.h>
#include <PDM.h>

#include <mask_test_inferencing.h>

#define EIDSP_QUANTIZE_FILTERBANK   0
#define led 22
#define button 23  // change accordingly as ble 33 sense pinout.
#include <ArduinoBLE.h>
#include <math.h>

/** Audio buffers, pointers and selectors */
typedef struct {
    int16_t *buffer;
    uint8_t buf_ready;
    uint32_t buf_count;
    uint32_t n_samples;
} inference_t;
static inference_t inference;
static bool record_ready = false;
static signed short sampleBuffer[2048];
static bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal



BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify);

BLEDescriptor switchDescriptor("00002902-0000-1000-8000-00805f9b34fb","led-service");


const int ledPin = LED_BUILTIN; // pin to use for the LED
float temp = 0;
int c=0;

// functions use in loop 2
void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;
  
    // Extract floating part
    float fpart = n - (float)ipart;
  
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
  
    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot
  
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter 
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
  
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}
  
// Converts a given integer x to string str[]. 
// d is the number of digits required in the output. 
// If d is more than the number of digits in x, 
// then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
  
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
  
    reverse(str, i);
    str[i] = '\0';
    return i;
}

void ei_printf(const char *format, ...) {
    static char print_buf[1024] = { 0 };
    va_list args;
    va_start(args, format);
    int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
    va_end(args);
    if (r > 0) {
        Serial.write(print_buf);
    }
}
static void pdm_data_ready_inference_callback(void)
{
    int bytesAvailable = PDM.available();
    // read into the sample buffer
    int bytesRead = PDM.read((char *)&sampleBuffer[0], bytesAvailable);
    if (record_ready == true || inference.buf_ready == 1) {
        for(int i = 0; i < bytesRead>>1; i++) {
            inference.buffer[inference.buf_count++] = sampleBuffer[i];
            if(inference.buf_count >= inference.n_samples) {
                inference.buf_count = 0;
                inference.buf_ready = 1;
            }
        }
    }
}
static bool microphone_inference_start(uint32_t n_samples)
{
    inference.buffer = (int16_t *)malloc(n_samples * sizeof(int16_t));
    if(inference.buffer == NULL) {
        return false;
    }
    inference.buf_count  = 0;
    inference.n_samples  = n_samples;
    inference.buf_ready  = 0;
    // configure the data receive callback
    PDM.onReceive(&pdm_data_ready_inference_callback);
    // optionally set the gain, defaults to 20
    PDM.setGain(80);
    //ei_printf("Sector size: %d nblocks: %d\r\n", ei_nano_fs_get_block_size(), n_sample_blocks);
    PDM.setBufferSize(4096);
    // initialize PDM with:
    // - one channel (mono mode)
    // - a 16 kHz sample rate
    if (!PDM.begin(1, EI_CLASSIFIER_FREQUENCY)) {
        ei_printf("Failed to start PDM!");
    }
    record_ready = true;
    return true;
}
static bool microphone_inference_record(void)
{
    inference.buf_ready = 0;
    inference.buf_count = 0;
    while(inference.buf_ready == 0) {
        delay(10);
    }
    return true;
}
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr)
{
    arm_q15_to_float(&inference.buffer[offset], out_ptr, length);
    return 0;
}
static void microphone_inference_end(void)
{
    PDM.end();
    free(inference.buffer);
}
#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_MICROPHONE
#error "Invalid model for current sensor."
#endif
void alarm(){
  for (size_t t = 0; t < 4; t++) {
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);

void setup() {

  Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    pinMode(button, INPUT);
    pinMode(led, OUTPUT);
    Serial.println("Edge Impulse Inferencing Demo");
    // summary of inferencing settings (from model_metadata.h)
    ei_printf("Inferencing settings:\n");
    ei_printf("\tInterval: %.2f ms.\n", (float)EI_CLASSIFIER_INTERVAL_MS);
    ei_printf("\tFrame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    ei_printf("\tSample length: %d ms.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT / 16);
    ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));
    if (microphone_inference_start(EI_CLASSIFIER_RAW_SAMPLE_COUNT) == false) {
        ei_printf("ERR: Failed to setup audio sampling\r\n");
        return;
    }

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);
  switchCharacteristic.addDescriptor(switchDescriptor);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising

 /* if (button==HIGH)
  {
  BLE.advertise();
  Serial.println("BLE LED Peripheral");
}*/  

    


  Scheduler.startLoop(loop2);
}

// Task no.1: cough detection
void loop() {

  ei_printf("Starting inferencing in 2 seconds...\n");
    delay(2000);
    ei_printf("Recording...\n");
    bool m = microphone_inference_record();
    if (!m) {
        ei_printf("ERR: Failed to record audio...\n");
        return;
    }
    ei_printf("Recording done\n");
    signal_t signal;
    signal.total_length = EI_CLASSIFIER_RAW_SAMPLE_COUNT;
    signal.get_data = &microphone_audio_signal_get_data;
    ei_impulse_result_t result = { 0 };
    EI_IMPULSE_ERROR r = run_classifier(&signal, &result, debug_nn);
    if (r != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", r);
        return;
    }
    // print the predictions
    ei_printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n",
        result.timing.dsp, result.timing.classification, result.timing.anomaly);
    for (size_t ix = 1; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    Serial.print( result.classification[ix].value);
    float Data = result.classification[ix].value;
    if (Data < 0.50){
    Serial.print("Cough Detected");
    alarm();
    }
    }    
    //for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    // ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
  // }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("    anomaly score: %.3f\n", result.anomaly);
#endif
}
  

  // IMPORTANT:

  // When multiple tasks are running 'delay' passes control to

  // other tasks while waiting and guarantees they get executed.

  

// Task no.2: ble operation

void loop2() {
   if (button==HIGH) // check the button- first press treated like on, then again treated like off
   c++;
    
  if (c%2!==0)
  {  
   Serial.println("button on");  
  BLE.advertise();
  Serial.println("BLE LED Peripheral");
    
    // listen for BLE peripherals to connect:
  
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {   // any value other than 0
          Serial.println(switchCharacteristic.value());
          Serial.println("LED on");
          digitalWrite(ledPin, HIGH);         // will turn the LED on
        } else  {                              // a 0 value
          Serial.println(switchCharacteristic.value());
          Serial.println(F("LED off"));
          digitalWrite(ledPin, LOW);          // will turn the LED off
        }
      }


          float tempAmbiente = random(1,500);
          char txString[8]; // make sure this is big enuffz
          ftoa(tempAmbiente,txString,2);

          Serial.println(txString);

          switchCharacteristic.writeValue(txString); 
//          
//         if(tempAmbiente != temp){
//          Serial.println(tempAmbiente);
//
//          switchCharacteristic.writeValue(tempAmbiente); 
//          temp=tempAmbiente;
//          }

      
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
 else
 {
  Serial.println("button off");
 break;
 }
  
}   


  yield();
}

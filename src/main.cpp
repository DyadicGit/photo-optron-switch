#include <Arduino.h>

// machine variables
const int ledPin = LED_BUILTIN;
const int QRD1114_PIN = A0;
// state variables
volatile byte ledState = LOW;
bool objectDetected = false;
// main variables
float roundedPhotoOptronVoltage;

void readPhotoOptron();
float roundToOneDecimal(float floatValue) ;
void updateObjectDetectedState();
void delayLedStateChange();

// time delay declarations
unsigned long int currentMillis = 0;
unsigned long int delayByMillisPreviouse = 0;
bool isDelayTime(unsigned long int delayByMillis);

void setup()
{
  pinMode(QRD1114_PIN, INPUT);
  pinMode(ledPin, OUTPUT);

  // Serial.begin(9600);
}

void loop()
{
  currentMillis = millis();

  readPhotoOptron();
  updateObjectDetectedState();
  delayLedStateChange();
  digitalWrite(ledPin, ledState);
  // Serial.println(roundedPhotoOptronVoltage);
}

void readPhotoOptron() {
  int photoOptronADC = analogRead(QRD1114_PIN);
  float photoOptronVoltage = (float) (photoOptronADC * 5.0) / 1024.0;
  roundedPhotoOptronVoltage = roundToOneDecimal(photoOptronVoltage);
}

float roundToOneDecimal(float floatValue) {
  return ((float)((int)(floatValue*10)))/10;
}

void updateObjectDetectedState() {
  if (roundedPhotoOptronVoltage < 3.5) {
    objectDetected = true;
  } else {
    objectDetected = false;
  }
}

void changeLedState() {
  if (objectDetected) {
    ledState = HIGH;
  }
}
void delayLedStateChange() {
  if (isDelayTime(1000)) {
    changeLedState();
	}
  if (!objectDetected) {
    ledState = LOW;
  }
}

// delay functions
bool isDelayTime(unsigned long int delayByMillis) {
  currentMillis = millis();
  bool result = (currentMillis - delayByMillisPreviouse >= delayByMillis) || (currentMillis < delayByMillis);
  if (result) {delayByMillisPreviouse = currentMillis;}
  return result;
}

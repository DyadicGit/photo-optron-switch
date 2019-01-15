#include <Arduino.h>

// machine variables
const int ledPin = PD0;
const int QRD1114_PIN = PC0;
const int TRIGGER_DELAY = 1000;
const int TRIGGER_VOLTAGE = 4.8;
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
}

void loop()
{
  currentMillis = millis();

  readPhotoOptron();
  updateObjectDetectedState();
  delayLedStateChange();
  digitalWrite(ledPin, ledState);
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
  if (roundedPhotoOptronVoltage <= TRIGGER_VOLTAGE) {
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
  if (isDelayTime(TRIGGER_DELAY)) {
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

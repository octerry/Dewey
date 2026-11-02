#include <Arduino.h>

const int LED_PIN = D3;
const int MIDDLE_TOUCH_PIN = 0;
const int PLUS_TOUCH_PIN = D1;
const int MINUS_TOUCH_PIN = D2;
const int MAX_LED_VALUE = 255;

float ledProgress = 0;
float ledValue = 1;
bool middleWasTouched = false;
bool plusWasTouched = false;
bool minusWasTouched = false;

void setup() {
  Serial.begin(115200);

  pinMode(MIDDLE_TOUCH_PIN, INPUT);
  pinMode(PLUS_TOUCH_PIN, INPUT);
  pinMode(MINUS_TOUCH_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if (digitalRead(PLUS_TOUCH_PIN) && ledValue < 1 && !plusWasTouched) {
    ledValue += .1;
    plusWasTouched = true;
  } else if (!digitalRead(PLUS_TOUCH_PIN)) {
    plusWasTouched = false;
  }
  
  if (digitalRead(MINUS_TOUCH_PIN) && ledValue > 0 && !minusWasTouched) {
    ledValue -= .1;
    minusWasTouched = true;
  } else if (!digitalRead(MINUS_TOUCH_PIN)) {
    minusWasTouched = false;
  }

  if (ledValue < 0) {
    ledValue = 0;
  }

  // ledProgress += .01;
  // ledValue = (cos(ledProgress) + 1) / 2;

  if (digitalRead(MIDDLE_TOUCH_PIN)) {
    analogWrite(LED_PIN, 0);
  }
  else analogWrite(LED_PIN, ledValue * MAX_LED_VALUE);
  delay(10);
}
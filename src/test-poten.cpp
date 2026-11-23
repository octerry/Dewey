#include <Arduino.h>

const int POT_PIN = A0;   // D4 = GPIO22 sur la XIAO ESP32C6

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);          // 0 à 4095
  analogSetAttenuation(ADC_11db);    // plage ~0 à 3,3 V
}

void loop() {
  int valeur = analogRead(POT_PIN);
  float tension = valeur * 3.3 / 4095.0;
  int pourcentage = map(valeur, 0, 4095, 0, 100);

  Serial.printf("Brut: %d | Tension: %.2f V | %d %%\n", valeur, tension, pourcentage);

  delay(100);
}
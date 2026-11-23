#include <Arduino.h>
#include <ESP32Servo.h>

// Broche connectée au fil signal (orange/jaune) du SG90
// Choisis une broche PWM valide de la Xiao ESP32C6, par ex. D2 (GPIO2)
const int servoPin = D2;

Servo monServo;

void setup() {
  Serial.begin(115200);

  // Autorise l'allocation de tous les timers PWM disponibles
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  monServo.setPeriodHertz(50);       // fréquence standard des servos 50Hz
  monServo.attach(servoPin, 500, 2400); // min/max pulse width en µs (à ajuster si besoin)

  Serial.println("Servo initialisé");
}

void loop() {
  // Balayage de 0° à 180°
  float deg = 90;
  float time = 1000;

  for (int i = 0; i < time; i++) {
    monServo.write(i * (deg/time));
    delay(1);
  }

  delay(500);

  // Retour de 180° à 0°
  for (int i = 0; i < time; i++) {
    monServo.write(deg - (i * (deg/time)));
    delay(1);
  }

  delay(500);
}
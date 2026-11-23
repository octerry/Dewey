#include <Arduino.h>
#include <ESP32Servo.h>

// Broche connectée au fil signal (orange/jaune) du SG90
// Choisis une broche PWM valide de la Xiao ESP32C6, par ex. D2 (GPIO2)
const int servoPin = D2;

Servo monServo;

// INPUT: Potentiometer should be connected to 5V and GND
int potPin = A0; // Potentiometer output connected to analog pin 3
int potVal = 0; // Variable to store the input from the potentiometer

// OUTPUT: Use digital pins 9-11, the Pulse-width Modulation (PWM) pins
// LED's cathodes should be connected to digital GND
int redPin = 3;   // Red LED,   connected to digital pin 9*

// Program variables
int redVal = 0;   // Variables to store the values to send to the pins
int grnVal = 0;
int bluVal = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(redPin, OUTPUT);   // sets the pins as output

  // Autorise l'allocation de tous les timers PWM disponibles
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  monServo.setPeriodHertz(50);       // fréquence standard des servos 50Hz
  monServo.attach(servoPin, 500, 2400); // min/max pulse width en µs (à ajuster si besoin)

  Serial.println("Servo initialisé");
}

float readPoten() {
  const float pinValue = analogRead(potPin);
  const float pourcentage = (pinValue / 26) / 100;
  float deg = pourcentage * 180 - 22;

  if (deg >= 180) deg = 180;

  return deg;   // read the potentiometer value at the input pin
}

// Main program
void loop()
{
  // Balayage de 0° à 180°
  float maxDeg = 90;
  float time = 1000;
  int i = readPoten();
  for (; i < time; i++) {
    Serial.println(readPoten());
    monServo.write(i * (maxDeg/time));
    delay(1);
  }

  delay(500);

  // Retour de 180° à 0°
  for (int i = 0; i < time; i++) {
    Serial.println(readPoten());
    monServo.write(maxDeg - (i * (maxDeg/time)));
    delay(1);
  }

  delay(500);
}
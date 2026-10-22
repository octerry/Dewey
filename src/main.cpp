#include <Arduino.h>
#include <ESP32Servo.h>

// Broche connectée au fil signal (orange/jaune) du SG90
// Choisis une broche PWM valide de la Xiao ESP32C6, par ex. D2 (GPIO2)
const int servoPinX= D1;
const int servoPinY= D2;
const float APP_PI = 3.14159265;

Servo monServoX;
Servo monServoY;

float currentPosX = 0;
float currentMoveX[4] = {-1}; // [startPosition, endPosition, step, time]
float currentPosY = 0;
float currentMoveY[4] = {-1};

void setup() {
  Serial.begin(9600);

  // Autorise l'allocation de tous les timers PWM disponibles
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  monServoX.setPeriodHertz(50);       // fréquence standard des servos 50Hz
  monServoX.attach(servoPinX, 500, 2400); // min/max pulse width en µs (à ajuster si besoin)
  monServoY.setPeriodHertz(50);       // fréquence standard des servos 50Hz
  monServoY.attach(servoPinY, 500, 2400); // min/max pulse width en µs (à ajuster si besoin)

  setMoveX(180, 2000);
}

void loop() {
  // Balayage de 0° à 180°
  float maxDegX = 180;
  float maxDegY = 90;
  int time = 1000;

  if (currentMoveX[0] < 0) {
    if (currentPosX == 180) {
      setMoveX(0, 2000);
    } else {
      setMoveX(180, 2000);
    }
  }

  Serial.print(currentMoveX[0]);
  Serial.print(" ");
  Serial.print(currentMoveX[1]);
  Serial.print(" ");
  Serial.print(currentMoveX[2]);
  Serial.print(" ");
  Serial.print(currentMoveX[3]);
  Serial.print("\n");
  
  updateServo();
  delay(1);
}

void setMoveX(float endPos, int time) {
  int startPos = currentPosX;
  int step = 0;
  currentMoveX[0] = startPos;
  currentMoveX[1] = endPos;
  currentMoveX[2] = step;
  currentMoveX[3] = time;
}

void updateServo() {
  if (currentMoveX[0] >= 0) {
    currentMoveX[2] += 1; // On ajoute au compteur
    // float index = (-cos(float(step)/float(time) * APP_PI ) + 1 ) / 2; 
    // currentPosX = index * (endPos - startPos) + startPos;
    const float index = (-cos(float(currentMoveX[2])/float(currentMoveX[3]) * APP_PI ) + 1 ) / 2; // On calcule l'étape
    currentPosX = index * (currentMoveX[1] - currentMoveX[0]) + currentMoveX[0]; // On applique l'étape
    Serial.print(" ");
    monServoX.write(currentPosX);

    if (currentMoveX[2] >= currentMoveX[3]) currentMoveX[0] = -1; // Reset quand c'est fini
  }
}

// void goToX(float endPos, int time) {
//   int startPos = currentPosX;

//   for (int i = 0; i < time; i++) {
//     float index = (-cos(float(i)/float(time) * APP_PI ) + 1 ) / 2; // -cos(2xPI)+1 / 2 (Visualise sur Geogebra si tu vois pas)
//     currentPosX = index * (endPos - startPos) + startPos;
//     monServoX.write(currentPosX);
//     delay(1);
//   }
// }

// void goToY(float endPos, int time) {
//   int startPos = currentPosY;

//   for (int i = 0; i < time; i++) {
//     float index = (-cos(float(i)/float(time) * APP_PI ) + 1 ) / 2; // -cos(2xPI)+1 / 2 (Visualise sur Geogebra si tu vois pas)
//     currentPosY = index * (endPos - startPos) + startPos;
//     monServoY.write(currentPosY);
//     delay(1);
//   }
// }

// void goTo(float endPosX, float endPosY, int time) {
//   int startPosX = currentPosX;
//   int startPosY = currentPosY;

//   for (int i = 0; i < time; i++) {
//     float index = (-cos(float(i)/float(time) * APP_PI ) + 1 ) / 2; // -cos(2xPI)+1 / 2 (Visualise sur Geogebra si tu vois pas)
//     currentPosX = index * (endPosX - startPosX) + startPosX;
//     currentPosY = index * (endPosY - startPosY) + startPosY;
//     monServoX.write(currentPosX);
//     monServoY.write(currentPosY);
//     delay(1);
//   }
// }
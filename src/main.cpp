#include <Arduino.h>
#include <ESP32Servo.h>

const int yawServoPin = D1;
const int pitchServoPin = D2;

const int minYawDeg = 45;
const int maxYawDeg = 135;
const int minPitchDeg = 30;
const int maxPitchDeg = 90;

Servo yawServo;
Servo pitchServo;

// SSD1306 ECRAN OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float xProgress = 0.5;
int eyesXPosition = 0;
int positionDelay = 0;
int blinkDelay = 0;
bool blinkFace = false;
bool goingLeft = false;
bool positionStopped = false;
bool needToStop = false;
int waitingTime = 0;

int pitchPosition = 50;
int yawPosition = 90;

void drawFace() {
  display.clearDisplay();

  // Yeux
  if (blinkFace) {
    display.drawLine(eyesXPosition - 14, 32, eyesXPosition + 14, 32, SSD1306_WHITE);
    display.drawLine(eyesXPosition + 60 - 14, 32, eyesXPosition + 60 + 14, 32, SSD1306_WHITE);
  } else {
    display.fillCircle(eyesXPosition + 60, 32, 14, SSD1306_WHITE);
    display.fillCircle(eyesXPosition, 32, 14, SSD1306_WHITE);
  }

  // Bouche
  display.fillCircle(eyesXPosition + 30, 50, 7, SSD1306_WHITE);
  display.fillRect(eyesXPosition + 30 - 7, 50 - 7, 15, 7, SSD1306_BLACK);

  display.display();
}

float easeInOut(float n, float force) {
  // Prends un nombre n entre 0 et 1 en entrée et lui applique un effet ease in out cubic
  float finalN = 0;

  if (n < 0.5) { // f(x) = -(-x + 0.5) ** (1/n) * 3 / 5 + 0.5
    finalN = (-n + 0.5); // [(-x + 0.5)]
    finalN = - pow(finalN, 1.0/force); // Racine cube [** (1/n)]
    finalN *= 3; // [* 3]
    finalN /= 5; // [/ 5]
    finalN += .5; // [+ 0.5]
  }
  else { // f(x) = (x - 0.5) ** (1/3) * 3 / 5 + 0.5
    finalN = n - 0.5; // [(x - 0.5)]
    finalN = pow(finalN, 1.0/force); // Racine cube [** (1/n)]
    finalN *= 3; // [* 3]
    finalN /= 5; // [/ 5]
    finalN += .5; // [+ 0.5]
  }

  return finalN;

  // le code qui suit fait l'inverse (je l'ai trouvé sans faire exprès)
  // // f(x) = (x - 0.5) ** 3 * 4 + 0.5
  // float finalN = n - 0.5;
  // Serial.print("=> ");
  // Serial.print(finalN);
  // finalN = pow(finalN, 3);
  // Serial.print("=> ");
  // Serial.print(finalN);
  // finalN = finalN * 4;
  // Serial.print("=> ");
  // Serial.print(finalN);
  // finalN += 0.5;
  // Serial.print("=> ");
  // Serial.print(finalN);
  // return finalN;
}

void updateServo() {
  yawPosition = xProgress * (maxYawDeg - minYawDeg) + minYawDeg;
  pitchPosition = 0;

  yawServo.write(yawPosition);
  pitchServo.write(pitchPosition);
};

void updatePosition() {
  if (!positionStopped) {
    // Update progress
    if (goingLeft) {
      if (xProgress > 0) {
        xProgress -= 0.02;
      } else {
        xProgress = 0;
        goingLeft = false;
      }
    } else {
      if (xProgress < 1) {
        xProgress += 0.02;
      } else {
        xProgress = 1;
        goingLeft = true;
        needToStop = true;
      }
    }

    // Linear to ease in out
    float finalxProgress = easeInOut(xProgress, 2);

    eyesXPosition = finalxProgress * 40 + 14;

    if (xProgress == 0.5 && !goingLeft) {
      positionDelay = 160;
      positionStopped = true;
    }
  } else {
    positionDelay--;
    if (positionDelay <= 0) {
      positionStopped = false;
    }
  }
}

void handlePause() {    // Update progress
  if (goingLeft) {
    if (xProgress > 0) {
      if (xProgress <= 0.5 && needToStop) { // S'arrete pendant 3 secondes
        waitingTime = 3000;
        needToStop = false;
        xProgress = 0.5;
        updateServo();
        drawFace();
      }
    }
  }
}

void handleBlink() {
  if (blinkDelay >= 300) {
    blinkFace = true;
    blinkDelay = 0;
    drawFace();
  } else {
    blinkDelay++;
    if (blinkDelay > 3) blinkFace = false;
    if (blinkDelay == 4) drawFace();
  }
}

void setup() {
  Serial.begin(115200);

  // SERVO
  // Autorise l'allocation de tous les timers PWM disponibles
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  yawServo.setPeriodHertz(50);       // fréquence standard des servos 50Hz
  yawServo.attach(yawServoPin, 500, 2400); // min/max pulse width en µs (à ajuster si besoin)
  pitchServo.setPeriodHertz(50);       // fréquence standard des servos 50Hz
  pitchServo.attach(pitchServoPin, 500, 2400); // min/max pulse width en µs (à ajuster si besoin)

  Serial.println("Servo initialisé");

  // SSD1306 ECRAN OLED
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Erreur : ecran SSD1306 non detecte");
  }
}

void loop() {
  if (waitingTime <= 0) {
    updatePosition();
    updateServo();
    handlePause();
    drawFace();
    handleBlink();
  } else {
    delay(20);
    waitingTime -= 20;
    handleBlink();
  }
}

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

// UART matériel de la XIAO ESP32C6
#define DF_RX D7   // GPIO17, reçoit depuis le TX du DFPlayer
#define DF_TX D6   // GPIO16, envoie vers le RX du DFPlayer

int playId = 1;

DFRobotDFPlayerMini player;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, DF_RX, DF_TX);

  Serial.println("Initialisation du DFPlayer...");
  delay(1000); // le module a besoin d'un peu de temps au démarrage

  if (!player.begin(Serial1)) {
    Serial.println("Échec : vérifie le câblage et la carte SD.");
    while (true) delay(100);
  }

  Serial.println("DFPlayer OK");
  player.volume(15);       // 0 à 30
  player.play(1); // joue /MP3/0001.mp3
}

void loop() {
  // Messages d'état du module (fin de piste, erreurs...)
  if (player.available()) {
    uint8_t type = player.readType();
    int value = player.read();
    if (type == DFPlayerPlayFinished) {
      // if (playId >= 7) playId = 0;
      // playId += 1;
      player.volume(30);       // 0 à 30
      player.loop(7);
    } else if (type == DFPlayerError) {
      Serial.print("Erreur DFPlayer : ");
      Serial.println(value);
    }
  }
}
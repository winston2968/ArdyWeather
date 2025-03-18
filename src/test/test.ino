#include <VirtualWire.h>

int RF_RX_PIN = 4;



void setup() {
  Serial.begin(9600);
  vw_set_rx_pin(RF_RX_PIN);  // initialisation de la broche de reception
  vw_setup(2000); // choix de la vitesse de transmission
  vw_rx_start(); //démarrage du récepteur
  Serial.println("ok");
}

void loop() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) {
    int i;
    // si on a reçu un message
    Serial.println("coucou");
    Serial.print("Reçu : ");
    for (i = 0; i < buflen; i++) {
      Serial.print(buf[i], HEX);
      Serial.print(" ");
    }
    Serial.println("");

    // Décodage du message 
    Serial.print("Protocole : ") ;
    Serial.println(char(buf[0]));
    Serial.print("Emetteur : ");
    Serial.println(char(buf[1]));
    Serial.print("Destinataire : ");
    Serial.println(char(buf[2]));
    for (int i = 0; i < 25 ; i++) {
      Serial.print(char(buf[i]));
    }
    Serial.println("");
  }

}
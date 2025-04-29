// ===========================================================================
//                      Station Module Code 
// ===========================================================================

// ------------ Import and define pin ------------

#include <TimerThree.h>
#include <VirtualWire.h>

int RF_RX_PIN = 4;  // Reception pin

// ------------ Functions ------------


// ------------ Setup ------------

void setup(){
  Serial.begin(9600);
  vw_set_rx_pin(RF_RX_PIN);   // Init Reception Broch
  vw_setup(2000);             // Transmission Speed
  vw_rx_start();              // Starting Receiving
}


// ------------ Loop ------------

void loop() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) {
    int i;
    // si on a reçu un message
    Serial.print("Reçu : ");
    for (i = 0; i < buflen; i++) {
      Serial.print(buf[i], HEX);
      Serial.print(" ");
    }
    Serial.println("");

    // Décodage du message 
    // Serial.print("Protocole : ") ;
    // Serial.println(char(buf[0]));
    // Serial.print("Emetteur : ");
    // Serial.println(char(buf[1]));
    // Serial.print("Destinataire : ");
    // Serial.println(char(buf[2]));
    for (int i = 0; i < 25 ; i++) {
      Serial.print(char(buf[i]));
    }
    Serial.println("coucou");

  }
}

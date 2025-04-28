// ===========================================================================
//                      Sensor Module Code
// ===========================================================================

/*
  Communication Protocol
*/

// ==========================================================================
// Include Libraries 

#include "Arduino.h"
#include "Wire.h"

#include <TimerFour.h>
#include <VirtualWire.h>

int RF_TX_PIN = 4;  // Sending pin 

// ------------ Global Variables ------------

// Radio Protocol variables 
static char SENSOR_NUM = '1';

int nb_ack = 0;
int nb_sent = 0;

char datagram_type[] = {'A', 'W', '0', SENSOR_NUM, '0', '0', '0'}; 

char datas_example[] = {'H','G','F','D','J','I','Z','J','C','K','J','B','Q','K','B','Z','S','B','V'};

// ------------ Radio Protocol ------------

/*
  Datagram Structure : 
    {Protocol, Target, Emmeteur, Type, Nb Send, Nb ACK, Datas}
  Example : 
    {AW, 2, 1, Datas, 10, 7, 6585689459076879544567980}
  
  - Protocol : Always AW, the name of the protocol. 
  - Target : the number of target station. 
  - Emmeteur : the number of actual station. 
  - Type : 'A' or 'D' (acquitement or datas)
  - Nb Send : Number of sent datagram. 
  - Nb ACK : Number of acquitted datagram. 
  - Datas : Datas if it's a Datas type datagram. 
*/

// Send datagram function 
void send_data(char* msg) {
  Serial.println("");
  Serial.println("Sending message...");
  for (int i = 0; i < 27; i++) {  // Affiche tout le message
    Serial.print(msg[i]);
  }
  Serial.println("");
  vw_send((uint8_t *)msg, 27); // Envoie 27 octets
  vw_wait_tx();
  Serial.println("Sent !");
}

// Build datagram message 
char* build_datagram(char datas[], bool send_to_station) {
  nb_sent += 1; 

  // Initialiser le datagramme de 27 octets
  char* datagram = (char*) malloc(27);  
  memset(datagram, 0, 27);  // Initialiser avec des zéros

  // Remplir les informations du header
  datagram[0] = datagram_type[0];  // Protocole
  datagram[1] = datagram_type[1];
  datagram[2] = '0';               // Station cible
  datagram[4] = 'D';               // Type de datagramme
  datagram[5] = '0' + nb_sent;
  datagram[6] = '0' + nb_ack;

  // Remplir les données dans le datagramme (à partir du 7ème octet)
  for (int i = 7; i < 27; i++) {
    datagram[i] = datas[i - 7];
  }

  return datagram;
}

void setup() {
  Serial.begin(9600);

  vw_set_tx_pin(RF_TX_PIN);
  vw_setup(2000); // 2000 bits par seconde
}

void loop() {
  // Obtenir le datagramme
  char* datagram = build_datagram(datas_example, true);

  // Afficher le datagramme créé
  Serial.println("Datagram created : "); 
  for (int i = 0 ; i < 27 ; i++) {
    Serial.print(datagram[i]); 
  }
  Serial.println("");

  // Envoyer le datagramme via le signal radio
  send_data(datagram);
  Serial.println("");

  // Libérer la mémoire après l'envoi
  free(datagram);

  delay(2000); // Délai de 2 secondes
}

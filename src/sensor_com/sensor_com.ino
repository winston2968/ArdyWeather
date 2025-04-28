#include "Arduino.h"
#include "Wire.h"
#include <TimerFour.h>
#include <VirtualWire.h>

int RF_TX_PIN = 4;  // Sending pin 

// Variables globales
static char SENSOR_NUM = '1';
volatile int nb_ack = 0;
volatile int nb_sent = 0;
volatile bool doit_envoyer = false;  // <-- Un drapeau pour demander un envoi

char datagram_type[] = {'A', 'W', '0', SENSOR_NUM, '0', '0', '0'}; 
char datas_example[] = {'H','G','F','D','J','I','Z','J','C','K','J','B','Q','K','B','Z','S','B'};

char datagram[27]; // <- Un seul datagram statique

// Fonction pour construire le datagram
void build_datagram() {
  if (nb_sent > 30) nb_sent = 0;
  nb_sent += 1;

  memset(datagram, 0, 27);

  datagram[0] = datagram_type[0];
  datagram[1] = datagram_type[1];
  datagram[2] = '0'; // Target
  datagram[4] = 'D'; // Type
  datagram[5] = '0' + nb_sent;
  datagram[6] = '0' + nb_ack;

  for (int i = 7; i < 27; i++) {
    datagram[i] = datas_example[i - 7];
  }
}

// Fonction appelée par l'interruption
void on_timer() {
  doit_envoyer = true;
}

void setup() {
  Serial.begin(9600);
  vw_set_tx_pin(RF_TX_PIN);
  vw_setup(2000);

  Timer4.initialize(2000000); // 200 ms
  Timer4.attachInterrupt(on_timer);
}

void loop() {
  if (doit_envoyer) {
    doit_envoyer = false; // reset le drapeau

    build_datagram();

    Serial.println("Datagram created:");
    for (int i = 0; i < 27; i++) {
      Serial.print(datagram[i]);
    }
    Serial.println("");

    vw_send((uint8_t *)datagram, 27);
    vw_wait_tx();

    Serial.println("Sent!");
    Serial.print("nb_sent: ");
    Serial.println(nb_sent);

    free(datagram);

    Serial.println("");
  }
}

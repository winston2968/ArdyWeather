// ===========================================================================
//                      Station Module Code
// ===========================================================================

/*
  Communication Protocol
*/


#include "Arduino.h"
#include "Wire.h"
// #include <TH06_dev.h>
// #include "THSensor_base.h"

#include <TimerFour.h>
#include <VirtualWire.h>


// ==========================================================================
// Global Variables

// Radio Protocol variables 
static char STATION_NUM = '0';

int reception_pin = 2;  // Reception pin 
int sendding_pin = 4; // Sendding pin 

int nb_ack = 0;
int nb_sent = 0;

char datagram[] = {'A', 'W', '0', STATION_NUM, '0', '0', '0', '0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'}; 

// Station Variables

char storage[800];    // Datas storage 
int cursor = 0;       // To fill-in the storage table

// ==========================================================================
// Radio Protocol


/*
  Datagram Structure : 
    {Protocol, Target, Emmeteur, Type, Nb Send, Nb ACK, Datas}
  Example : 
    {A, W, 2, 1, Datas, 10, 7, 6585689459076879544567980}
  
  - Protocol : Always AW, the name of the protocol. 
  - Target : the number of target station. 
  - Emmeteur : the number of actual station. 
  - Type : 'A' or 'D' (acquitement or datas)
  - Nb Send : Number of sent datagram. 
  - Nb ACK : Number of acquitted datagram. 
  - Datas : Datas if it's a Datas type datagram. 
*/



// Datagram decoding function 
void datagram_decoding(char* received_msg) {
  // Get datagram infos 
  char radio_protocol_1 = received_msg[0]; 
  char radio_protocol_2 = received_msg[1]; 
  int msg_target = received_msg[2]; 
  char msg_emetor = received_msg[3]; 
  char datagram_type = received_msg[4]; 
  int nb_sent_msg = received_msg[5] - '0'; 
  int nb_ack_msg = received_msg[6] - '0'; 

  Serial.print("---| Datagram Type: "); 
  Serial.println(datagram_type); 
  Serial.print("---| Nb Sent: "); 
  Serial.println(nb_sent_msg); 
  Serial.print("---| Nb Ack : "); 
  Serial.println(nb_ack_msg); 

  if (datagram_type == 'A') {

  } else {
    // Send acquitment datagram
    nb_ack += 1 ; 
    send_acquitement(msg_emetor, nb_ack); 
    Serial.println("Message acquitted ! "); 

    // Save datas 
  
  }

}

void send_acquitement(char target, int nb_ack) {

  datagram[2] = target ; 
  datagram[4] = 'A'; 
  datagram[6] = '0' + nb_ack ;

  vw_send((uint8_t *)datagram, 27);
  vw_wait_tx();
}



// ==========================================================================
// Setup and Loop


void setup() {
  Serial.begin(9600);

  // Reception init
  Serial.println("Reception Ready ! ");
  vw_set_rx_pin(reception_pin);
  vw_setup(2000);
  vw_rx_start();

  // Sendding init 
  vw_set_tx_pin(sendding_pin);
  Serial.println("Sedding Ready ! "); 
}

void loop() {

  // Read received message 
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  if (vw_get_message(buf, &buflen)) {
    Serial.println("Message reçu !");
    
    Serial.print("---| Hex: ");
    char* received_msg = (char*) malloc(buflen);
    memset(received_msg, 0, buflen);

    /* 
    for (int i = 0; i < buflen; i++) {
      Serial.print(buf[i], HEX);
      Serial.print(" ");
    }
    Serial.println(""); */
    
    Serial.print("Texte: ");
    for (int i = 0; i < buflen; i++) {
      Serial.print((char)buf[i]);
      received_msg[i] = (char) buf[i]; 
    }
    Serial.println(""); 

    if (received_msg[2] == STATION_NUM) {
      datagram_decoding(received_msg); 
    } else {
      Serial.println("---| Datagram not for me..."); 
    }

    free(received_msg); 

    Serial.println("");
    
  } 
}



















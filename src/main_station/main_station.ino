// ===========================================================================
//                          Station Module Main
// ===========================================================================

#include "Arduino.h"
#include "Arduino.h"
#include "Wire.h"
#include <TimerFour.h>
#include <TimerThree.h>
#include <VirtualWire.h>


// ===========================================================================
// Global Variables 

// Radio Protocol Variables 
#define is_station true 
#define datagram_size 27 
#define datas_size 16

int sending_pin = 8; 
int reception_pin = 2; 
char STATION_NUM = '0' ; 
char DEST_NUM ; 
volatile int nb_ack = 0;
volatile int nb_send = 0; 
int nb_ack_other = 0; 
char datagram[datagram_size];

// Storage Variables 
#define STORAGE_SIZE 2000 
char temp_storage[STORAGE_SIZE]; 
char hum_storage[STORAGE_SIZE]; 
int storage_count = 0; 

// Prototypes 


// ===========================================================================
// Setup and Loop 

void setup() {
  Serial.begin(9600);

  // Reception init
  vw_set_rx_pin(reception_pin);
  vw_setup(2000);
  vw_rx_start();
  Serial.println("Reception Ready ! ");

  // Sendding init 
  vw_set_tx_pin(sending_pin);
  Serial.println("Sending Ready ! "); 
}


void loop() {
  // Listen for datagram reception
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  if (vw_get_message(buf, &buflen)) {
    Serial.println("------- Datagram Received -------"); 
    // Saving received datagram
    char* received_msg = (char*) malloc(buflen);
    memset(received_msg, 0, buflen);
    // Display received datagram
    Serial.print("---| Text: ");
    for (int i = 0; i < buflen; i++) {
      Serial.print((char)buf[i]);
      received_msg[i] = (char) buf[i]; 
    }
    Serial.println(); 
    // Decoding datagram and send acquitment 
    if (received_msg[0] == 'A' && received_msg[1] == 'W') {
      datagram_decoding(received_msg); 
    }
    // Free memory
    free(received_msg);



    Serial.println(); 
  } 
}
















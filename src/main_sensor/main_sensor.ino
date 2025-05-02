// ===========================================================================
//                          Sensor Module Main
// ===========================================================================

#include "Arduino.h"
#include "Arduino.h"
#include "Wire.h"
#include <TimerFour.h>
#include <TimerThree.h>
#include <VirtualWire.h>


// ===========================================================================
// Global Variables 

// Communication Variables 
#define is_station false 
#define datagram_size 27 
int sending_pin = 8; 
int reception_pin = 2; 
char MODULE_NUM = '1' ; 
char DEST_NUM = '0' ;   // Corresponding to station in this project 
volatile int nb_ack = 0;
volatile int nb_send = 0; 
int nb_ack_other = 0; 
char datagram[datagram_size]; 
char last_datagram[datagram_size]; 
volatile bool send_datagram_flag = false ; 

// 54 values × 3 char ASCII = 162, for 2 tables = 324
char merged_table[324];

// Sendding STACK variables 
int nb_tables_stack = 0; 

// Weather station variables 
float temp_table[3][18] = {0};       // To store temperatures before sendding 
float hum_table[3][18] = {0};        // To store humidity values before sendding 
int temp_hum_cpt = 0;                // To know if we need to send 
volatile bool temp_hum_flag = false; 

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

  // Sendding Pin
  vw_set_tx_pin(sending_pin);
  vw_setup(2000);
  Serial.println("Sending Ready ! "); 


  // Hum and Temp Timer 
  Timer3.initialize(1000000);
  Timer3.attachInterrupt(set_update_temp_hum_flag);  
  Serial.println("Starting Getting Weather Values !"); 

  // Send datas timer 
  Timer4.initialize(2000000); 
  Timer4.attachInterrupt(set_send_datagram_flag); 

  Serial.println(); 
}

void loop() {

  // Timers depending functions 
  if (temp_hum_flag == true) {
    update_temp_hum();
  }
  
  // Check if we need to send a datagram 
  if (send_datagram_flag && nb_tables_stack != 0) {
    send_datagram(); 
    Serial.println(); 
    send_datagram_flag = false; 
  }
  
  // Listen for datagram reception
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  if (vw_get_message(buf, &buflen)) {
    Serial.println("---| Datagram Received !");
    // Saving received datagram
    char* received_msg = (char*) malloc(buflen);
    memset(received_msg, 0, buflen);
    // Display received datagram
    Serial.print("---| Text: ");
    for (int i = 0; i < buflen; i++) {
      Serial.print((char)buf[i]);
      received_msg[i] = (char) buf[i]; 
    }
    Serial.println(""); 
    datagram_decoding(received_msg); 
    // Free memory
    free(received_msg);



    Serial.println(); 
  } 
  
}















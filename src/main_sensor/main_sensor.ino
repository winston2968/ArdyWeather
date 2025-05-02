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
int sendding_pin = 8; 
int reception_pin = 4; 
static char SENSOR_NUM = '2';
volatile int nb_ack = 0;
volatile int nb_ack_station = 0; 
volatile int nb_sent = 0;
char datas_example[] = {'H','G','F','D','J','I','Z','J','C','K','J','B','Q','K','B','Z','S','B'};
char datagram[27]; // <- One static datagram
bool is_sending = false;        // To suspend sending if not acquitted
char last_datagram[27]; 
int last_send_size = 0; 

// 54 values × 3 char ASCII = 162, for 2 tables = 324
char merged_table[324];
bool send_datagram_flag = false ; 

// Sendding STACK variables 
int nb_tables_stack = 0; 

// Weather station variables 
float temp_table[3][18] = {0};       // To store temperatures before sendding 
float hum_table[3][18] = {0};        // To store humidity values before sendding 
int temp_hum_cpt = 0;                // To know if we need to send 
bool temp_hum_flag = false; 

// Prototypes 
// void update_temp_hum(); 
// char* convert_temp_table_to_ASCII_table(float table[]); 
// void send_temp_hum(); 
// void build_datagram(); 
// void send_datagram(bool resend); 
// void datagram_decoding(char* received_msg); 



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
  vw_set_tx_pin(sendding_pin);
  vw_setup(2000);
  Serial.println("Sendding Ready ! "); 

  // Hum and Temp Timer 
  Timer3.initialize(100000);
  Timer3.attachInterrupt(set_update_temp_hum_flag);  
  Serial.println("Starting Getting Weather Values !"); 

  // Send datas timer 
  Timer4.initialize(2000000); 
  Timer4.attachInterrupt(set_send_datagram_flag); 


  // display_tables(); 

  Serial.println(); 
}

void loop() {

  // Timers depending functions 
  if (temp_hum_flag == true) {
    update_temp_hum();
  }
  
  if (send_datagram_flag == true && nb_tables_stack != 0) {
    send_datagram(); 
  }

  // Datagram reception 
  
}















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
int sendding_pin = 6; 
int reception_pin = 4; 
static char SENSOR_NUM = '2';
volatile int nb_ack = 0;
volatile int nb_ack_station = 0; 
volatile int nb_sent = 0;
char datas_example[] = {'H','G','F','D','J','I','Z','J','C','K','J','B','Q','K','B','Z','S','B'};
char datagram[27]; // <- One static datagram
bool is_sending = false;        // To suspend sending if not acquitted
char last_datagram[27]; 
float last_send_time = 0.0; 
float last_send_size = 0; 
bool waiting_ack = false ;

// Weather station variables 
float temp_table[3][19] = {0};       // To store temperatures before sendding 
float hum_table[3][19] = {0};        // To store humidity values before sendding 
int temp_hum_cpt = 0;                // To know if we need to send 

// Prototypes 
void update_temp_hum(); 
char* convert_temp_table_to_ASCII_table(float table[]); 
void send_temp_hum(); 
void buil_datagram(); 
void send_datagram(bool resend); 
void datagram_decoding(char* received_msg); 



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

  // Hum and temp init 
  // Timer3.initialize(1000000);
  // Timer3.attachInterrupt(update_temp_hum);  
  // Serial.println("Starting Getting Weather Values !"); 

  display_tables(); 
}

void loop() {

}















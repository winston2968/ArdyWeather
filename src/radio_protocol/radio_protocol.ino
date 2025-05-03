// =========================================================================
//                     Radio Protocol for ArdyWeather
// =========================================================================

/*
  Datagram Structure : 
    {Protocol, Destination, Emetor, Type, Datas Type, Nb Send, Nb ACK, Datas, ...}
  Example : 
    {A, W, 2, 1, D, T, 10, 7, 6585689459076879544567980}
  
  - Protocol : Always AW, the name of the protocol. 
  - Destination : the number of target station. 
  - Emetor : the number of actual station. 
  - Type : 'A' or 'D' (acquitement or datas)
  - Datas Type : 
      - 'T' for temperature 
      - 'H' for humidity
  - Nb Send : Number of sent datagram. 
  - Nb ACK : Number of acquitted datagram. 
  - Datas : Datas if it's a Datas type datagram. 
*/

#include "Wire.h"
#include <TimerFour.h>
#include <VirtualWire.h>


// =========================================================================
// Global Variables 

#define is_station true 
#define datagram_size 27 
int sending_pin = 8; 
int reception_pin = 2; 
char MODULE_NUM ; 
char DEST_NUM ; 
volatile int nb_ack = 0;
volatile int nb_send = 0; 
int nb_ack_other = 0; 
char datagram[datagram_size]; 
char last_datagram[datagram_size]; 
volatile bool send_datagram_flag = false ; 

// Test variables 
int STATION_NUM = '0'; 
int SENSOR_NUM = '1'; 

// =========================================================================
// Radio Protocol functions 

/*
  Build datagram depending on input infos. 
  Store datagram in the global variable char datagram[27]
*/
void build_datagram(char destination, char datagram_type, char datas_type) {
  // We will send next datagram 
  if (datagram_type == 'D') {
      nb_send += 1; 
  }
  // Define datagram header
  datagram[0] = 'A'; 
  datagram[1] = 'W'; 
  datagram[2] = destination; 
  datagram[3] = MODULE_NUM; 
  datagram[4] = datagram_type; 
  datagram[5] = datas_type; 
  datagram[6] = '0' + nb_send; 
  datagram[7] = '0' + nb_ack;  
  // Fill-in datas 
  // TODO : depends on module 
}

/*
  Send actual or last datagram depending if last datagram was acquited. 
*/
void send_datagram() {
  // Get/build datagram to send 
  if (nb_send == nb_ack_other) {
    // We can send a new datagram
    build_datagram(DEST_NUM, 'D', 'T'); 
    Serial.println("---| Sending a new datagram"); 
  } else {
    // We need to re-send last datagram
    memcpy(datagram, last_datagram, datagram_size); 
    Serial.println("Re-sending lest datagram"); 
  }
  // Send datagram 
  vw_send((uint8_t *)datagram, datagram_size);
  vw_wait_tx();
  // Keep datagram on buffer
  memcpy(last_datagram, datagram, datagram_size); 
  Serial.println("---| Datagram Send Succesfully !");
  Serial.print("Text: "); 
  for (int i = 0; i < datagram_size; i++) {
    Serial.print(datagram[i]); 
  } 
  Serial.println(); 
}


/*
  Decode received datagram for user interface. 
*/
void datagram_decoding(char* received_msg) {
  // Get datagram_infos 
  char msg_dest = received_msg[2];
  char msg_emetor = received_msg[3]; 
  char datagram_type = received_msg[4]; 
  char datas_type = received_msg[5]; 
  char nb_send_msg = received_msg[6] - '0'; 
  char nb_ack_msg = received_msg[7] - '0'; 
  // Display values 
  Serial.print("Destination: "); 
  Serial.println(msg_dest); 
  Serial.print("Emetor: "); 
  Serial.println(msg_emetor); 
  Serial.print("Datagram Type: "); 
  Serial.println(datagram_type); 
  Serial.print("Datas Type: "); 
  Serial.println(datas_type); 
  Serial.print("Sender send number: "); 
  Serial.println(nb_send_msg); 
  Serial.print("Sender ack number: "); 
  Serial.println(nb_ack_msg); 
  // Acquitement generation 
  if (datagram_type == 'A') {
    // It's an acquitement datagram, nothing to do
    nb_ack_other = nb_send_msg; 
  } else {
    // We need to acquit the received datagram
    nb_ack = nb_send_msg; 
    send_acquitment(); 
  }
  // Process datas
  // TODO : process datas depending on module. 
}

/*
  Send acquitment to nb_ack variable to the other MODULE. 
*/
void send_acquitment() {
  build_datagram(DEST_NUM, 'A', '0'); 
  // Send datagram 
  vw_send((uint8_t *)datagram, datagram_size);
  vw_wait_tx();
  // Log infos 
  Serial.println("---| Acquitment datagram send !");
  for (int i = 0; i < datagram_size; i++) {
    Serial.print(datagram[i]); 
  } 
  Serial.println();
}

void set_send_datagram_flag() {
  send_datagram_flag = true; 
}

// =========================================================================
// Setup and loop

void setup() {
  // Testing variables 
  if (is_station) {
    MODULE_NUM = STATION_NUM; 
    DEST_NUM = SENSOR_NUM; 
  } else {
    MODULE_NUM = SENSOR_NUM; 
    DEST_NUM = STATION_NUM; 
  }

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



}

void loop() {
  // Check if we need to send a datagram 
  if (send_datagram_flag) {
    send_datagram(); 
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
    // Free memory
    free(received_msg);



    Serial.println(); 
  } 
}




















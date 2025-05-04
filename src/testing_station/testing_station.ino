// ===========================================================================
//                     Station Multi-Com Testing
// ===========================================================================


/*
  Testing code for station multi-module reception feature. 
*/

/*
  Datagram Structure : 
    {Protocol, Destination, Emetor, Type, Datas Type, Nb Send, Nb ACK, CKSM1, CKSM2 Datas, ...}
  Example : 
    {A, W, 2, 1, D, T, 10, 7, CKSM1, CKSM2, 6585689459076879544567980}
  
  - Protocol : Always AW, the name of the protocol. 
  - Destination : the number of target station. 
  - Emetor : the number of actual station. 
  - Type : 'A' or 'D' (acquitement or datas)
  - Datas Type : 
      - 'T' for temperature 
      - 'H' for humidity
  - Nb Send : Number of sent datagram. 
  - Nb ACK : Number of acquitted datagram. 
  - CKSM1 and CKSM2 : ckecksum values from Fletcher-16 algorithm. 
  - Datas : Datas if it's a Datas type datagram. 
*/

#include "Arduino.h"
#include "VirtualWire.h"
#include "TimerFour.h"

// ===========================================================================
// Global Variables 

#define datagram_size 27
#define datas_size 16
#define nb_stations 5 // /!\ Under or equal to 9 !!!!! 

int sending_pin = 8; 
int reception_pin = 2; 
int module_num = 0;                               // Selected with random() when build_datagram
char DESTINATION = '0'; 
int nb_ack[nb_stations] = {0};                    // ACK and Send numbers differents for each module
int nb_send[nb_stations] = {0}; 
int nb_ack_other[nb_stations] = {0}; 
char datagram[datagram_size] = {0}; 
char last_datagram[datagram_size] = {0}; 
volatile bool send_datagram_flag = false ; 

// Testing values 
char test_datas[] = {'A','G','H','J','D','K','N','S','K','N','C','P','L','N','H','J'}; 


// ===========================================================================
// Radio functions

/*
  Calculating checksum function for datagram sending and reception. 
*/
uint16_t get_checksum() {
  uint16_t sum = 0; 
  // Calculate checksum without checksum numbers in datagram
  datagram[8] = '0'; 
  datagram[9] = '0'; 
  for (int i = 0; i < datagram_size; i++) {
    sum += (uint8_t) datagram[i]; 
  }
  uint16_t result = (uint16_t) (sum & 0xFFFF);
  return result ;  
}

/*
  Build datagram function depending on module_num 
  randomly generated. 
*/
void build_datagram(char datagram_type, char datas_type) {
  // Define module num
  module_num = random(0, nb_stations); 
  Serial.print("---| Module Num : "); 
  Serial.println(module_num + 1); 
  // Re-init datagram values
  memset(datagram, 0, sizeof(datagram)); 
  // We send a new data datagram
  if (datagram_type == 'D') {
    nb_send[module_num] += 1; 
    for (int i = 0; i < datas_size; i++) {
      datagram[i + 10] = test_datas[i]; 
    }
  }
  // Fill-in datagram header
  datagram[0] = 'A'; 
  datagram[1] = 'W'; 
  datagram[2] = DESTINATION; 
  datagram[3] = (module_num + 1) + '0'; 
  datagram[4] = datagram_type ; 
  datagram[5] = datas_type; 
  datagram[6] = nb_send[module_num] + '0'; 
  datagram[7] = nb_ack[module_num] + '0'; 
  // Calculate checksum
  uint16_t checksum = get_checksum(); 
  datagram[8] = (checksum >> 8) & 0xFF ;
  datagram[9] = checksum & 0xFF ;  
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
  int nb_send_msg = received_msg[6] - '0'; 
  int nb_ack_msg = received_msg[7] - '0'; 
  uint16_t received_checksum = (received_msg[8] << 8) | received_msg[9];
  module_num = (msg_dest - '0') - 1; 
  // Display values 
  Serial.print("---| Destination: "); 
  Serial.println(module_num); 
  Serial.print("---| Emetor: "); 
  Serial.println(msg_emetor); 
  Serial.print("---| Datagram Type: "); 
  Serial.println(datagram_type); 
  Serial.print("---| Datas Type: "); 
  Serial.println(datas_type); 
  Serial.print("---| Sender send number: "); 
  Serial.println(nb_send_msg); 
  Serial.print("---| Sender ack number: "); 
  Serial.println(nb_ack_msg); 
  // Acquitement generation 
  if (datagram_type == 'A') {
    // It's an acquitement datagram, nothing to do
    nb_ack_other[module_num] = nb_ack_msg; 
  } 
  Serial.print("---| Nb Ack Other : "); 
  Serial.println(nb_ack_other[module_num]); 
  Serial.print("---| Nb Send : "); 
  Serial.println(nb_send[module_num]); 
}


/*
  Send datagram function 
*/
void send_datagram() {
  Serial.println("------- Sending Datagram Process Lauched -------"); 
  if (nb_send[module_num] == nb_ack_other[module_num]) {
    // We can send a new datagram 
    build_datagram('D', 'T'); 
  } else {
    Serial.print("---| Module Num : "); 
    Serial.println(module_num + 1); 
    Serial.print("---| Nb Send Module Num : "); 
    Serial.println(nb_send[module_num]); 
    Serial.print("---| Nb ACK Module NUM : "); 
    Serial.println(nb_ack_other[module_num]); 
    // We need to resend last_datagram
    memcpy(datagram, last_datagram, datagram_size); 
    Serial.println("*** Re-sending last datagram ***"); 
  }
  // Sending datagram 
  vw_send((uint8_t *)datagram, datagram_size);
  vw_wait_tx();
  // Keep datagram on buffer 
  memcpy(last_datagram, datagram, datagram_size); 
  Serial.println("*** Datagram Send Succesfully ! ***");
  Serial.print("Text: "); 
  for (int i = 0; i < datagram_size; i++) {
    Serial.print(datagram[i]); 
  } 
  Serial.println(); 
  Serial.println(); 
}


void set_send_datagram_flag() {
  send_datagram_flag = true ; 
}

// ===========================================================================
// Setup and loop 


void setup() {
  Serial.begin(9600); 
  // Init radio 
  vw_set_rx_pin(reception_pin);
  vw_setup(2000);
  vw_rx_start();
  Serial.println("Reception Ready ! ");
  vw_set_tx_pin(sending_pin);
  vw_setup(2000);
  Serial.println("Sending Ready ! "); 
  // Init sendind timer 
  Timer4.initialize(4000000); 
  Timer4.attachInterrupt(set_send_datagram_flag); 

  Serial.println(); 
}


void loop() {

  // Verify if we need to send 
  if (send_datagram_flag) {
    send_datagram(); 
    send_datagram_flag = false;
  }

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
    Serial.println(""); 
    if (received_msg[0] == 'A' && received_msg[1] == 'W') {
      datagram_decoding(received_msg); 
    }
    // Free memory
    free(received_msg);



    Serial.println(); 
  }
}




















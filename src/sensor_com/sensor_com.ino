// ===========================================================================
//                      Sensor Module Code
// ===========================================================================

/*
  Communication Protocol for ArdyWeather Sensor :
    - Send datagram to Station and wait for acquitment
*/

#include "Arduino.h"
#include "Wire.h"
#include <TimerFour.h>
#include <VirtualWire.h>

// ==========================================================================
// Global Variables

int sendding_pin = 4;   // Sending pin 
int reception_pin = 2;  // Reception pin 

// Communication Variables

static char SENSOR_NUM = '2';
volatile int nb_ack = 0;
volatile int nb_ack_station = 0; 
volatile int nb_sent = 0;

char datas_example[] = {'H','G','F','D','J','I','Z','J','C','K','J','B','Q','K','B','Z','S','B'};

char datagram[27]; // <- One static datagram

bool is_sending = false;        // To suspend sending if not acquitted

// Delay to re-send datagram variables 

unsigned long last_send_time = 0;   // Last send time 
unsigned long timeout = 800;   // Delay before re-send 
bool message_sent = false;          // Flag to check if datagram was sent
bool waiting_ack = false;          // Flag to check if datagram was acquitted 
char last_datagram[27];
int last_sent_size = 0; 

// Weather Station variables 

float temp_table[19]; 
float hum_table[19]; 

int temp_hum_cpt = 0; 



// ==========================================================================
// Weather station functions

void update_temp_hum() {
  float actual_temp = (float) random(0, 40); 
  float actual_hum = (float) random(0, 100); 
  
  if (temp_hum_cpt == 18) {
    send_temp_hum(); 
    temp_hum_cpt = 0; 
  } else {
    temp_table[temp_hum_cpt] = actual_temp ; 
    hum_table[temp_hum_cpt] = actual_hum; 
  }
}

void send_temp_hum() {
  // TODO : fill the function 
  // Add a flag to know when we have to send 
}


/*
  Convert float temperature table to char table to send it throught radio. 
  It get a fixed 3 length float table. 
*/
char* convert_temp_table_to_ASCII_table(float temp_table[]) {

  // Calculate string table length
  int length = sizeof(temp_table) ; 
  // Create table 
  char* char_table = (char*)malloc(length * 3 + 1);  // +1 for the null terminator
  Serial.println("-----");
  Serial.println(length);

  // Convert float values
  for (int i = 0; i < length ; i++) {
    int value = temp_table[i] * 100;
    
    // Convert to char
    char c1 = (value / 91) + 33 ;
    char c2 = ((value % 91) / 10) + 33;
    char c3 = (value % 10) + 33 ;

    // Serial.println(c1);
    // Serial.println(c2);
    // Serial.println(c3);

    // Store the values in char_table
    char_table[i * 3] = c1;
    char_table[i * 3 + 1] = c2;
    char_table[i * 3 + 2] = c3;
    
  }
  char_table[length * 3] = '\0'; // Null-terminate the string

  return char_table ;
}

// ==========================================================================
// Radio Protocol

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

// Function to build datagram 
void build_datagram() {
  if (nb_sent == 30) {
    nb_sent = 0;
  }
  nb_sent += 1;

  memset(datagram, 0, 27);

  datagram[0] = 'A';
  datagram[1] = 'W';
  datagram[2] = '1'; // Target
  datagram[3] = SENSOR_NUM; 
  datagram[4] = 'D'; // Type
  datagram[5] = '0' + nb_sent;
  datagram[6] = '0' + nb_ack;

  for (int i = 7; i < 27; i++) {
    datagram[i] = datas_example[i - 7];
  }
}



// Sendding function 
void send_datagram(bool resend = false) {

  if (!resend) {

    // We don't resend datagram 
    build_datagram(); 
    Serial.print("---| Datagram created: ");
    for (int i = 0; i < 27; i++) {
      Serial.print(datagram[i]);
    }
    Serial.println("");

  } else {

    // We need to resend last datagram
    memcpy(datagram, last_datagram, 27);  // Restaurer la dernière version envoyée
    Serial.println("---| Resending previous datagram:");
  }

  vw_send((uint8_t *)datagram, 27);
  vw_wait_tx();

  // Keep datagram on buffer
  last_send_time = millis(); 
  memcpy(last_datagram, datagram, sizeof(datagram));
  last_sent_size = sizeof(datagram);

  Serial.print("---| nb_sent: ");
  Serial.println(nb_sent);
  Serial.println("===> Sent!");
  Serial.println("");

  waiting_ack = true ; 
}


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
    Serial.print("Acquitement Datagram for: "); 
    Serial.println(nb_ack_msg); 
    nb_ack_station = nb_ack_msg ;       // Update station received datagrams
    Serial.println("---| Actual ack number: "); 
    Serial.println(nb_ack_station); 

    waiting_ack = false ; 
  } 
}



// ==========================================================================
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
  Serial.println("Sending Ready ! "); 
}

void loop() {

  // Sendding datagram 

  if (nb_ack_station == nb_sent) {

    send_datagram(false); // normal sending 
    last_send_time = millis();

  } else if (waiting_ack && millis() - last_send_time > timeout) {

    Serial.println("ACK not received in time, resending...");
    send_datagram(true);  // renvoi du datagramme précédent
    last_send_time = millis();  // redémarre le timer
  }

  // Acquitment Reception

  // Read received message 
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  if (vw_get_message(buf, &buflen)) {
    Serial.println("==> Datagram Received !");
    
    char* received_msg = (char*) malloc(buflen);
    memset(received_msg, 0, buflen);
    
    Serial.print("---| Text: ");
    for (int i = 0; i < buflen; i++) {
      Serial.print((char)buf[i]);
      received_msg[i] = (char) buf[i]; 
    }

    Serial.println(""); 

    if (received_msg[2] == SENSOR_NUM) {
      datagram_decoding(received_msg); 
    } else {
      Serial.println("---| Datagram not for me..."); 
    } 

    free(received_msg); 

    Serial.println("");
    
  } 

}

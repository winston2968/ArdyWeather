// ===========================================================================
//                      Sensor Module Code
// ===========================================================================

/*
  Communication Protocol
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
volatile bool must_send = false;  // <-- Flag to ask for sending 

char datagram_type[] = {'A', 'W', '1', SENSOR_NUM, '0', '0', '0'}; 
char datas_example[] = {'H','G','F','D','J','I','Z','J','C','K','J','B','Q','K','B','Z','S','B'};

char datagram[27]; // <- One static datagram


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

  datagram[0] = datagram_type[0];
  datagram[1] = datagram_type[1];
  datagram[2] = '1'; // Target
  datagram[3] = SENSOR_NUM; 
  datagram[4] = 'D'; // Type
  datagram[5] = '0' + nb_sent;
  datagram[6] = '0' + nb_ack;

  for (int i = 7; i < 27; i++) {
    datagram[i] = datas_example[i - 7];
  }
}

// Interruption function
void on_timer() {
  must_send = true;
}

// Sendding function 
void send_datagram() {
  build_datagram();

  Serial.print("---| Datagram created: ");
  for (int i = 0; i < 27; i++) {
    Serial.print(datagram[i]);
  }
  Serial.println("");

  vw_send((uint8_t *)datagram, 27);
  vw_wait_tx();

  Serial.print("---| nb_sent: ");
  Serial.println(nb_sent);
  Serial.println("Sent!");

  Serial.println("");
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

  // Timer4.initialize(2000000); // 200 ms
  // Timer4.attachInterrupt(on_timer);
}

void loop() {

  // Sendding datagram 

  if (nb_ack_station == nb_sent) {
    // Station had received all datagrams 
    // we send the following 

    Serial.print("Nb Ack Station: "); 
    Serial.println(nb_ack_station); 
    Serial.print("Nb Sent: "); 
    Serial.println(nb_sent); 

    send_datagram();

    Serial.println("Sendding Datagram"); 
    must_send = false; // reset the flag
    send_datagram();
  } else {
    // Serial.print("Waiting for acquitment nb :"); 
    // Serial.println(nb_sent); 
  }

  // Acquitement Reception

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

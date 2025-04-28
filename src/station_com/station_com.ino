// ===========================================================================
//                      Station Module Code
// ===========================================================================

/*
  Communication Protocol
*/

// ==========================================================================
// Include Libraries 


#include "Arduino.h"
#include "Wire.h"
// #include <TH06_dev.h>
// #include "THSensor_base.h"

#include <TimerFour.h>
#include <VirtualWire.h>

int RF_TX_PIN = 4;  // Reception pin 


// ==========================================================================
// Global Variables

// Radio Protocol variables 
static char STATION_NUM = '0';

int nb_ack = 0;
int nb_sent = 0;

char datagram_type[] = {'A', 'W', '0', STATION_NUM, '0', '0', '0'}; 

// Sensor Variables


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

// Send datagram function 
void send_data(char* msg) {
  Serial.println("");
  Serial.println("Sending message...");
  for (int i = 0; i < 27; i++) {  // Display all message 
    Serial.print(msg[i]);
  }
  Serial.println("");
  vw_send((uint8_t *)msg, 27); // Send 27 octets
  vw_wait_tx();
  Serial.println("Sent !");
}

// Create datagram function 



// ==========================================================================
// Setup and Loop


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}




















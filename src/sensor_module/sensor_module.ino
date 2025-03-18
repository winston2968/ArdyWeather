// ===========================================================================
//                      Sensor Module Code
// ===========================================================================

/*

  Envoi des données (température et humidité) toutes les 10 secondes 
  à la station pour permettre un suivi régulier. 


*/


// ------------ Import and define pin ------------

#include <TimerFour.h>
#include <VirtualWire.h>

int RF_TX_PIN = 4;  // Sedding pin 

// Testing tables 
float temp_table[] = {17.00, 9.00, 11.01, 10.02, 11.09, 31.01, 24.23, 17.11, 18.45, 46.02}; 

// Testing message
char msg[] = "101 Coucou c'est moi" ;

// Max radio datagram length
int max_length = 10 ;

// ------------ Functions ------------

/*
  Convert float temperature table to char table to send it throught radio. 
  It get a fixed 3 length float table. 
*/
String convert_temp_table_to_ASCII_table(float temp_table[]) {

  // Calculate string table length
  int length = (sizeof(temp_table) / sizeof(temp_table[0])) ; 
  // Create table 
  char string_table[length*3]; 

  // Convert float values
  for (int i = 0; i < length ; i++) {
    int value = temp_table[i] * 100;
    
    // Convert to char
    char c1 = (value / 91) + 33 ;
    char c2 = ((value % 91) / 10) + 33;
    char c3 = (value % 10) + 33 ;

    char concatenate[] = {c1,c2,c3}; 
    string_table[i] = concatenate ;

    Serial.println(string_table);
  }
}


void send_data(void) {
  // Get info from sensors

  // Convert info into string values

  // Send infos to the station
  Serial.println("");
  Serial.println("--------- Envoi d'un message --------");
  Serial.println(3 + msg); 
  vw_send((uint8_t *)msg, 1 + strlen(msg));
  vw_wait_tx();
  Serial.println("envoyé");
}

// ------------ Setup ------------

void setup() {
  Serial.begin(9600);
  
  // Radio Init for sender
  Serial.begin(9600);
  vw_set_tx_pin(RF_TX_PIN);
  vw_setup(2000);

  // Init Timer 
  // Timer4.initialize(1000000);
  // Timer4.attachInterrupt(send_data);

  convert_temp_table_to_ASCII_table(temp_table);

}


// ------------ Loop ------------

void loop() {
  // send_data();
  // delay(3000);
}





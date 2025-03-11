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
// float temp_table = {17.00, 9.00, 11.01, 10.02, 11.09, 31.01, 24.23, 17.11, 18.45, 46.02}; 


char msg[] = "190 10,02,31,37,21,17,09";

char HEXA_TABLE[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

// ------------ Functions ------------

float* get_temp(int length) {
  float* temp = new float[length]; 
  for (int i = 0; i < length; i++) {
    temp[i] = random(-2, 40);
  }
  return temp;
}

String convert_to_HEX(int num) {
  return String(num, HEX); 
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

  // delete[] values;

  Serial.println(convert_to_HEX(590));
  Serial.println(convert_to_HEX(100));
}


// ------------ Loop ------------

void loop() {
  // send_data();
  // delay(3000);
}





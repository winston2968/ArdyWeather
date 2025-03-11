// ===========================================================================
//                      Sensor Module Code
// ===========================================================================


// ------------ Import and define pin ------------

#include <TimerFour.h>
#include <VirtualWire.h>

int RF_TX_PIN = 4;  // Sedding pin 

char msg[] = "190 10,02,31,37,21,17,09";

// ------------ Functions ------------

float* get_temp(int length) {
  float* temp = new float[length]; 
  for (int i = 0; i < length; i++) {
    temp[i] = random(-2, 40);
  }
  return temp;
}

String convert_temp_array(float* temp) {
  String temp_string = "";
  return temp_string ;
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
}


// ------------ Loop ------------

void loop() {
  send_data();
  delay(3000);
}





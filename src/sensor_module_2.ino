
#include <TimerFour.h>
#include <VirtualWire.h>

int RF_TX_PIN = 4;  // Sedding pin 


// ------------ Global Variables ------------

static char SENSOR_NUM = '1' ;

// Testing tables 
float float_table[] = {22.00, 8.00, 99.11, 17.02}; 

char debut_dataframe[] = "M61";

// ------------ Functions ------------

char* convert_table(float float_table[], int length) {
  static char char_table[50];
  char_table[0] = '\0';

  for (int i = 0 ; i < length ; i++) {
    int c1 = (int) float_table[i]; 
    int c2 = (int) ((float_table[i] - c1) * 100) ; 
    // Serial.println(c1);
    // Serial.println(c2);

    // char_table[i] = (char) c1; 
    // char_table[i+1] = (char) c2 ;

    // Ajouter les parties entières et décimales à la chaîne char_table
    char temp[20];  // Chaîne temporaire pour formater un nombre
    snprintf(temp, sizeof(temp), "%d.%02d ", c1, c2);  // Formater et ajouter à char_table
    strcat(char_table, temp);  // Concaténer le résultat à char_table
  }

  return char_table ;
}

void send_data(char msg[]) {
  // Send infos to the station
  Serial.println("");
  Serial.println("--------- Envoi d'un message --------");
  Serial.println(msg);
  vw_send((uint8_t *)msg, 1 + strlen(msg));
  vw_wait_tx();
  Serial.println("envoyé");
}


void setup() {
  Serial.begin(9600);
  
  // Radio Init for sender
  vw_set_tx_pin(RF_TX_PIN);
  vw_setup(2000);

  // Init Timer 
  // Timer4.initialize(1000000);
  // Timer4.attachInterrupt(send_data);

  
}

void loop() {

  int length = sizeof(float_table) / sizeof(float_table[0]);
  char* char_table = convert_table(float_table, length);

  char msg[250];

  strcpy(msg, debut_dataframe);
  strcat(msg, char_table);
  
  Serial.println(sizeof(msg));

  send_data(msg);
  delay(3000);

}

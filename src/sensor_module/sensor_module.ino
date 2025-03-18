// ===========================================================================
//                      Sensor Module Code
// ===========================================================================

/*

  Envoi des données (température et humidité) toutes les 10 secondes 
  à la station pour permettre un suivi régulier. 

  Problème : La conversion ne fonctionne pas bien, 
  toutes les valeurs de temp_table ne sont pas converties. 
*/

// ------------ Import and define pin ------------

#include <TimerFour.h>
#include <VirtualWire.h>

int RF_TX_PIN = 4;  // Sedding pin 


// ------------ Global Variables ------------

static char SENSOR_NUM = '1' ;

// Testing tables 
float temp_table[] = {17.00, 8.00, 11.01, 10.02, 11.09, 31.01, 24.23, 17.11, 18.45, 46.02}; 


// ------------ Functions ------------

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

// ------------ Radio Protocol ------------

/*
  Datagram Structure :
    {Protocol, Destinataire, Emeteur, Nb Reçues, Nb Send, Datas} 
*/

const int NB_DATAS = 40;

char datagram[NB_DATAS] = {'\0'};


void send_data(char msg[]) {
  // Send infos to the station
  Serial.println("");
  Serial.println("--------- Envoi d'un message --------");
  Serial.println(msg);
  vw_send((uint8_t *)msg, 1 + strlen(msg));
  vw_wait_tx();
  Serial.println("envoyé");
}

// ------------ Setup ------------

void setup() {
  Serial.begin(9600);
  
  // Radio Init for sender
  vw_set_tx_pin(RF_TX_PIN);
  vw_setup(2000);

  // Init Timer 
  // Timer4.initialize(1000000);
  // Timer4.attachInterrupt(send_data);




  datagram[0] = 'A';
  datagram[1] = '0';                // Control Station 
  datagram[2] = SENSOR_NUM;         // Sensor Station Number
  datagram[3] = '0';                // Num of sent datagrams
  datagram[4] = '0';                // Num of received datagrams

}


// ------------ Loop ------------

void loop() {
  char* datas_char = convert_temp_table_to_ASCII_table(temp_table);
  Serial.println(sizeof(datas_char));
  for (int i = 0 ; i < sizeof(datas_char) ; i++) {
    Serial.println(datas_char[i]);
  }
  Serial.println("");

  send_data(datas_char);
  free(datas_char);
  // Serial.println(sizeof(datagram) - 5);
  delay(3000);
}





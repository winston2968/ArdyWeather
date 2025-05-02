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
  datagram[3] = STATION_NUM; 
  datagram[4] = datagram_type; 
  datagram[5] = datas_type; 
  datagram[6] = '0' + nb_send; 
  datagram[7] = '0' + nb_ack;  
  // Fill-in datas 
  for (int i = 8; i < datagram_size; i++) {
    datagram[i] = '0'; 
  }
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
    nb_ack_other = nb_ack_msg; 
  } else {
    // We need to acquit the received datagram
    nb_ack = nb_send_msg; 
    send_acquitment(msg_emetor); 
  }
  // Process datas
  // TODO : process datas depending on module. 
}

/*
  Send acquitment to nb_ack variable to the other MODULE. 
*/
void send_acquitment(char destination) {
  build_datagram(destination, 'A', '0'); 
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




















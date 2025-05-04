// =========================================================================
//                     Radio Protocol for ArdyWeather
// =========================================================================

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
  // Re-init datagram values
  memset(datagram, 0, sizeof(datagram)); 
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
  for (int i = 10; i < datagram_size; i++) {
    datagram[i] = '0'; 
  }
  // Calculate checksum 
  uint32_t sum = 0; 
  for (int i = 0; i < datagram_size; i++) {
    sum += (uint8_t) datagram[i]; 
  }
  uint16_t result = (uint16_t)(sum & 0xFFFF) ; 
  datagram[8] = (result >> 8) & 0xFF;
  datagram[9] = result & 0xFF;
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
  // Display values 
  Serial.print("---| Destination: "); 
  Serial.println(msg_dest); 
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
    nb_ack_other = nb_ack_msg; 
  } else {
    // We verify checksum bits
    uint32_t sum = 0; 
    received_msg[8] = '0'; 
    received_msg[9] = '0'; 
    for (int i = 0; i < datagram_size; i++) {
      sum += (uint8_t) received_msg[i]; 
    }
    uint16_t result = (uint16_t)(sum & 0xFFFF) ; 
    if (received_checksum = result) {
      // We need to acquit the received datagram
      Serial.println("*** Checksum valid ! ***"); 
      nb_ack = nb_send_msg; 
      send_acquitment(msg_emetor); 
      // Process datas
    add_to_storage(received_msg, received_msg[5]);  
    } else {
      Serial.println("*** Checksum Invalid ! ***"); 
      Serial.print("---| Received : "); 
      Serial.println(received_checksum); 
      Serial.print("---| Calculated : "); 
      Serial.println(result); 
    }
  }
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
  Serial.println("------ Acquitment datagram send ! -------");
  Serial.print("Text : "); 
  for (int i = 0; i < datagram_size; i++) {
    Serial.print(datagram[i]); 
  } 
  Serial.println();
}




















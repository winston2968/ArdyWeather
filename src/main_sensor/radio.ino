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
  if (nb_send == 30) {
    nb_send = 0; 
  }

  // Re-init datagram values
  memset(datagram, 0, sizeof(datagram)); 
  // Define datagram header
  datagram[0] = 'A'; 
  datagram[1] = 'W'; 
  datagram[2] = destination; 
  datagram[3] = MODULE_NUM; 
  datagram[4] = datagram_type; 
  datagram[5] = datas_type; 
  datagram[6] = '0' + nb_send; 
  datagram[7] = '0' + nb_ack;  
  // Fill-in datas 
  Serial.print("---| Tables Stack : ");
  Serial.println(nb_tables_stack);
  for (int i = 0; i < 16; i++) {
    datagram[i + 10] = merged_table[i + (16 * (nb_tables_stack - 1))];
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
  Send actual or last datagram depending if last datagram was acquited. 
*/
void send_datagram() {
  Serial.println("------- Sending Datagram Process Lauched -------"); 
  // Get/build datagram to send 
  Serial.print("Nb Send: "); 
  Serial.println(nb_send); 
  Serial.println("Nb Ack Other: "); 
  Serial.println(nb_ack_other); 
  if (nb_send == nb_ack_other) {
    // We can send a new datagram
    if (nb_tables_stack < 24) {
       build_datagram(DEST_NUM, 'D', 'T'); 
    } else {
       build_datagram(DEST_NUM, 'D', 'H'); 
    }
    Serial.println("---| Sending a new datagram"); 
  } else {
    // We need to re-send last datagram
    memcpy(datagram, last_datagram, datagram_size); 
    Serial.println("Re-sending last datagram"); 
  }
  // Send datagram 
  vw_send((uint8_t *)datagram, datagram_size);
  vw_wait_tx();
  // Keep datagram on buffer
  memcpy(last_datagram, datagram, datagram_size); 
  Serial.println("---| Datagram Send Succesfully !");
  Serial.print("Text: "); 
  for (int i = 0; i < datagram_size; i++) {
    Serial.print(datagram[i]); 
  } 
  Serial.println(); 
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
    nb_tables_stack -= 1; 
    display_merged_table(); 
  } else {
    // We need to acquit the received datagram
    nb_ack = nb_send_msg; 
    send_acquitment(); 
  }
  // Process datas
  // TODO : process datas depending on module. 
}

/*
  Send acquitment to nb_ack variable to the other MODULE. 
*/
void send_acquitment() {
  build_datagram(DEST_NUM, 'A', '0'); 
  // Send datagram 
  vw_send((uint8_t *)datagram, datagram_size);
  vw_wait_tx();
  // Log infos 
  Serial.println("------ Acquitment datagram send ! ------");
  Serial.print("Text : "); 
  for (int i = 0; i < datagram_size; i++) {
    Serial.print(datagram[i]); 
  } 
  Serial.println();
}

void set_send_datagram_flag() {
  send_datagram_flag = true; 
}



















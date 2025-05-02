// ===========================================================================
//                              Radio Protocol 
// ===========================================================================

/*
  Datagram Structure : 
    {Protocol, Target, Emmeteur, Type, Nb Send, Nb ACK, Datas}
  Example : 
    {AW, 2, 1, Datas, 10, 7, 6585689459076879544567980}
  
  - Protocol : Always AW, the name of the protocol. 
  - Target : the number of target station. 
  - Emmeteur : the number of actual station. 
  - Type : 'A' or 'D' (acquitement or datas)
  - Datas Type : 
      'T' -> temperarure 
      'H' -> Humidity
  - Nb Send : Number of sent datagram. 
  - Nb ACK : Number of acquitted datagram. 
  - Datas : Datas if it's a Datas type datagram. 
*/

void set_send_datagram_flag() {
  send_datagram_flag = true ; 
}


// Function to build datagram 
void build_datagram() {
  // Do nothing if all datas ever sent 
  if (nb_tables_stack == 0) return; 
  // Reset datagram count if too big 
  if (nb_sent == 30) nb_sent = 0;

  // Put datagram datas to zero 
  memset(datagram, 0, 27);

  // Set datagram type depending to merged_table
  char datas_type; 
  if (nb_tables_stack > 3) {
    datas_type = 'H';         // For humidity values
  } else {
    datas_type = 'T';         // For temprature values 
  }

  // Setting datagram header
  datagram[0] = 'A';
  datagram[1] = 'W';
  datagram[2] = '1'; // Target
  datagram[3] = SENSOR_NUM; 
  datagram[4] = 'D'; // Type
  datagram[5] = datas_type; 
  datagram[6] = '0' + nb_sent;
  datagram[7] = '0' + nb_ack;

  // Copy 18 char to send from merged_table
  int offset = (18 - nb_tables_stack) * 18; // Start index in merged_table
  for (int i = 0; i < 18; i++) {
    datagram[8 + i] = merged_table[offset + i];
  }

  nb_sent += 1; 
  nb_tables_stack -= 1;
}

// Sendding function 
void send_datagram() {
  
  if (nb_ack_station == nb_sent) {
    // We don't resend datagram 
    build_datagram(); 
    // Display created datagram
    Serial.print("---| Datagram created: ");
    for (int i = 0; i < 27; i++) {
      Serial.print(datagram[i]);
    }
    Serial.println("");

  } else {

    // We need to resend last datagram
    memcpy(datagram, last_datagram, 27);  // Restore last datagram sent
    Serial.println("---| Resending previous datagram");
  }

  vw_send((uint8_t *)datagram, 27);
  vw_wait_tx();

  // Keep datagram on buffer
  memcpy(last_datagram, datagram, sizeof(datagram));
  last_send_size = sizeof(datagram);

  Serial.print("---| nb_sent: ");
  Serial.println(nb_sent);
  Serial.println("===> Sent!");
  Serial.println("");

  // Reset the flag to wait
  send_datagram_flag = false; 
}



// Datagram decoding function 
void datagram_decoding(char* received_msg) {
  // Get datagram infos 
  char radio_protocol_1 = received_msg[0]; 
  char radio_protocol_2 = received_msg[1]; 
  int msg_target = received_msg[2]; 
  char msg_emetor = received_msg[3]; 
  char datagram_type = received_msg[4]; 
  char datas_type = received_msg[5]; 
  int nb_sent_msg = received_msg[6] - '0'; 
  int nb_ack_msg = received_msg[7] - '0'; 

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

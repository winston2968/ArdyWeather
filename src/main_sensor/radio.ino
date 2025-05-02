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
  - Nb Send : Number of sent datagram. 
  - Nb ACK : Number of acquitted datagram. 
  - Datas : Datas if it's a Datas type datagram. 
*/


// Function to build datagram 
void build_datagram() {
  if (nb_sent == 30) {
    nb_sent = 0;
  }
  nb_sent += 1;

  memset(datagram, 0, 27);

  datagram[0] = 'A';
  datagram[1] = 'W';
  datagram[2] = '1'; // Target
  datagram[3] = SENSOR_NUM; 
  datagram[4] = 'D'; // Type
  datagram[5] = '0' + nb_sent;
  datagram[6] = '0' + nb_ack;

  for (int i = 7; i < 27; i++) {
    datagram[i] = datas_example[i - 7];
  }
}



// Sendding function 
void send_datagram(bool resend = false) {

  if (!resend) {

    // We don't resend datagram 
    build_datagram(); 
    Serial.print("---| Datagram created: ");
    for (int i = 0; i < 27; i++) {
      Serial.print(datagram[i]);
    }
    Serial.println("");

  } else {

    // We need to resend last datagram
    memcpy(datagram, last_datagram, 27);  // Restaurer la dernière version envoyée
    Serial.println("---| Resending previous datagram:");
  }

  vw_send((uint8_t *)datagram, 27);
  vw_wait_tx();

  // Keep datagram on buffer
  last_send_time = millis(); 
  memcpy(last_datagram, datagram, sizeof(datagram));
  last_send_size = sizeof(datagram);

  Serial.print("---| nb_sent: ");
  Serial.println(nb_sent);
  Serial.println("===> Sent!");
  Serial.println("");

  waiting_ack = true ; 
}





// Datagram decoding function 
void datagram_decoding(char* received_msg) {
  // Get datagram infos 
  char radio_protocol_1 = received_msg[0]; 
  char radio_protocol_2 = received_msg[1]; 
  int msg_target = received_msg[2]; 
  char msg_emetor = received_msg[3]; 
  char datagram_type = received_msg[4]; 
  int nb_sent_msg = received_msg[5] - '0'; 
  int nb_ack_msg = received_msg[6] - '0'; 

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

    waiting_ack = false ; 
  } 
}

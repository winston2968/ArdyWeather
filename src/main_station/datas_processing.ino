// ===========================================================================
//                    Station Datas Processing 
// ===========================================================================


/*
  Add received datas to storage
*/

void add_to_storage(char* received_msg, char datas_type) {
  if (storage_count == STORAGE_SIZE) {
    Serial.println("ERROR : Storage full !"); 
  } else {
    if (datas_type = 'T') {
      for (int i = 0; i < 16; i++) {
        temp_storage[i + storage_count * 16] = received_msg[i + 10]; 
      } 
    } else {
      for (int i = 0; i < 18; i++) {
        hum_storage[i + storage_count * 16] = received_msg[i + 10]; 
      } 
    }
  }
  
}



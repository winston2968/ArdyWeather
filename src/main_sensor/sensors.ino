// ===========================================================================
//                          Weather Sensor Functions
// ===========================================================================

/*
  Generate fake temperature and humidity values 
  to send to the station.   
*/

void update_temp_hum() {
  float actual_temp = (float) random(0, 40); 
  float actual_hum = (float) random(0, 100); 
  if (temp_hum_cpt == 57) {
    send_temp_hum(); 
    temp_hum_cpt = 0; 
  } else {
    int row = temp_hum_cpt % 3;       // 3 lignes
    int col = temp_hum_cpt / 3;       // 19 colonnes
    temp_table[row][col] = actual_temp; 
    hum_table[row][col] = actual_hum; 
    temp_hum_cpt++;
  }
  display_tables(); 
}


/*
  Convert float temperature table to char table to send it throught radio. 
  It get a fixed 3 length float table. 
*/

char* convert_temp_table_to_ASCII_table(float table[]) {
  // Calculate string table length
  int length = sizeof(table) ; 
  // Create table 
  char* char_table = (char*)malloc(length * 3 + 1);  // +1 for the null terminator
  Serial.println("-----");
  Serial.println(length);
  // Convert float values
  for (int i = 0; i < length ; i++) {
    int value = table[i] * 100;
    // Convert to char
    char c1 = (value / 91) + 33 ;
    char c2 = ((value % 91) / 10) + 33;
    char c3 = (value % 10) + 33 ;
    // Store the values in char_table
    char_table[i * 3] = c1;
    char_table[i * 3 + 1] = c2;
    char_table[i * 3 + 2] = c3; 
  }
  char_table[length * 3] = '\0'; // Null-terminate the string

  return char_table ;
}



void send_temp_hum() {
  // TODO : fill the function 
  // Add a flag to know when we have to send 
}


/*
  Display temp and hum tables in Serial Monitor
*/
void display_tables() {
  Serial.println("===> Temp Table :"); 
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 19; j++) {
      Serial.print(temp_table[i][j], 2);
      Serial.print(" ");
    }
    Serial.println();
  }

  Serial.println("===> Hum Table :"); 
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 19; j++) {
      Serial.print(hum_table[i][j], 2);
      Serial.print(" ");
    }
    Serial.println();
  }
}

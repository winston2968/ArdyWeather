// ===========================================================================
//                          Weather Sensor Functions
// ===========================================================================



void set_update_temp_hum_flag() {
  temp_hum_flag = true ; 
}

/*
  Generate fake temperature and humidity values 
  to send to the station.   
*/

void update_temp_hum() {

  float actual_temp = (float) random(0, 40); 
  float actual_hum = (float) random(0, 100); 
  if (temp_hum_cpt == 54) {
    build_merged_table(); 
    temp_hum_cpt = 0; 
    // display_merged_table(); 
    // reset_tables();
  } 
  // Adding values to tables in every case
  int row = temp_hum_cpt % 3;       // 3 lignes
  int col = temp_hum_cpt / 3;       // 19 colonnes
  temp_table[row][col] = actual_temp; 
  hum_table[row][col] = actual_hum; 
  temp_hum_cpt++;
  // Reset the flag 
  temp_hum_flag = false ; 

  // Serial.print("Values cpt tables: "); 
  // Serial.println(temp_hum_cpt); 
  // Serial.println(); 
}


/*
  Convert float tables to char tables for sendding. 
  Each float value is converted to 3 ASCII char. 
*/

void convert_float_table_to_ASCII(float table[3][18], char* dest) {
  int cpt = 0; 
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 18; j++) {
      int value = (int) (table[i][j] * 100);     // Two decimals precision

      char c1 = (value / 91) + 33 ;
      char c2 = ((value % 91) / 10) + 33;
      char c3 = (value % 10) + 33 ; 

      dest[cpt++] = c1; 
      dest[cpt++] = c2; 
      dest[cpt++] = c3; 
    }
  }
}

/* 
  Copy and convert temp_table and hum_table in merged_table for sendding.   
*/

void build_merged_table() {
  convert_float_table_to_ASCII(temp_table, merged_table); 
  convert_float_table_to_ASCII(hum_table, merged_table + 162); 
  nb_tables_stack = 18;       // We need to send 18 tables with radio signal
}


/*
  Display temp and hum tables in Serial Monitor
*/

void display_tables() {
  Serial.println("===> Temp Table :"); 
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 18; j++) {
      Serial.print(temp_table[i][j], 2);
      Serial.print(" ");
    }
    Serial.println();
  }

  Serial.println("===> Hum Table :"); 
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 18; j++) {
      Serial.print(hum_table[i][j], 2);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void display_merged_table() {
  for (int i = 0; i < 324; i++) {
    Serial.print(merged_table[i]); 
    Serial.print(" ");
    if (i % 20 == 0) Serial.println(); 
  }
  Serial.println(); 
}

/*
  Reset Tables to zero 
  (Not efficient in real-life because it takes too time... but good for tests)
*/

void reset_tables() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 19; j++) {
      temp_table[i][j] = 0; 
      hum_table[i][j] = 0; 
    }
  }
}

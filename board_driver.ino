/* HW GPIO configuration */
#define LED_MR_N_PIN  4 // RESET, D4
#define LED_CLOCK_PIN 6 //SHCP, D5
#define LED_LATCH_PIN 5 //STCP, D6 
#define LED_OE_N_PIN 3 // D3 
#define LED_PIN 2 //D2

#define HALL_OUT_S0 10 //D10
#define HALL_OUT_S1  9 //D9
#define HALL_OUT_S2 8 //D8

#define HALL_ROW_S0 A7  //A7/D21
#define HALL_ROW_S1 A6  //A6/D20
#define HALL_ROW_S2 A5  //A5/D19

#define HALL_SENSE A3  //A3

/* BOARD SETUP/CONFIGURATION */
#define LED_BRIGHTNESS 100 // LED brightess in percent
#define SENSE_THRS 200 // Reduce if pieces are not recognized, increase if Board starts blinking randomly

#define LED_DIM_VAL (255 - LED_BRIGHTNESS/100 * 255)  

/* ---------------------------------------
 *  Function to initiate GPIOs.
 *  Defines GPIOs input and output states. 
 *  Depends on Arduino HW (adapt HW GPIO configuration to match Arduino Board)
 *  @params[in] void
 *  @return void
*/   
void initHW(void) {
  pinMode(LED_MR_N_PIN, OUTPUT);
  pinMode(LED_CLOCK_PIN, OUTPUT);
  pinMode(LED_LATCH_PIN, OUTPUT);
  pinMode(LED_OE_N_PIN, OUTPUT);
  pinMode(LED_OE_N_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_MR_N_PIN, 0);
  digitalWrite(LED_OE_N_PIN, 1);

  pinMode(HALL_OUT_S0, OUTPUT);
  pinMode(HALL_OUT_S1, OUTPUT);
  pinMode(HALL_OUT_S2, OUTPUT);

  pinMode(HALL_ROW_S0, OUTPUT);
  pinMode(HALL_ROW_S1, OUTPUT);
  pinMode(HALL_ROW_S2, OUTPUT);

  pinMode(HALL_SENSE, INPUT);

}


/* ---------------------------------------
 *  Function to write array to LED shift registers.
 *  Activates LEDs immediately.
 *  @params[in] byte array (max size 8 bytes)
 *  @return void
*/   
void shiftOut(byte ledBoardState[]) {

  bool pinStateEN;

  digitalWrite(LED_PIN, 0);
  digitalWrite(LED_MR_N_PIN, 0);
  delay(1);
  digitalWrite(LED_MR_N_PIN, 1);

  for (int i = 0; i < 8; i++)  {
    for (int k = 0; k < 8; k++) {

      digitalWrite(LED_CLOCK_PIN, 0);

      if (ledBoardState[i] & (1 << k)) {
        pinStateEN = 1;
      }
      else {
        pinStateEN = 0;
      }
      
      digitalWrite(LED_PIN, pinStateEN);
      digitalWrite(LED_CLOCK_PIN, 1);
      digitalWrite(LED_PIN, 0);
    }
  }
  digitalWrite(LED_CLOCK_PIN, 0);

}



/* ---------------------------------------
 *  Function to ready Hall sensors states to array.
 *  Multiplexing all sensors. Sets 0 or 1 in array if threshold is exceeded.
 *  @params[in] byte array (max size 8 bytes)
 *  @return void
*/   
void readHall(byte hallBoardState[]) {

  int hall_val = 0;

  for (int k = 0; k < 8; k++) {
    hallBoardState[k] = 0x00;
  }

  for (int row_index = 0; row_index < 8; row_index++)
  {
    bool bit0 = ((byte)row_index & (1 << 0)) != 0;
    bool bit1 = ((byte)row_index & (1 << 1)) != 0;
    bool bit2 = ((byte)row_index & (1 << 2)) != 0;
    digitalWrite(HALL_ROW_S0, bit0);
    digitalWrite(HALL_ROW_S1, bit1);
    digitalWrite(HALL_ROW_S2, bit2);

    for (int col_index = 0; col_index < 8; col_index++) {
        bool bit0 = ((byte)col_index & (1 << 0)) != 0;
        bool bit1 = ((byte)col_index & (1 << 1)) != 0;
        bool bit2 = ((byte)col_index & (1 << 2)) != 0;
        digitalWrite(HALL_OUT_S0, bit0);
        digitalWrite(HALL_OUT_S1, bit1);
        digitalWrite(HALL_OUT_S2, bit2);

      //delayMicroseconds(10);
      hall_val = analogRead(HALL_SENSE);
      //delayMicroseconds(100);


      if (hall_val < SENSE_THRS) {
        hallBoardState[row_index] |= 1UL << (col_index);
      }
    }

    }

}




/* ---------------------------------------
 *  Function that clears all LED states.
 * Writes 0 to shift registers for all LEDs.
 *  @params[in] void
 *  @return void
*/  
void clearDisplay(void) {
  byte ledBoardState[8];
  
  for (int k = 0; k < 8; k++) {
    ledBoardState[k] = 0x00;
  }
  digitalWrite(LED_MR_N_PIN, 0);
  digitalWrite(LED_OE_N_PIN , 1);
  digitalWrite(LED_LATCH_PIN, 0);
  shiftOut(ledBoardState);
  digitalWrite(LED_LATCH_PIN, 1);
  //digitalWrite(LED_OE_N_PIN , 1);
  analogWrite(LED_OE_N_PIN , LED_DIM_VAL);
}


void displayStep(void) {
  clearDisplay();
  
  byte led_test_array[8] = {0x00};
 
  for (int i = 0; i<8; i++)
  {
    if (channel_vel[i] > 0)
    {
       led_test_array[i] |= 1UL << (current_step);
    }
  }

  
  //led_test_array[0] |= 1UL << (current_step);
  
  digitalWrite(LED_LATCH_PIN, 0);
  shiftOut(led_test_array);
  digitalWrite(LED_LATCH_PIN, 1);
  //digitalWrite(LED_OE_N_PIN , 0);
  analogWrite(LED_OE_N_PIN , LED_DIM_VAL);

}

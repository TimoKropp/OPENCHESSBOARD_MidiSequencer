/* ---------------------------------------
 * This is the main file of the OpenChessBoard firmware v1.0.0
*/   



int current_step = 0;
int channel_vel[8] = {0};
   

/*MIDI NOTES 
                OCTAVE
NOTE -1  0   1   2   3   4   5   6   7    8   9
   C  0 12  24  36  48  60  72  84  96  108 120
  C#  1 13  25  37  49  61  73  85  97  109 121
   D  2 14  26  38  50  62  74  86  98  110 122
  D#  3 15  27  39  51  63  75  87  99  111 123
   E  4 16  28  40  52  64  76  88 100  112 124
   F  5 17  29  41  53  65  77  89 101  113 125
  F#  6 18  30  42  54  66  78  90 102  114 126   
   G  7 19  31  43  55  67  79  91 103  115 127
  G#  8 20  32  44  56  68  80  92 104  116 
   A  9 21  33  45  57  69  81  93 105  117 
  A# 10 22  34  46  58  70  82  94 106  118 
   B 11 23  35  47  59  71  83  95 107  119 
*/

/* Note Selection from upper table for all 8 rows */
byte PadNote[8] = { 
 1, 2 , 3 , 60 , 72, 74 , 77 , 79 };   
   
#define BPM 120
#define NOTELENGTH  500


unsigned long initTime[8] = {0};
unsigned long step_initTime = 0;
int BPM_delay = (60000/BPM)-75;
int duration[8] = {1};
int step_duration = 1;


void setup() {
  Serial.begin(112500);
  initHW();
}


void loop() {
  read_keys();
  displayStep();
  step_initTime = millis();
  for (int channel = 0; channel < 8; channel++)
  {
    initTime[channel] = millis();
  }
  for (int channel = 0; channel < 8; channel++)
  {
    if(channel < 3){
      if (channel_vel[channel]>0){
        initTime[channel] = millis();
        MIDI_TX(144,PadNote[channel],channel_vel[channel], 1); //note on
      }
    }
    /* use the bottom three rows for midi channel 2 */
    else{
      if(channel_vel[channel]>0){
        initTime[channel] = millis();
        MIDI_TX(144,PadNote[channel],channel_vel[channel], 2); //note on
      }
    }
  }
  
  step_duration  =  millis() - step_initTime;
  
  for (int channel = 0; channel < 8; channel++){
    duration[channel] =  millis() - initTime[channel];
  }
  
  while (step_duration < BPM_delay){
    
    step_duration =  millis() - step_initTime;
    
    for (int channel = 0; channel < 8; channel++){
      duration[channel] =  millis() - initTime[channel];
      if (duration[channel] >= NOTELENGTH){
            MIDI_TX(128,PadNote[channel],0, 1); //note off
            MIDI_TX(128,PadNote[channel],0, 2); //note off
      }
    }
  }
  current_step++;
  if(current_step == 8){
    
    current_step = 0;
  }
}

void read_keys(){
  
  for (int k = 0; k < 8; k++) {
    channel_vel[k] = 0;
  }
  
  byte read_hall_array[8] = {0x00};
  
  readHall(read_hall_array);
  
  for (int i = 0; i<8; i++){    
      if((read_hall_array[7-i] & (1UL << (7-current_step))) > 0)
       {
          channel_vel[i] = 127;
       }
    }
  }

  void MIDI_TX(byte MESSAGE, byte PITCH, byte VELOCITY, byte MIDICHANNEL)
{
    byte status1 = MESSAGE + MIDICHANNEL;
    Serial.write(status1);
    Serial.write(PITCH);
    Serial.write(VELOCITY);
}

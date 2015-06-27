#define DA_0 4
#define DA_1 5
#define DA_2 6
#define DA_3 7
//#define DA_4 8
//#define DA_5 9
//#define DA_6 10
//#define DA_7 11
#define DA_4 19
#define DA_5 18
#define DA_6 17
#define DA_7 16


#define CS   10

#define SDATA 8
#define RCLK  14
#define SRCLK 15


#define INACTIVE B11110011
#define READ     B00000100
#define WRITE    B00001000
#define ADDRESS  B00001100
#define CYCLE_TIME 20000


#include <SD.h>

void writeRegister(int reg, int* frame);

File root;
File current_song;

void setup()
{
  /*Set pins 2 and 3 as output, but leave the state of pins 0 and
  1 unchanged */
  DDRD = DDRD | B00001100;
  /*Set the remaining pins used for output in the conventional way
  */
  pinMode(SDATA, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(DA_0, OUTPUT);
  pinMode(DA_1, OUTPUT);
  pinMode(DA_2, OUTPUT);
  pinMode(DA_3, OUTPUT);
  pinMode(DA_4, OUTPUT);
  pinMode(DA_5, OUTPUT);
  pinMode(DA_6, OUTPUT);
  pinMode(DA_7, OUTPUT);
  
  pinMode(CS, OUTPUT);

  digitalWrite(RCLK, LOW);
  digitalWrite(SRCLK, LOW);
  Serial.begin(9600);
  
  if (!SD.begin(CS))
  {
     Serial.println("Initialization Failed");
  }
  else
  {
     Serial.println("Initialization OK");
     root = SD.open("/");
     current_song = root.openNextFile();
     if (current_song)
     {
         Serial.println("File Open OK");
     }
  }
     
  int i = 0;
  for (i = 0; i < 8; i++)
  {
      writeShift(0x80 >> i);
      delay(50); 
  }
  for (i = 0; i < 8; i++)
  {
      writeShift(0x01 << i);
      delay(50); 
  }
  PORTD &= INACTIVE;

}

void writeShift(byte val)
{
    digitalWrite(SRCLK, LOW);
    digitalWrite(RCLK, LOW);
    digitalWrite(SDATA, val & 0x1);
    digitalWrite(RCLK, LOW);
    digitalWrite(RCLK, HIGH);
    digitalWrite(RCLK, LOW);
    digitalWrite(SDATA, (val & 0x2) >> 1);
    digitalWrite(RCLK, HIGH);
    digitalWrite(RCLK, LOW);
    digitalWrite(SDATA, (val & 0x4) >> 2);
    digitalWrite(RCLK, HIGH);
    digitalWrite(RCLK, LOW);
    digitalWrite(SDATA, (val & 0x8) >> 3);
    digitalWrite(RCLK, HIGH);
    digitalWrite(RCLK, LOW);
    digitalWrite(SDATA, (val & 0x10) >> 4);
    digitalWrite(RCLK, HIGH);
    digitalWrite(RCLK, LOW);
    digitalWrite(SDATA, (val & 0x20) >> 5);
    digitalWrite(RCLK, HIGH);
    digitalWrite(RCLK, LOW);
    digitalWrite(SDATA, (val & 0x40) >> 6);
    digitalWrite(RCLK, HIGH);
    digitalWrite(RCLK, LOW);
    digitalWrite(SDATA, (val & 0x80) >> 7);
    digitalWrite(RCLK, HIGH);
    digitalWrite(RCLK, LOW);
    digitalWrite(SRCLK, HIGH);
    digitalWrite(SRCLK, LOW);
}

void loop()
{
  //int frame[16];
  unsigned long start = micros();      
  if(current_song.available())
  {
      //int i;

      byte reg_0 = current_song.read();
      byte reg_1 = current_song.read();
      byte reg_2 = current_song.read();
      byte reg_3 = current_song.read();
      byte reg_4 = current_song.read();
      byte reg_5 = current_song.read();
      byte reg_6 = current_song.read();
      byte reg_7 = current_song.read();
      byte reg_8 = current_song.read();
      byte reg_9 = current_song.read();
      byte reg_10 = current_song.read();
      byte reg_11 = current_song.read();
      byte reg_12 = current_song.read();
      byte reg_13 = current_song.read();
      //byte reg_14 = current_song.read();
      //byte reg_15 = current_song.read();
      
      //for(i = 0; i < 16; i++)
      //{
      //  frame[i] = Serial.read();
      //}
      writeRegister(0, reg_0);   
      writeRegister(1, reg_1);   
      writeRegister(2, reg_2);   
      writeRegister(3, reg_3);   
      writeRegister(4, reg_4);   
      writeRegister(5, reg_5);   
      writeRegister(6, reg_6);   
      writeRegister(7, reg_7);   
      writeRegister(8, reg_8);   
      writeRegister(9, reg_9);   
      writeRegister(10, reg_10);   
      writeRegister(11, reg_11);   
      writeRegister(12, reg_12);   
      writeRegister(13, reg_13);   
      writeShift(reg_0);

      while(micros() - start < CYCLE_TIME);
       //Serial.print("\n");
  }
  else
  {
      current_song = root.openNextFile();
      if (!current_song)
      {
          root.rewindDirectory();
          current_song = root.openNextFile(); 
      }
  }
}  

void writeRegister(byte reg, byte val)
{
      //int j;
      //  Serial.print(frame[reg], HEX);
      //  Serial.print(' ');
      //Serial.print("\n");
      /*Assume the chip is in 'Inactive' mode*/
      PORTD |= ADDRESS;
      digitalWrite(DA_0, reg & 0x1);
      digitalWrite(DA_1, (reg & 0x2) >> 1);
      digitalWrite(DA_2, (reg & 0x4) >> 2);
      digitalWrite(DA_3, (reg & 0x8) >> 3);
      digitalWrite(DA_4, 0);
      digitalWrite(DA_5, 0);
      digitalWrite(DA_6, 0);
      digitalWrite(DA_7, 0);
      delayMicroseconds(1);
      PORTD &= INACTIVE;
      digitalWrite(DA_0, val & 0x1);
      digitalWrite(DA_1, (val & 0x2) >> 1);
      digitalWrite(DA_2, (val & 0x4) >> 2);
      digitalWrite(DA_3, (val & 0x8) >> 3);
      digitalWrite(DA_4, (val & 0x10) >> 4);
      digitalWrite(DA_5, (val & 0x20) >> 5);
      digitalWrite(DA_6, (val & 0x40) >> 6);
      digitalWrite(DA_7, (val & 0x80) >> 7);
      delayMicroseconds(1);
      PORTD |= WRITE;
      delayMicroseconds(1);
      PORTD &= INACTIVE;
}




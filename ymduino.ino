#define DA_0 4
#define DA_1 5
#define DA_2 6
#define DA_3 7
#define DA_4 8
#define DA_5 9
#define DA_6 10
#define DA_7 11

#define INACTIVE B11110011
#define READ     B00000100
#define WRITE    B00001000
#define ADDRESS  B00001100

void writeRegister(int reg, int* frame);

void setup()
{
  /*Set pins 2 and 3 as output, but leave the state of pins 0 and
  1 unchanged */
  DDRD = DDRD | B00001100;
  /*Set the remaining pins used for output in the conventional way
  */
  //pinMode(2, OUTPUT);
  //pinMode(3, OUTPUT);
  pinMode(DA_0, OUTPUT);
  pinMode(DA_1, OUTPUT);
  pinMode(DA_2, OUTPUT);
  pinMode(DA_3, OUTPUT);
  pinMode(DA_4, OUTPUT);
  pinMode(DA_5, OUTPUT);
  pinMode(DA_6, OUTPUT);
  pinMode(DA_7, OUTPUT);

  Serial.begin(9600);
  PORTD &= INACTIVE;
}

void loop()
{
  int frame[16];
  if(Serial.available() >= 16)
  {
      int i;
      for(i = 0; i < 16; i++)
      {
        frame[i] = Serial.read();
      }
          
      int j;
      for(j = 0; j < 16; j++)
        writeRegister(j, frame);   

      /*for(j = 0; j < 16; j++)
      {
        Serial.print(frame[j], HEX);
        Serial.print(' ');

      }*/
      
       //Serial.print("\n");
  }
}  

void writeRegister(int reg, int* frame)
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
      delayMicroseconds(2);
      PORTD &= INACTIVE;
      digitalWrite(DA_0, frame[reg] & 0x1);
      digitalWrite(DA_1, (frame[reg] & 0x2) >> 1);
      digitalWrite(DA_2, (frame[reg] & 0x4) >> 2);
      digitalWrite(DA_3, (frame[reg] & 0x8) >> 3);
      digitalWrite(DA_4, (frame[reg] & 0x10) >> 4);
      digitalWrite(DA_5, (frame[reg] & 0x20) >> 5);
      digitalWrite(DA_6, (frame[reg] & 0x40) >> 6);
      digitalWrite(DA_7, (frame[reg] & 0x80) >> 7);
      delayMicroseconds(15);
      PORTD |= WRITE;
      delayMicroseconds(15);
      PORTD &= INACTIVE;
      //delayMicroseconds(5);
}



void setup()
{
  Serial.begin(9600);
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
      {
        Serial.print(frame[j], HEX);
        Serial.print(' ');
      }
       Serial.print("\n");
  }

}



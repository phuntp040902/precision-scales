  #include <LiquidCrystal.h>
  #include <math.h>
  LiquidCrystal lcd(7, 6, 5, 4, 10, 8);
   
  #define DT A0
  #define SCK A1
  #define sw 9
  #define led 11
  float sample=0;
  float val=0;
  float count=0;
  int max_value = 0;
   
  unsigned long readCount(void)
  {
      unsigned long Count;
      unsigned char i;
      pinMode(DT, OUTPUT);
      digitalWrite(DT,HIGH);
      digitalWrite(SCK,LOW);
      Count=0;
      pinMode(DT, INPUT);
      while(digitalRead(DT));
      for (i=0;i<24;i++)
      {
        digitalWrite(SCK,HIGH);
        Count=Count<<1;
        digitalWrite(SCK,LOW);
        if(digitalRead(DT))
        Count++;
      }
      digitalWrite(SCK,HIGH);
      Count=Count^0x800000;
      digitalWrite(SCK,LOW);
      return(Count);
  }
   
  void setup()
  {   
      pinMode(2, INPUT_PULLUP);
      attachInterrupt(0, turn_up_value, FALLING);
      pinMode(3, INPUT_PULLUP);
      attachInterrupt(1, turn_down_value, FALLING);
      pinMode(led, OUTPUT);
      digitalWrite(led, LOW);
      pinMode(SCK, OUTPUT);
      pinMode(sw, INPUT_PULLUP);
      lcd.begin(16, 2);
      lcd.print(" Weight ");
      lcd.setCursor(0,1);
      lcd.print(" Measurement ");
      delay(1000);
      lcd.clear();
      calibrate();
  }
   
  void loop()
  {   
      count= readCount();
      float w = 2*(-(count-sample)/val);
      float w1 = (round(w))/1000.0;
      if (w1 > max_value)
        digitalWrite(led, HIGH);
      else
        digitalWrite(led, LOW);
      lcd.setCursor(0,0);
      lcd.print("Do:");
      lcd.setCursor(5,0);
      lcd.print(w1);
      lcd.print(" g");
      lcd.setCursor(0,1);
      lcd.print("Max ");
      lcd.setCursor(7,1);
      lcd.print(max_value);
      delay (2000);
      lcd.clear(); 
      if(digitalRead(sw)==0)
      {
        val=0;
        sample=0;
        w=0;
        count=0;
        calibrate();
      }
      }
  void turn_up_value()
  {   
      delay(20);
      if ((max_value < 1000)&&(digitalRead(2)==0))
          max_value += 50;
        lcd.setCursor(0,1);
        lcd.print("Max ");
        lcd.setCursor(7,1);
        lcd.print(max_value);
        lcd.display();
        delay (1000);      
  }
  void turn_down_value()
  {   
      delay(20);
      if ((max_value > 0)&&(digitalRead(3)==0))
          max_value -= 50;
        lcd.setCursor(0,1);
        lcd.print("Max ");
        lcd.setCursor(7,1);
        lcd.print(max_value);
        lcd.display(); 
        delay (1000);    
  }
  
  void calibrate()
  {
      lcd.clear();
      lcd.print("Calibrating...");
      lcd.setCursor(0,1);
      lcd.print("Please Wait...");
      for(int i=0;i<100;i++)
      {
        count=readCount();
        sample+=count;
      }
      sample/=100;
      lcd.clear();
      lcd.print("Put 100g & wait");
      count=0;
      while(count<1000)
      {
        count=readCount();
        count=sample-count;
      }
      lcd.clear();
      lcd.print("Please Wait....");
      delay(2000);
      for(int i=0;i<100;i++)
      {
        count=readCount();
        val+=sample-count;
      }
      val=val/100.0;
      val=val/50000.0; // put here your calibrating weight
      lcd.clear();
  }

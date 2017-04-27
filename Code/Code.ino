#include <LiquidCrystal.h>

#define reed A0

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

float radius = 3.0;

int reedVal;
long timer = 0;
float mph = 0.00;
float circumference;

int maxReedCounter = 100;
int reedCounter;

void setup()
{
  lcd.begin(16, 2);
 
  
  reedCounter = maxReedCounter;
  circumference = 2*3.14*radius;

  pinMode(reed, INPUT);
  
  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 1999;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS11);   
  TIMSK1 |= (1 << OCIE1A);
  
  sei();
 
}

ISR(TIMER1_COMPA_vect) 
{
  reedVal = digitalRead(reed);
  if (reedVal)
  {
    if (reedCounter == 0)
    {
      mph = (56.8*float(circumference))/float(timer);
      timer = 0;
      reedCounter = maxReedCounter;
    }
    else{
      if (reedCounter > 0)
      {
        reedCounter -= 1;
      }
    }
  }
  else
  {
    if (reedCounter > 0)
    {
      reedCounter -= 1;//decrement reedCounter
    }
  }
  if (timer > 3000)
  {
    mph = 0;//if no new pulses from reed switch- tire is still, set mph to 0
  }
  else
  {
    timer += 1;//increment timer
  } 
}

void displayMPH()
{
  lcd.setCursor(0,1);
  lcd.print(mph);
}

void loop()
{
  lcd.print("Speed...");
  lcd.setCursor(8,1);
  lcd.print("mph");
  displayMPH();
  delay(10);
  lcd.clear();
}


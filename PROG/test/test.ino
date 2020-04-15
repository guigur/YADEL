#include <LiquidCrystal.h>
#define TEMPERATURENOMINAL 25   
#define THERMISTORNOMINAL 100000      
#define BCOEFFICIENT 3950
#define WARNTEMP 60
#define MAXTEMP 65

//lcd(rs, en, d4, d5, d6, d7);
LiquidCrystal lcd(13, 12, 11, 10, 8, 7);

uint8_t ctrl = 5;
uint8_t fan = 6;
uint8_t buz = 9;
uint8_t Vsense = A0;
uint8_t temp = A1;

byte degreeChar[] = {
  B01100,
  B10010,
  B10010,
  B01100,
  B00000,
  B00000,
  B00000,
  B00000
};
byte tempChar[] = {
  B00100,
  B01010,
  B01010,
  B01010,
  B01110,
  B11111,
  B11111,
  B01110
};

void setup()
{
  pinMode(ctrl, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(buz, OUTPUT);
  pinMode(Vsense, INPUT);
  pinMode(temp, INPUT);

  lcd.begin(20, 4);
  lcd.print("hello, world!");
  lcd.createChar(1, degreeChar);
  lcd.createChar(2, tempChar);
}

void  beep(unsigned char delayms)
{
    //tone(buzzer, 440, delayms);
    analogWrite(buz, 1);
    delay(delayms);
    analogWrite(buz, 0);
    delay(delayms);
}

void loop() {
   float reading;
  reading = analogRead(temp);

  reading = (1023 / reading)  - 1;     // (1023/ADC - 1) 
  reading = 4700 / reading;  // 10K / (1023/ADC - 1)
  
  lcd.setCursor(0, 1);
  //lcd.print(millis() / 1000);
  
  lcd.write(2);
  //lcd.print(reading);

  float steinhart;
  steinhart = reading / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
  lcd.print(" ");
  lcd.print(steinhart);
  lcd.write(1);
  lcd.print("C");
  int val = 0;
  if(steinhart < 70)
    val = map(steinhart, 30, 70, 10, 255);
  else
    val = 255;
  analogWrite(fan, val);

  
  if (steinhart >= MAXTEMP)
  {
    beep(100);
  }
  else if (steinhart >= WARNTEMP)
  {
    beep(440);
  }
}

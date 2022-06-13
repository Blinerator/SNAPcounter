#include <Pushbutton.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

Pushbutton button(12); //this is the button that determines how many notes to count at a time

#define RESET 7
#define onoff 8

int count=0, triggered = 0, motorlatch=1, countupto=-1, switchlatch=0;
int count1=3;

void setup() {
  lcd.begin();
  lcd.setCursor(0,0);
  lcd.print("Count:");
  lcd.print(count);
  lcd.setCursor(0,1);
  lcd.print("Cnt Up To:NA");
  
  pinMode(6, OUTPUT); //this is the PWM pin
  pinMode(A1, INPUT); //this is the photoresistor pin
  pinMode(RESET, INPUT_PULLUP);
  pinMode(onoff, INPUT_PULLUP);

}
void loop() {
  lcd.setCursor(0,1);
  lcd.print("Cnt Up To:");
  if(button.getSingleDebouncedPress()){
    count1++;
  }
  
//this section sets how many notes to count at a time

  if(count1==0){
    countupto=10;
  }
  if(count1==1){
    countupto=20;
  }
  if(count1==2){
    countupto=40;
  }
  if(count1==3){
    countupto=-1;
  }
  if(count1==4){
    count1=0;
  }

  if(count1==3){
    lcd.print("NA");
  }
  else{
    lcd.print(countupto);
  }




  if(motorlatch==0){  //turning motor on/off.  Faster speed=faster wear for adhesive
    analogWrite(6,75);
  }
  else{
    analogWrite(6,0);
  }

  lcd.setCursor(6,0);
  lcd.print(count);
  lcd.print("   ");
  int reading = analogRead(A1);
  if(reading<100){          //when contact with lazer is broken, reading drops
    if(triggered == 0){
      count++;
      triggered = 1;
    }
  }
  if(reading>100){
    triggered = 0;
  }

  if(count == countupto){  //stops the motor if a certain # of notes has been counted
    if(triggered==0){
      motorlatch=1;
      switchlatch=1;
    }
  }
  else{
    switchlatch=0;
  }

  if(digitalRead(RESET)==LOW){
    count=0;
  }

  lcd.setCursor(12,0);
  if(switchlatch==0){              //tells the user if the motor is on or off
    if(digitalRead(onoff)==LOW){
        motorlatch=0;
        lcd.print("on ");
    }
    if(digitalRead(onoff)==HIGH){
        motorlatch=1;
        lcd.print("off");
    }
  }

}

#include <LiquidCrystal.h>
#include <EEPROM.h>

struct Data{
  float Voltage;
  float Current;
};


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
int Read_Voltage  = A3;
int Read_Current  = A4;
int Read_Button = A1;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
uint32_t btnTimer = 0;


void setup() {
  pinMode(Read_Button, INPUT_PULLUP);
  lcd.begin(16, 2); //Initialise 16*2 LCD
  lcd.print(" Arduino Tester "); //приветственное сообщение на 1-й строке
  lcd.setCursor(0, 1);
  lcd.print(" TLIB1 "); // приветственное сообщение на 2-й строке
  delay(2000);
  lcd.clear();
}

int counter = 0;
uint32_t sec_before=0;

void loop() {

  Data data;
 
  uint32_t sec = millis() / 1000ul; 
  
  float Voltage_Value = analogRead(Read_Voltage);
  float Current_Value = analogRead(Read_Current);
  boolean flag = false;
  
  Voltage_Value = Voltage_Value * (5.0/1023.0);
  Current_Value = Current_Value * (5.0/1023.0) * 0.01;

  data.Voltage = Voltage_Value;
  data.Current = Current_Value;
  

  lcd.setCursor(0, 0);
  lcd.print("V="); lcd.print(Voltage_Value); 
  lcd.print("  "); 
  lcd.print("I=");lcd.print(Current_Value); 
  float Power_Value = Voltage_Value * Current_Value;
  lcd.setCursor(0, 1);
  lcd.print("Power="); lcd.print(Power_Value); 

  
  if(sec-sec_before==1){
  EEPROM.put(counter, data);
  counter+= 8;
  }
  sec_before=sec;

  bool btnState = !digitalRead(Read_Button);
  if (btnState && millis() - btnTimer > 100) {
    btnTimer = millis();
    lcd.clear();
    int j=1;
    for(int i=0;i<counter;i+=8){
      
      EEPROM.get(i, data);
      lcd.clear();
      lcd.print(j);
      lcd.print(")V="); lcd.print(data.Voltage); 
      lcd.print("  "); 
      lcd.print("I=");lcd.print(data.Current);
      j++;
      delay(1000); 
    }
    
  }
  
  
  
  if(counter == 80){
    counter -= 8;
    
  }

  
  delay(200);
  lcd.clear();
  
}

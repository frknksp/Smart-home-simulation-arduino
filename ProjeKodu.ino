#include <Password.h>
#include<Keypad.h>
#include <LiquidCrystal.h>

unsigned long eskiZaman = 0;
unsigned long anlikZaman;

LiquidCrystal lcd(7, 6, 11, 10, 9, 8);

Password password= Password("1234");

const int yesilled=33;
const int kirmiziled=32;
const byte satirSayisi = 4;
const byte sutunSayisi = 3;

char tuslar[satirSayisi][sutunSayisi] = {{'1','2','3'},{'4','5','6'},{'7','8','9'},{'*','0','#'}};

byte satirPinleri[satirSayisi] = {25,26,27,28};
byte sutunPinleri[satirSayisi] = {22,23,24};

Keypad tusTakimi = Keypad(makeKeymap(tuslar), satirPinleri, sutunPinleri, satirSayisi, sutunSayisi);

#define FlamePin 30
#define buzzer 3
#define PirPin 29
#define lampPin 40

int  lm35Pin =A2;
int hareket = LOW;
int okunan_deger = 0;
float sicaklik_gerilim = 0;
float sicaklik = 0;
int flame = LOW;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  tusTakimi.addEventListener(keypadEvent);
  Serial.println("4 haneli sifrenizi giriniz (onaylamak icin * resetlemek icin # basiniz)\n");
  pinMode(FlamePin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(PirPin, INPUT);
  pinMode(lampPin, OUTPUT);
  pinMode(yesilled,OUTPUT);
  pinMode(kirmiziled,OUTPUT);
  lcd.setCursor(0,0);
  pinMode(yesilled,OUTPUT);
  pinMode(kirmiziled,OUTPUT);

}

void loop() {
  anlikZaman = millis(); 
  tusTakimi.getKey();
 
  okunan_deger = analogRead(lm35Pin);
  sicaklik_gerilim = (5000.0 / 1023.0)*okunan_deger;
  sicaklik = sicaklik_gerilim /10.0;
  lcd.setCursor(0,0);
  lcd.println("SICAKLIK:");
  lcd.println(sicaklik);
 
  if(sicaklik > 30.0 ){
  lcd.setCursor(0,1);
  lcd.println("Sicaklik yukseldi");    
  }
  if(anlikZaman-eskiZaman > 100){ 
    lcd.clear();
   eskiZaman = anlikZaman;
  }
  if(sicaklik < 20.0 ){
  lcd.setCursor(0,1);
  lcd.println("Sicaklik dustu");
  }
  
  hareket=digitalRead(PirPin); 
  if (hareket == HIGH){                   // hareket algılandıysa
     digitalWrite(lampPin, HIGH);
     }
   else{  
     digitalWrite(lampPin, LOW);
   }
     
  flame = digitalRead(FlamePin);
  if(flame== HIGH){                     //yangın algılandıysa
     digitalWrite(buzzer, HIGH);
   }
   else{
     digitalWrite(buzzer, LOW); 
   }
}
void keypadEvent(KeypadEvent tus){
  switch(tusTakimi.getState())
  {
    case PRESSED:
    Serial.print("*");
  switch(tus){
    case '*': checkPass();  password.reset(); break;
    case '#': password.reset(); break;
    default: password.append(tus);
    }
  }
}
void checkPass(){
  if(password.evaluate()){
    Serial.println(" dogru sifre girdiniz \n");
    digitalWrite(yesilled,HIGH);
    digitalWrite(kirmiziled,LOW);
  }
  else{
    Serial.println(" yanlis sifre girdiniz \n");
    digitalWrite(yesilled,LOW);
    digitalWrite(kirmiziled,HIGH);
  }
}

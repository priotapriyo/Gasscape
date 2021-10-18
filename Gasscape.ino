#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

int pos = 0;
int val;int motor = 8;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
SoftwareSerial mySerial(9, 10);
byte tx=1;
TinyGPS gps;  
const int SPEAKER = 6;
const int LED_RED = 7;
const int LED_YELLOW = 10;
int Relay=7;
int tempC_1 = 0;
int smkC_1 = 0; 
const int SensorPin1 = A0;
const int SensorPin2 = A1;
String textForSMS;

void setup()
{
  pinMode(motor, OUTPUT);
  pinMode(tx, OUTPUT);
  pinMode(Relay, OUTPUT);
  
  pinMode(SPEAKER, OUTPUT);
  lcd.begin(14, 2);
  delay(100);
  pinMode(SensorPin1, INPUT);
  pinMode(SensorPin2, INPUT);
  pinMode(SPEAKER, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT); 
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  mySerial.begin(9600);
  Serial.begin(9600); 
}
void loop()
{
  int tempC_1 = analogRead(SensorPin1);
  int SmkC_1 = analogRead(SensorPin2);
  tempC_1 = analogRead(SensorPin1); 
  tempC_1 = (5.0 * tempC_1 * 100.0) / 1024.0;
  smkC_1 = analogRead(SensorPin2); 
  smkC_1 = (5.0 * smkC_1 * 100.0) / 1024.0; 
  delay(50);
  
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  for (unsigned long start = millis(); millis() -start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      if (gps.encode(c)) 
      newData = true;  
      }
    } 
    
    if (tempC_1 >= 50) 
    {
      digitalWrite(Relay,HIGH);
      val = analogRead(pos);
      val = map(val, 0, 1023, 0, 180);
      delay(50);
      digitalWrite(motor, HIGH);
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_YELLOW, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  EXCESSIVE GAS IN THE AIR ");
      lcd.setCursor(0, 1);
      lcd.print(" DANGER!! ");
      delay(100);
      lcd.clear();
      lcd.print("Sending SMS...");
      delay(100);
      
      tone(SPEAKER, 1047, 500);
      delay(200);
      tone(SPEAKER, 1109, 300);
      delay(200);
      tone(SPEAKER, 1175, 100);
      delay(5);
      
      float flat, flon;
      unsigned long age;
      gps.f_get_position(&flat, &flon, &age);
      Serial.println("AT+CMGF=1");
      delay(100);
      Serial.print("AT+CMGS=\"+8801738739735\"\r");
      Serial.print("GAS LEAKAGE ALERT!\r");
      delay(100);
      Serial.print("AT+CMGS=\"+8801738739735\"\r");
      Serial.print("DANGER!\r");
      delay(100);
      Serial.print("AT+CMGS=\"+8801738739735\"\r");
      Serial.print("PLEASE OPEN DOORS AND WINDOWS! \r");
      delay(200);
      Serial.println((char)26); 
      delay(200);
      Serial.println();
      }
      else
      {
        digitalWrite(Relay,LOW);
        delay(50);
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(motor, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  NO GAS LEAKAGE    ");
        lcd.setCursor(0, 1);
        lcd.print("  SAFE   ");
        } 
      }

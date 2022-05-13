#include <stdlib.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#define sense_p 7         
#define sense_ty dd    
DHT dht(sense_p, sense_ty); 
#define name "name"     
#define pass "pass"   
#define ip "XXXXXXXXXX"

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

#define d1 8
#define d2 9
#define d3 10

int temp = 0, i = 0;
int led = 13;
char str[15];
String msg = "GET /update?key=xxxxx";
float temp;
int hum;
String temp;
int err;
void setup()
{

  Serial.begin(); 
  Serial.println("..");
  delay(5000);
  if(Serial.find("OK")){
    con();
  }
  pinMode(led, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d1, OUTPUT);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d1, HIGH);
  lcd.setCursor(0, 0);
  delay(2000);
  lcd.clear();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("d2 d3 d1 ");
  lcd.setCursor(0, 1);
  lcd.print("0 0 0 ");
}

void loop(){
   start: 
  err=0;
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  char buffer[10];
  temp = dtostrf(temp, 4, 1, buffer);
  update();
  if (err==1){
       goto start;

  }
  delay(1000);
  if (temp == 1)
  {
    stat();
    temp = 0;
    i = 0;
    delay(1000);
  }

}

void update(){
  String promp = "AT+CipSTART=\"TCP\",\"";
  promp += ip;
  promp += "\",80";
  Serial.println(promp);
  delay(2000);

  if(Serial.find("err")){
    return;

  }
  promp = msg ;
  promp += "&field1=";  
  promp += temp;
  promp += "&field2="; 
  promp += String(hum);
  promp += "\r\n";
  Serial.print("AT+CipSEND=");
  Serial.println(promp.length());
  if(Serial.find(">")){
    Serial.print(promp);
  }

  else{
    Serial.println("AT+CipCLOSE");

    err=1;
  }

}

  boolean con(){
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String promp="AT+CWJAP=\"";
  promp+=name;
  promp+="\",\"";
  promp+=pass;
  promp+="\"";
  Serial.println(promp);
  delay(5000);

  if(Serial.find("OK")){
    return true;

  }else{

    return false;
  }

}

void stat()
{
  if (!(strncmp(str, "d1 on", 5)))
  {
    digitalWrite(d1, LOW);
    lcd.setCursor(13, 1);
    lcd.print("ON ");
    delay(150);
  }

  else if (!(strncmp(str, "d1 0", 5)))
  {
    digitalWrite(d1, HIGH);
    lcd.setCursor(13, 1);
    lcd.print("0 ");
    delay(150);
  }

  else if (!(strncmp(str, "d2 on", 6)))
  {
    digitalWrite(d2, LOW);
    lcd.setCursor(0, 1);
    lcd.print("ON ");
    delay(150);
  }

  else if (!(strncmp(str, "d2 0", 7)))
  {
    digitalWrite(d2, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("0 ");
    delay(150);
  }

  else if (!(strncmp(str, "d3 on", 8)))
  {
    digitalWrite(d3, LOW);
    lcd.setCursor(7, 1);
    lcd.print("ON ");
    delay(150);
  }

  else if (!(strncmp(str, "d3 0", 9)))
  {
    digitalWrite(d3, HIGH);
    lcd.setCursor(7, 1);
    lcd.print("0 ");
    delay(150);
  }


}
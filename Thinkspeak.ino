#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int voltPin=A0;
float voltage;
float current,curr;
float t,t1,temp;
//
float capacity =500;
float initialCharge = 0.0;
float remainingCharge = 0.0;
float charge;
float soc = 0.0; 
//

#include <SoftwareSerial.h>
#define RX 10
#define TX 11
String AP = "OnePlus Nord";       // CHANGE ME
String PASS = "630AKeerthan"; // CHANGE ME
String API = "0HE6Y8UDAGW8IMW0";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field1 = "field1";
String field2 = "field2";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
SoftwareSerial wmod(RX,TX); 


void setup() {
  pinMode(voltPin,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  // put your setup code here, to run once:
  int i=0;
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  //lcd.print("working ");
  Serial.begin(9600);
  wmod.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

void loop() {
  // put your main code here, to run repeatedly:
  t=analogRead(A0);
  voltage=(t*5.0)/1024.0;
  lcd.setCursor(0,1);
  lcd.print("V:");
  lcd.setCursor(4,1);
  lcd.print(voltage);
  current=0;
  //
  for(int an=0;an<=20;an++)
  {
    t1=analogRead(A1);
  curr = (t1*5000.0)/1024;
  current += (2500-curr)/66;
  delay(50);
  }
  current=current/20;
  charge = (current * 1000.0 * 1000.0 )/ 3600.0; 
  remainingCharge = initialCharge + charge;
  soc = (remainingCharge / capacity) * 100.0;
  Serial.print("Remaining charge: ");
  Serial.print(remainingCharge);
  Serial.print(" mAh, ");
  Serial.print("State of charge: ");
  Serial.print(soc);
  Serial.println(" %");
  //
  temp=((analogRead(A2)/1024.0)*5000.0)/10;
  Serial.print("Temp:");
  Serial.println(temp);
  lcd.setCursor(8,1);
  lcd.print("A:");
  lcd.setCursor(11,1);
  lcd.print(current);
  Serial.println(voltage);
  Serial.println(current);
  delay(100);

  String getData2 = "GET /update?api_key="+ API +"&"+ field1 +"="+String(soc);
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData2.length()+4),4,">");
 wmod.println(getData2);delay(500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");

 String getData3 = "GET /update?api_key="+ API +"&"+ field2 +"="+String(temp);
  sendCommand("AT+CIPMUX=1",5,"OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  sendCommand("AT+CIPSEND=0," +String(getData3.length()+4),4,">");
  wmod.println(getData3);delay(500);countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0",5,"OK");
}
int getSensorData(){
  return random(1000); // Replace with your own sensor code
}
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    wmod.println(command);//at+cipsend
    if(wmod.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  found = false;
  
 }

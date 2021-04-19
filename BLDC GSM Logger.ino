/**

Name: BLDC Pump GSM Data Logger
Purpose: Logs current and voltage values of the BLDC Pump during usage.

@Author: Chrispine Tinega <tinega.chris@kickstart.org>

Last edited by: tinegachris <https://github.com/tinegachris>

@Version: 1.1

**/

char* particulars[3];
String data_str;
char sms_rx[122]; //Received 122 characters text SMS
boolean data_received = false;//
int relay1 = 7;
char buffer[50];
char number[15] = "+254768047115"; //Number to send SMS to
int analogInput = A1;
float vout = 0.0;
float vin = 0.0;
float R1 = 29400.0; // resistance of R1 (100K) -see text!
float R2 = 2200.0; // resistance of R2 (10K) - see text!
float Power=0.0;
int value = 0;

void setup()
{
  
   pinMode(analogInput, INPUT);
   Serial.begin(9600); //opens serial port, sets data rate to 9600 bps
   pinMode(relay1, OUTPUT); 
   
   for(int y= 0;y<10;y++){
      digitalWrite(relay1, HIGH);
      delay(200);
      digitalWrite(relay1, LOW);
      delay(200);
    }
 
   delay(60000);  //delay 1 minute for GSM to power up and accept commands
   
  for(int x= 0;x<20;x++){
        digitalWrite(relay1, HIGH);
        delay(200);
        digitalWrite(relay1, LOW);
        delay(200);
    }
 
  ping_GSM();
  
  Serial.println("ATE0\r");
  delay(100);
  Serial.println("AT+CMGF=1\r");
  delay(100);


}

int index = 0;

void loop()
{
  
  float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,Current=0.0;
  
  for (int x = A1; x < 150; x++){     
    Samples = Samples + AcsValue;  
    delay (3); 
    }
  
  AvgAcs=Samples/150.0;
  
  Current = (2.5 - (AvgAcs * (5.0 / 1024.0)) );


  value = analogRead(analogInput);
  vout = (value * 5.0) / 1024.0; 
  vin = ((vout / (R2/(R1+R2)))-4)+18;

  Power = (vin * Current);

  if (Current>=0.0) {
    digitalWrite(relay1, HIGH);
    delay(2000);
    Serial.print("AT+CMGS=\"");
    Serial.print(number);  
    Serial.println("\"\r");
    delay(500);
    Serial.println("PUMP WORKING");
    Serial.print("CURRENT=");
    Serial.print(Current);
    Serial.print("A");
    Serial.print(" VOLTAGE=");
    Serial.print(vin);
    Serial.print("V");
    Serial.print("  POWER=");
    Serial.print(Power);
    Serial.print("W");
    Serial.write(0x1a);
    }
   
  digitalWrite(relay1,LOW);
  delay(300000);
  
}
  
void ping_GSM() {
     Serial.println("ATE0\r");
     delay(500);
}


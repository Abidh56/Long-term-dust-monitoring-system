#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SimpleTimer.h>

char auth[] = "Your Auth";  //The authentication token send to your email 
//Your WIFI credentials
char ssid[] = "SSID";                             
char pass[] = "PASSWORD";

#define ESP8266_BAUD 115200    
ESP8266 wifi(&Serial);

int SensorPin = A5;
int led = 7;  
float SensorOut = 0;
float SensorVo = 0;
float Dust = 0;
float Voltage_noDust = 0.12;      //calibrate by changing this
float K = 0.5;                    //Sensitivity from the datasheet 0.5V/ugm/m^3
SimpleTimer timer;

void sendSensor(){  
  digitalWrite(led,LOW);                     //turning on the led
  delayMicroseconds(280);                    //wait for 0.28ms
  SensorOut = analogRead(SensorPin);         //read the sensor output voltage
  delayMicroseconds(50);                     //wait for 50us
  digitalWrite(led,HIGH);                    //turning off the led
  delayMicroseconds(9670);                   //wait for (10000-280-50)=9670us
  SensorVo = SensorOut*(5.0/1024);           //calculating sensor output voltage
  Dust = (SensorVo-Voltage_noDust)*100/K;    //calculating Dust density(ugm)/m^3
 
  if ( Dust < 0)
  {
    Dust = 0.00;
  }
  Blynk.virtualWrite(V1,Dust);  //sending data to the cloud
}

void setup()
{
 pinMode(led,OUTPUT);
 Serial.begin(115200);
 delay(10);
 Serial.begin(ESP8266_BAUD);
 delay(10);
 Blynk.begin(auth, wifi, ssid, pass);
 timer.setInterval(1000L, sendSensor);     //set timer interval of 1s
}

void loop()
{
 Blynk.run();
 timer.run();
}


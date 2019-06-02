// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

 RH_ASK driver(2000, 15, 13, 5);

#define DHTPIN 2     
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);

struct dataStruct{
  float hum;
  float temp;
  unsigned long counter;   
}myData;
byte tx_buf[sizeof(myData)] = {0};
void setup() 
{
    Serial.begin(9600); 
    Serial.println("DHTxx test!");
    if (!driver.init())
         Serial.println("init failed");
    dht.begin();
}

void loop() 
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    uint8_t h = dht.readHumidity();
    uint8_t t = dht.readTemperature();

    myData.hum=h;
    myData.temp=t;
    
    
    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h)) 
    {
        Serial.println("Failed to read from DHT");
    } 
    else 
    {


      memcpy(tx_buf, &myData, sizeof(myData) );
  byte zize=sizeof(myData);
  driver.send((uint8_t *)tx_buf, zize);

   // driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    myData.counter++;
    
        Serial.print("Humidity: "); 
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(t);
        Serial.println(" *C");
    }
    delay(1000);
}

// ask_receiver.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module

#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


 RH_ASK driver(2000, 15, 13, 5);

struct dataStruct{
  float hum;
  float temp;
  unsigned long counter;
   
}myData;


void setup()
{
    Serial.begin(9600);  // Debugging only
    if (!driver.init())
         Serial.println("init failed");

           if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}

void loop()
{
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

    if (driver.recv(buf, &buflen)) // Non-blocking
    {
  int i;

  // Message with a good checksum received, dump it.
  driver.printBuffer("Got:", buf, buflen);
        memcpy(&myData, buf, sizeof(myData));
        Serial.println("");
       
                Serial.print("hum: ");
        Serial.print(myData.hum);
       
       
                          Serial.print("  temp: ");
        Serial.print(myData.temp);
       
        Serial.print("  counter: ");
        Serial.println(myData.counter);
display.clearDisplay();
          display.setCursor(0,0);             // Start at top-left corner 
     display.setTextSize(2);             // Draw 2X-scale text
   display.setTextColor(WHITE);
   display.println("Temp="+String(int(myData.temp)-3)+"C");
   display.println("Hum="+String(int(myData.hum))+"%");
   display.setTextSize(1);     
   display.println("");
   display.println("Count="+String(myData.counter));
   display.println("Aleksandr Popov");
  display.display();
    }
}

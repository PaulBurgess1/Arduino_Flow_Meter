#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

int flowPin = 2;    //This is the input pin on the Arduino
double flowRate;    //This is the value we intend to calculate.
volatile int count; //This integer needs to be set as volatile to ensure it updates correctly during the interrupt process.

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  //OLED Begin
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Hello, world!");
  display.display();
  
  //Begin Flow Meter
  pinMode(flowPin, INPUT_PULLUP);           //Sets the pin as an input. INPUT_PULLUP preffered over INPUT.
  attachInterrupt(0, Flow, RISING);  //Configures interrupt 0 (pin 2 on the Arduino Uno) to run the function "Flow"

}
void loop() {
  // put your main code here, to run repeatedly:
  count = 0;      // Reset the counter so we start counting from 0 again
  interrupts();   //Enables interrupts on the Arduino
  delay (1000);   //Wait 1 second
  noInterrupts(); //Disable the interrupts on the Arduino

  //Start the math
  flowRate = (count * 2.25);        //Take counted pulses in the last second and multiply by 2.25mL
  flowRate = flowRate * 60;         //Convert seconds to minutes, giving you mL / Minute
  flowRate = flowRate / 1000;       //Convert mL to Liters, giving you Liters / Minute
  
  //Print to Display
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print(flowRate);
  display.println(" L");
  display.display();
  
}

void Flow()
{
   count++; //Every time this function is called, increment "count" by 1
}

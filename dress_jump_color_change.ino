#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
int numLEDS = 12;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numLEDS, 6, NEO_GRB + NEO_KHZ800);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);

float prev_z = 0;
bool jumping = false;

void setup() {
  pixels.begin();
  pixels.setBrightness(100);
  pixels.show();

  if (!accel.begin()) {
   // Serial.println("Oops ... unable to initialize the LSM303");
    while (1);
  }
}

void loop() {
  // Get the accelerometer data
  sensors_event_t event;
  accel.getEvent(&event);

  // Check if a jump is detected
  float z = event.acceleration.z;
  if(z < prev_z && z < -8.5) {
    jumping = true;
  }
  prev_z = z;

  // Change the NeoPixel LED color when a jump is detected
  if(jumping) {
    // Generate a random color
    uint32_t color = pixels.Color(random(3) * 127, random(3) * 127, random(3) * 127);  

    // Set the color of all LEDs in the string
    for(int i=0; i<numLEDS; i++) {
      pixels.setPixelColor(i, color);
    }

    pixels.show();
    jumping = false;
  }
  else {
    //pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    //pixels.show();
  }

  // Add a small delay to prevent overloading the board
  delay(100);
}

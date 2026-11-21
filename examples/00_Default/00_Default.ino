/*
  Example: Wavelit Default Template
  Board: Wavelit (ESP32-C3)

  Overview:
  - Use this sketch as a starting point for your own Wavelit projects.
  - Init() prepares Wavelit for use at startup.
  - Run() keeps the display and MicroLink app controls running.
  - Add custom text, images, or animations after Run() in loop().
*/

#include <wavelit.h>

Wavelit myWavelit;

void setup() {
  Serial.begin(115200);  // Set Serial baud rate to 115200. Ensure Tools/USB_CDC_On_Boot is enabled if using Serial.
  myWavelit.Init();      // Initializes Wavelit
}

void loop() {
  myWavelit.Run();
}

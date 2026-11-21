/*
  Example: Wavelit Custom Text Message
  Board: Wavelit (ESP32-C3)

  Overview:
  - Display a custom message using PrintText().
  - Change the text inside the quotation marks to show your own message.
  - You can also enter a message directly in the MicroLink app.
*/

#include <wavelit.h>

Wavelit myWavelit;

void setup() {
  Serial.begin(115200);  // Set Serial baud rate to 115200. Ensure Tools/USB_CDC_On_Boot is enabled if using Serial.
  myWavelit.Init();      // Initializes Wavelit
}

void loop() {
  myWavelit.Run();
  myWavelit.PrintText("Put Text Here..");
}

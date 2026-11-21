# Wavelit

<img align="right" width="280" src="https://microbots.io/cdn/shop/files/WAVELIT__11__EXTRA_-_Copy_1200x.jpg?v=1778499308" alt="Wavelit pixel display">

Wavelit is a tiny persistence-of-vision (POV) display by microbots. As the flap moves back and forth, a single row of RGB LEDs changes colour at precise points in each sweep. The result is an 11 × 23 pixel image that appears to float above the base.

Wavelit can show the time, display custom messages, and remind you when it’s time to take a break. Choose from 30+ built-in faces and animations and 20+ routines, or use the Wavelit Arduino library to code your own pixel animations.

[Explore Wavelit on microbots.io](https://microbots.io/products/wavelit)

<br clear="right">

## Getting started

1. To unpack Wavelit, gently lift it out of the box by its base and place it on a flat, stable surface. If you’d like to secure it in place, use the included double-sided adhesive pad under the base.
2. Connect the supplied USB-C cable to the back of the base to power it on.
3. Install [MicroLink for Android or iOS](https://microbots.io/pages/microlink) and enable Bluetooth on your phone.
4. Open MicroLink, tap **Add**, and select **Wavelit**. The app syncs the time and calibrates the display. Keep the flap clear until calibration finishes.
5. Choose your favourite faces, adjust the colours and brightness, and set your reminders.

Once setup is complete, Wavelit runs on USB power, even when your phone is disconnected.

## Gesture controls

| Gesture | Action |
| --- | --- |
| Swipe your hand beneath Wavelit | Switch to the next face or animation. |
| Gently tap the flap during an animation | Change its colour. |
| Gently tap the flap when a reminder appears | Snooze the reminder. |

## Arduino Library

The Wavelit Arduino library lets you display messages with `PrintText()`, show your own pixel art with `PrintImage()`, and loop through multiple images to create animations. You can also enter text directly in the MicroLink app.

For a still image, resize your PNG to **23 × 11 pixels**. For scrolling artwork, keep it **11 pixels high** and make it wider than 23 pixels. Wavelit automatically scrolls the image across the display. Convert each PNG to a 256-colour `uint8_t` array using the [Digole Picture to C converter](https://www.digole.com/tools/PicturetoC_Hex_converter.php). Select **RGB332** (`RRR GGG BB`), which uses one byte per pixel.

The library includes example sketches for text, pixel art, and scrolling animations that you can adapt for your own projects.

## Attribution & Third-Party Components

The **Wavelit Arduino library** was developed by **microbots**. It uses two third-party libraries.

| Library | Role in Wavelit |
| --- | --- |
| **BLEOTA** | Handles over-the-air firmware updates via Bluetooth Low Energy (BLE). |
| **Adafruit_NeoPixel** | Controls the RGB LEDs on the moving flap. |

Each third-party library is distributed under its own licence. See the licence included with each library for details.

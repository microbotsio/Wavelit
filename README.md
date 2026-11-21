# Wavelit

<img align="right" width="280" src="https://microbots.io/cdn/shop/files/WAVELIT__11__EXTRA_-_Copy_1200x.jpg?v=1778499308" alt="Wavelit pixel display">

Wavelit is a tiny persistence-of-vision (POV) display by microbots. As the flap moves back and forth, a single row of RGB LEDs changes colour at precise points in each sweep. The result is an 11 × 23 pixel image that appears to float above the base.

Wavelit can show the time, display custom messages, and remind you when it’s time to take a break. Choose from 30+ built-in faces and animations and 20+ routines, or use the Wavelit Arduino library to code your own pixel animations.

[Explore Wavelit on microbots.io](https://microbots.io/products/wavelit)

<br clear="right">

## Getting started

1. Lift Wavelit by its base and place it on a flat, stable surface.
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

## Custom animations with Arduino

The Wavelit Arduino library lets you create animations for the 11 × 23 pixel display. For a first project, try animating a travelling wave. Draw a sine wave across the display, then shift it a little to one side with each frame.

## Attribution & Third-Party Components

The **Wavelit Arduino library** was developed by **microbots**. It uses two third-party libraries.

| Library | Role in Wavelit |
| --- | --- |
| **BLEOTA** | Handles over-the-air firmware updates via Bluetooth Low Energy (BLE). |
| **Adafruit_NeoPixel** | Controls the RGB LEDs on the moving flap. |

Each third-party library is distributed under its own licence. See the licence included with each library for details.

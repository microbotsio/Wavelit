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

## Custom animations with Arduino

The Wavelit Arduino library lets you create animations for the 11 × 23 pixel display and includes several example sketches to help you get started.
## Custom text, images and animations with Arduino

The Wavelit Arduino library lets you display messages, draw your own pixel art, and loop through animation frames on the 23 × 11 pixel display. The library includes an example sketch for each of these.

### Display a text message

You can enter a message in the MicroLink app or display one directly from an Arduino sketch using `PrintText()`. To try it, open the `Custom_Text_Message` example and change the text inside the quotation marks.

### Display your own image

The `Print_Logo` example uses `PrintImage()` to display a Superman logo. The original PNG is included in the examples folder so you can see how the artwork looks at the display’s native resolution.

1. Create or resize your PNG to **23 pixels wide × 11 pixels high**.
2. Upload it to the [Digole Picture to C converter](https://www.digole.com/tools/PicturetoC_Hex_converter.php) and choose the **256-colour** output. Each pixel uses one byte in RGB332 format (`RRR GGG BB`), giving you **253 bytes** for the full image.
3. Open `Print_Logo` and replace the values inside the `superman_logo` array with the bytes from the converter. Keep the array name so the existing `PrintImage()` call displays your artwork.
4. Upload the sketch to Wavelit.

### Loop and scroll an animation

The `Print_Animation` example alternates between two Space Invader frames and repeats the sequence. Each frame is **46 × 11 pixels**, twice the width of the display. Wavelit automatically scrolls the wider artwork across the display while the frames alternate.

To try it with your own artwork, draw two PNGs at **46 × 11 pixels** and change one detail between them, such as a character’s legs. Convert each PNG separately using the same 256-colour setting, then replace the two frame arrays in `Print_Animation`. Each frame contains **506 bytes**. Upload the sketch to see your character move as it scrolls.

## Attribution & Third-Party Components

The **Wavelit Arduino library** was developed by **microbots**. It uses two third-party libraries.

| Library | Role in Wavelit |
| --- | --- |
| **BLEOTA** | Handles over-the-air firmware updates via Bluetooth Low Energy (BLE). |
| **Adafruit_NeoPixel** | Controls the RGB LEDs on the moving flap. |

Each third-party library is distributed under its own licence. See the licence included with each library for details.

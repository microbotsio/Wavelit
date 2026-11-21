#ifndef WAVELIT_BLE_H
#define WAVELIT_BLE_H

#include <Arduino.h>

#define SERVICE_UUID "b2345678-1234-1234-1234-123456789012"
#define CHAR_TIME_UUID "dcba0001-dcba-4321-dcba-432123456789"
#define CHAR_FACE_ENABLE_UUID "dcba0002-dcba-4321-dcba-432123456789"
#define CHAR_FACE_TEXT_UUID "dcba0003-dcba-4321-dcba-432123456789"
#define CHAR_ANIMATION_FACES_COLORS_UUID "dcba0006-dcba-4321-dcba-432123456789"
#define CHAR_FACE_STOPWATCH_UUID "dcba0005-dcba-4321-dcba-432123456789"
#define CHAR_FACE_CLOCK_UUID "dcba0007-dcba-4321-dcba-432123456789"
#define CHAR_SETTINGS_UUID "dcba0009-dcba-4321-dcba-432123456789"
#define CHAR_REMINDER_RX_UUID "dcba0008-dcba-4321-dcba-432123456789"
#define CHAR_READ_REMINDER_UUID "dcba0010-dcba-4321-dcba-432123456789"

void ColorUpdate(void);
void Face_Sync(void);
void Init_Sync(void);
void BLE_Init(void);
void FactoryReset(void);

#endif
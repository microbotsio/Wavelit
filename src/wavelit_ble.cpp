#include "wavelit.h"
#include "wavelit_ble.h"
#include "wavelit_faces.h"
#include "wavelit_reminders.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <BLEOTA.h>
#include <EEPROM.h>

BLECharacteristic *pTimeCharacteristic = NULL;
BLECharacteristic *pFaceEnableCharacteristic = NULL;
BLECharacteristic *pTextFaceCharacteristic = NULL;
BLECharacteristic *pStopwatchFaceCharacteristic = NULL;
BLECharacteristic *pClockFaceCharacteristic = NULL;
BLECharacteristic *pColorCharacteristic = NULL;
BLECharacteristic *pImageFaceCharacteristic = NULL;
BLECharacteristic *pReminderRxCharacteristic = NULL;
BLECharacteristic *pReminderReadCharacteristic = NULL;
BLECharacteristic *pSettingsCharacteristic = NULL;

extern uint16_t image_array[11][23];
extern uint8_t image_num;
extern bool sw_enable_flag, EEP_write_flag, swtimer_flag;

uint8_t dataBuffer[REMINDER_BUFFER_SIZE];
uint8_t image_color[31] = { 7U };
char textfaceText[64] = { 'H', 'E', 'L', 'L', 'O', ' ', 'W', 'O', 'R', 'L', 'D', '\0' };

bool ota_flag = 0, init_learnflag = 0, TuneAdjustFlag = 0, counter_format = 0, counter_type = 0, text_change = 1, time_change = 1, torch_type = 0;
uint8_t AutoCalibrateInProgress = 0, first_connect = 0, standby_minute = 0, standby_seconds = 0, auto_calibrate_flag = 0, save_alignment_flag = 0, auto_swipe_last = 0, auto_swipe = 0, shutdown_timer = 30, dark_shutdown = 0, autoshutdown_minutes = 0, autoshutdown_hour = 0, autoshutdown_flag = 0, autotune_flag_last = 0, autotune_flag = 0, led_brightness = 200, counter_h_limit = 0, counter_l_limit = 0, time_day = 0, time_hour = 0, time_minute = 0, time_seconds = 0;
uint16_t currentIndex = 0, expectedPacketNumber = 0, tune_value_last = 0;
uint32_t face_enable = 395323;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) override {
    Serial.println(">> Wavelit: Connected to App");
    delay(500);
    Init_Sync();
  }

  void onDisconnect(BLEServer *pServer) override {
    Serial.println(">> Wavelit: Disconnected from App");
    delay(500);                     // Small delay to ensure proper disconnection before re-advertising
    BLEDevice::startAdvertising();  // Restart advertising so new devices can connect
    Serial.println(">> Wavelit: Bluetooth Advertising..");
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String value = pCharacteristic->getValue();  // Use Arduino String
    standby_minute = 0;
    standby_seconds = 0;

    if (pCharacteristic == pTimeCharacteristic) {
      // Expecting 3 bytes: HH, MM, SS
      if (value.length() == 4) {
        time_day = (static_cast<uint8_t>(value[0]));
        time_hour = static_cast<uint8_t>(value[1]);
        time_minute = static_cast<uint8_t>(value[2]);
        time_seconds = static_cast<uint8_t>(value[3]);
        time_change = 1;
      }
    } else if (pCharacteristic == pFaceEnableCharacteristic) {
      if (value.length() >= 2) {
        image_num = value[0] + 1;
        uint8_t face1 = value[1];
        uint8_t face2 = value[2];
        uint8_t face3 = value[3];
        uint8_t face4 = value[4];
        face_enable = face1 + (face2 << 8) + (face3 << 16) + (face4 << 24);

        if (face_enable == 0) {
          image_num = FACE_NUM_LOGO;
          face_enable = FACE_ID_LOGO;
        }
        ColorSet(image_color[image_num]);

        EEPROM.put(EEPROM_IMAGE_NUM, image_num);
        EEPROM.put(EEPROM_FACE_EN1, face1);
        EEPROM.put(EEPROM_FACE_EN2, face2);
        EEPROM.put(EEPROM_FACE_EN3, face3);
        EEPROM.put(EEPROM_FACE_EN4, face4);
        EEP_write_flag = 1;
      }
    } else if (pCharacteristic == pTextFaceCharacteristic) {
      if (value.length() > 1) {
        int a = 0;
        text_change = 1;
        for (a = 0; a <= 64; a++) {
          textfaceText[a] = '\0';
        }
        image_color[FACE_NUM_TEXT] = value[0];
        for (a = 1; a < value.length(); a++) {
          textfaceText[a - 1] = (char)value[a];
        }
        textfaceText[a + 1] = '\0';
        ColorSet(image_color[FACE_NUM_TEXT]);
        ColorUpdate();
      }
    } else if (pCharacteristic == pColorCharacteristic) {
      if (value.length() == 7) {
        image_color[FACE_NUM_WAVE] = value[0];
        image_color[FACE_NUM_HEART] = value[1];
        image_color[FACE_NUM_PI] = value[2];
        image_color[FACE_NUM_LIGHTNING] = value[3];
        image_color[FACE_NUM_CROSS] = value[4];
        image_color[FACE_NUM_INFINITY] = value[5];
        image_color[FACE_NUM_BRB] = value[6];
        ColorSet(image_color[image_num]);
        ColorUpdate();
      }
    } else if (pCharacteristic == pStopwatchFaceCharacteristic) {
      if (value.length() == 5) {
        swtimer_flag = 1;
        image_color[FACE_NUM_STOPWATCH] = value[0];
        ColorSet(image_color[FACE_NUM_STOPWATCH]);
        ColorUpdate();

        counter_format = value[1];   // 2nd byte is isHHMM (1 for HH:MM, 0 for MM:SS)
        counter_type = value[2];     // 3rd byte is isCountdown (1 for Countdown, 0 for Stopwatch)
        counter_h_limit = value[3];  // 4th byte is countdownTime1 (Hours or Minutes)
        counter_l_limit = value[4];  // 5th byte is countdownTime2 (Minutes or Seconds)

        if ((counter_type == 1) && ((counter_h_limit == 0) && (counter_l_limit == 0))) {
          sw_enable_flag = 0;
        } else {
          sw_enable_flag = 1;
        }
      }
    } else if (pCharacteristic == pClockFaceCharacteristic) {
      if (value.length() == 1) {
        image_color[FACE_NUM_TIME] = value[0];
        ColorSet(image_color[FACE_NUM_TIME]);
        ColorUpdate();
      }
    } else if (pCharacteristic == pSettingsCharacteristic) {
      if (value.length() == 14) {
        autotune_flag = static_cast<uint8_t>(value[0]);  //0xFF or 0x00
        if (autotune_flag != autotune_flag_last) {
          EEPROM.put(EEPROM_FINE_TUNE, autotune_flag);
          EEP_write_flag = 1;
        }
        autotune_flag_last = autotune_flag;

        int16_t tune_value = static_cast<int16_t>(value[1] | (value[2] << 8));
        if (tune_value_last != tune_value) {
          tune_value_last = tune_value;
          TuneAdjustFlag = 1;
        }
        led_brightness = static_cast<uint8_t>(value[5]) * 2.54;

        save_alignment_flag = static_cast<uint8_t>(value[3]);

        if (auto_calibrate_flag == 0) {  //wait for calibration to be done, before rewrite
          auto_calibrate_flag = static_cast<uint8_t>(value[4]);

          if (auto_calibrate_flag == 0xFF) {
            AutoCalibrateInProgress = 0xFF;
            Init_Sync();
          }
        }

        autoshutdown_flag = static_cast<uint8_t>(value[6]);  //0xFF or 0x00
        autoshutdown_hour = static_cast<uint8_t>(value[7]);
        autoshutdown_minutes = static_cast<uint8_t>(value[8]);
        dark_shutdown = static_cast<uint8_t>(value[9]);  //0xFF or 0x00
        shutdown_timer = static_cast<uint8_t>(value[10]);

        uint8_t factory_reset_flag = static_cast<uint8_t>(value[11]);

        if (factory_reset_flag != 0) {
          FactoryReset();
        }

        uint8_t calibration_inprogress = static_cast<uint8_t>(value[12]);  //clear
        auto_swipe = static_cast<uint8_t>(value[13]);
        if (auto_swipe != auto_swipe_last) {
          EEPROM.put(EEPROM_AUTO_SWIPE, auto_swipe);
          EEP_write_flag = 1;
        }
        auto_swipe_last = auto_swipe;
      }
    } else if (pCharacteristic == pReminderRxCharacteristic) {
      //10 - Start command
      //12 - Data command
      //11 - End command
      //13- Start End Command - 2 packets only

      if (value.length() <= 2) {
        return;
      }

      uint8_t packetNumber = value[0];
      uint8_t packetCmd = value[1];

      if (packetCmd == 0x10 || packetCmd == 0x13) {
        currentIndex = 0;
        expectedPacketNumber = 0;
        memset(dataBuffer, 0, REMINDER_BUFFER_SIZE);
      }

      if (packetNumber != expectedPacketNumber) {
        //Packet out of sequence or duplicate. Ignoring.
        return;
      }

      for (uint16_t i = 2; i < value.length(); i++) {
        if (currentIndex < REMINDER_BUFFER_SIZE) {
          dataBuffer[currentIndex] = value[i];
          currentIndex++;
        }
      }

      if (packetCmd == 0x11 || packetCmd == 0x13) {
        //End of data transmission. Full array received
        Save_Reminder();
      } else {
        expectedPacketNumber++;
      }
    } else {
      //Skip
    }
  }
};

class OTACallbacks : public BLEOTACallbacks {
public:
  //This callback method is invoked just before the APP OTA update begins.
  void beforeStartOTA() {
    Serial.println(">> Wavelit: Downloading New Update");
    ota_flag = 1;
  }
  //This callback method is invoked just before the SPIFFS OTA update begins.
  void beforeStartSPIFFS() {
  }
  //This callback method is invoked just after the update completes.
  void afterStop() {
    Serial.println(">> Wavelit: New Update Complete");
    ota_flag = 0;
  }
  //This callback method is invoked just after the update abort.
  void afterAbort() {
    Serial.println(">> Wavelit: New Update Aborted");
  }
};

void BLE_Init(void) {
  // Initialize BLE
  BLEDevice::init(DEVICE_NAME);
  BLEServer *bleServer = BLEDevice::createServer();
  bleServer->setCallbacks(new MyServerCallbacks());

  BLEService *bleService = bleServer->createService(BLEUUID(SERVICE_UUID), 30, 0);

  pTimeCharacteristic = bleService->createCharacteristic(CHAR_TIME_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pTimeCharacteristic->addDescriptor(new BLE2902());    /*Add description to client configuration*/
  pTimeCharacteristic->setCallbacks(new MyCallbacks()); /*Set callback functions using the MyCallbacks class*/

  pFaceEnableCharacteristic = bleService->createCharacteristic(CHAR_FACE_ENABLE_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
  pFaceEnableCharacteristic->addDescriptor(new BLE2902());
  pFaceEnableCharacteristic->setCallbacks(new MyCallbacks());

  pTextFaceCharacteristic = bleService->createCharacteristic(CHAR_FACE_TEXT_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pTextFaceCharacteristic->addDescriptor(new BLE2902());
  pTextFaceCharacteristic->setCallbacks(new MyCallbacks());

  pColorCharacteristic = bleService->createCharacteristic(CHAR_ANIMATION_FACES_COLORS_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
  pColorCharacteristic->addDescriptor(new BLE2902());
  pColorCharacteristic->setCallbacks(new MyCallbacks());

  pStopwatchFaceCharacteristic = bleService->createCharacteristic(CHAR_FACE_STOPWATCH_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pStopwatchFaceCharacteristic->addDescriptor(new BLE2902());
  pStopwatchFaceCharacteristic->setCallbacks(new MyCallbacks());

  pClockFaceCharacteristic = bleService->createCharacteristic(CHAR_FACE_CLOCK_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pClockFaceCharacteristic->addDescriptor(new BLE2902());
  pClockFaceCharacteristic->setCallbacks(new MyCallbacks());

  pSettingsCharacteristic = bleService->createCharacteristic(CHAR_SETTINGS_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
  pSettingsCharacteristic->addDescriptor(new BLE2902());
  pSettingsCharacteristic->setCallbacks(new MyCallbacks());

  // reminder Characteristic
  pReminderRxCharacteristic = bleService->createCharacteristic(CHAR_REMINDER_RX_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pReminderRxCharacteristic->addDescriptor(new BLE2902());
  pReminderRxCharacteristic->setCallbacks(new MyCallbacks());

  // reminder read Characteristic
  pReminderReadCharacteristic = bleService->createCharacteristic(CHAR_READ_REMINDER_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pReminderReadCharacteristic->addDescriptor(new BLE2902());
  pReminderReadCharacteristic->setCallbacks(new MyCallbacks());

  // Add OTA Service
  BLEOTA.begin(bleServer);
  BLEOTA.setCallbacks(new OTACallbacks());
  BLEOTA.setFWVersion(SW_VERSION);
  BLEOTA.setHWVersion(HW_VERSION);
  BLEOTA.setManufactuer(MANUFACTURER);

  BLEOTA.init();

  bleService->start();  // Start the service and begin advertising

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLEOTA.getBLEOTAuuid());
  pAdvertising->addServiceUUID(SERVICE_UUID);
  BLEDevice::startAdvertising();
}

void Face_Sync(void) {
  uint8_t tempchar[5] = { 0 };
  if (image_num < FACE_NUM_REMINDER) {
    tempchar[0] = image_num - 1;
  } else {
    tempchar[0] = 0;
  }
  tempchar[1] = (face_enable & 0xFF);
  tempchar[2] = ((face_enable >> 8) & 0xFF);
  tempchar[3] = ((face_enable >> 16) & 0xFF);
  tempchar[4] = ((face_enable >> 24) & 0xFF);
  pFaceEnableCharacteristic->setValue((uint8_t *)&tempchar, sizeof(tempchar));
  pFaceEnableCharacteristic->notify();
}

void Init_Sync(void) {
  uint8_t colorchar[7] = { 0 };
  uint8_t settingschar[14] = { 0 };
  uint8_t stopwatchchar[5] = { 0 };

  ColorSet(image_color[image_num]);

  Face_Sync();

  colorchar[0] = image_color[FACE_NUM_WAVE];
  colorchar[1] = image_color[FACE_NUM_HEART];
  colorchar[2] = image_color[FACE_NUM_PI];
  colorchar[3] = image_color[FACE_NUM_LIGHTNING];
  colorchar[4] = image_color[FACE_NUM_CROSS];
  colorchar[5] = image_color[FACE_NUM_INFINITY];
  colorchar[6] = image_color[FACE_NUM_BRB];

  pColorCharacteristic->setValue((uint8_t *)&colorchar, sizeof(colorchar));
  pColorCharacteristic->notify();

  pTextFaceCharacteristic->setValue(&image_color[FACE_NUM_TEXT], 1);
  pTextFaceCharacteristic->notify();

  stopwatchchar[0] = image_color[FACE_NUM_STOPWATCH];
  stopwatchchar[1] = counter_format;
  stopwatchchar[2] = counter_type;
  stopwatchchar[3] = counter_h_limit;
  stopwatchchar[4] = counter_l_limit;

  pStopwatchFaceCharacteristic->setValue((uint8_t *)&stopwatchchar, sizeof(stopwatchchar));
  pStopwatchFaceCharacteristic->notify();

  pClockFaceCharacteristic->setValue(&image_color[FACE_NUM_TIME], 1);
  pClockFaceCharacteristic->notify();

  settingschar[0] = autotune_flag;
  settingschar[1] = tune_value_last & 0xFF;
  settingschar[2] = (tune_value_last >> 8) & 0xFF;
  settingschar[3] = 0;
  settingschar[4] = 0;
  settingschar[5] = (led_brightness / 2.54);
  settingschar[6] = autoshutdown_flag;
  settingschar[7] = autoshutdown_hour;
  settingschar[8] = autoshutdown_minutes;
  settingschar[9] = dark_shutdown;
  settingschar[10] = shutdown_timer;
  settingschar[11] = 0;
  settingschar[12] = AutoCalibrateInProgress;
  settingschar[13] = auto_swipe;

  pSettingsCharacteristic->setValue((uint8_t *)&settingschar, sizeof(settingschar));
  pSettingsCharacteristic->notify();

  if (first_connect != FIRST_CONNECT_KEY) {
    first_connect = FIRST_CONNECT_KEY;
    init_learnflag = 0;
    EEP_write_flag = 1;
    EEPROM.put(EEPROM_FIRST_CONNECT, first_connect);
  }
}

void ColorUpdate(void) {
  EEPROM.put(EEPROM_COL_WAVE, image_color[FACE_NUM_WAVE]);
  EEPROM.put(EEPROM_COL_HEART, image_color[FACE_NUM_HEART]);
  EEPROM.put(EEPROM_COL_PI, image_color[FACE_NUM_PI]);
  EEPROM.put(EEPROM_COL_LIGHTNING, image_color[FACE_NUM_LIGHTNING]);
  EEPROM.put(EEPROM_COL_CROSS, image_color[FACE_NUM_CROSS]);
  EEPROM.put(EEPROM_COL_INFINITY, image_color[FACE_NUM_INFINITY]);
  EEPROM.put(EEPROM_COL_STOPWATCH, image_color[FACE_NUM_STOPWATCH]);
  EEPROM.put(EEPROM_COL_TIME, image_color[FACE_NUM_TIME]);
  EEPROM.put(EEPROM_COL_TEXT, image_color[FACE_NUM_TEXT]);
  EEPROM.put(EEPROM_COL_BRB, image_color[FACE_NUM_BRB]);
  EEP_write_flag = 1;
}

void FactoryReset(void) {
  ClearDisplay();
  for (uint8_t e = 0; e < EEPROM_SIZE; e++) {
    EEPROM.put(e, 0xFF);
  }
  EEPROM.commit();
  delay(100);
  esp_restart();
}

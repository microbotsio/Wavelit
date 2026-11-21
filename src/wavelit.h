#ifndef WAVELIT_H
#define WAVELIT_H

#include <Arduino.h>
#include <Wire.h>

#define SW_VERSION "1.0.0"
#define HW_VERSION "4.9"
#define MANUFACTURER "Microbots"
#define DEVICE_NAME "wavelit"

#define LED_PIN 10U
#define LED_DEFAULT_BRIGHTNESS 10U

#define DRIVE_TIMER 8
#define DRIVE_BASE_FREQ 20000

#define COIL_PIN_1 7
#define COIL_PIN_2 6

#define ROW_NUM 11U
#define COL_NUM 23U

#define VCNL4040_ADDRESS 0x60
#define VCNL4040_ALS_CONF_REG 0x00
#define VCNL4040_PS_CONF1_2_REG 0x03
#define VCNL4040_PS_CONF3_MS_REG 0x04
#define VCNL4040_PROX_REG 0x08
#define VCNL4040_AMBIENT_REG 0x09
#define VCNL4040_WHITE_REG 0x0A

#define TOUCH_THRESHOLD 1000U
#define PROX_THRESHOLD 400U
#define PROX_OFFSET 50U
#define LIGHT_THRESHOLD 1U
#define AVRG_FILTER_SIZE 8U

#define SCORE_TIMING 25U
#define SCORE_INDEX 11U
//#define SCORE_SIZE 5U
#define ADC_ARRAY_SIZE 10U

#define EEPROM_SIZE 24
#define EEPROM_AUTO_TUNE 0
#define EEPROM_OFFSET_TUNE 2
#define EEPROM_IMAGE_NUM 4
#define EEPROM_FACE_EN1 5
#define EEPROM_FACE_EN2 6
#define EEPROM_FACE_EN3 7
#define EEPROM_FACE_EN4 8
#define EEPROM_COL_WAVE 9
#define EEPROM_COL_HEART 10
#define EEPROM_COL_PI 11
#define EEPROM_COL_LIGHTNING 12
#define EEPROM_COL_CROSS 13
#define EEPROM_COL_INFINITY 14
#define EEPROM_COL_STOPWATCH 15
#define EEPROM_COL_TIME 16
#define EEPROM_COL_TEXT 17
#define EEPROM_COL_BRB 18
#define EEPROM_FINE_TUNE 19
#define EEPROM_AUTO_SWIPE 20
#define EEPROM_FIRST_CONNECT 21
#define FIRST_CONNECT_KEY 0xC

const uint16_t init_tune_adjust[SCORE_INDEX] = { 1600U, 1700U, 1800U, 1900U, 2000U, 2100U, 2200U, 2300U, 2400U, 2500U, 2600U };

class Wavelit {
private:
  void Flip();
  void Sleep_Handler();
  void Image_Brightness(uint8_t level);
  void Image_Handler();
  void Image_Swipe();
  void Hall_Handler();
  void IR_Handler();
  void Init_Tone();
  void Time_Handler();
  void Alarm();

  bool Light_Init();
  void Light_Reset();
  uint16_t Light_ProximityRead();
  uint16_t Light_WhiteRead();
  uint16_t Light_AmbientRead();
  void Light_Read();

  uint8_t _ImageArray[2024] = { 0 };
  uint16_t _light_data[3] = { 0 };
  uint8_t _i2c_write_array[10] = { 0 };
  uint8_t _i2c_read_array[10] = { 0 };
  uint32_t _tune_score = 0, _high_score = 0, _low_score = 0, _current_score = 0;
  bool _ir_flag = 0, _tune_melody = 0, _calibration = 0;
  uint16_t _ImageSize = 0, _first_connect_timer = 0, _tune_timer = 0, _tune_bestscore = 0, _ir_timer = 0, _prox_last = 0, _score_timer = 0, _adc_val_last = 0, _max_score = 0, _learn_value = 0, _val = 0, _timerx = 0, _timery = 1450, _val_last = 0U, _swipe_timer = 0U;
  uint8_t _print_enable = 0, _auto_swipe_timer = 0, _finetune_freq = 0, _finetune_timer = 0, _tune_index = 0, _level_last = 0, _score_size = 0, _adc_counter = 0, _i2c_write_size = 0, _time_timer = 0;
  uint16_t _adc_samples[ADC_ARRAY_SIZE];

public:
  Wavelit();
  void Init();
  void Run();
  void PrintText(char word[]);
  void PrintImage(const uint8_t* arr1, uint16_t size);
};

#endif
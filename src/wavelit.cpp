#include "wavelit.h"
#include "wavelit_faces.h"
#include "wavelit_ble.h"
#include "wavelit_reminders.h"
#include <BLEOTA.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>


Adafruit_NeoPixel Wavelit_Pixels = Adafruit_NeoPixel(23, 1, NEO_GRB + NEO_KHZ800);

bool I2CRead(uint8_t add, uint8_t *buffer, size_t size);
bool I2CWrite(uint8_t add, uint8_t *buffer, size_t size);

hw_timer_t *cctimer = NULL;
hw_timer_t *clocktimer = NULL;

size_t _maxBufferSize = 32;
size_t maxBufferSize();

bool EEP_write_flag = 0, sleep_flag = 0, polarity_flap = 0, first_time_flag = 0, w_timeflag = 1, swtimer_flag = 0, second_timeflag = 1, second_flip = 0;
uint8_t light_index = 0, swtime_seconds = 0, swtime_minute = 0, swtime_hour = 0;

uint8_t image_row = 0U, image_num = FACE_NUM_WAVE;
uint16_t light_avrg[AVRG_FILTER_SIZE] = { 0 };
uint16_t prox_avrg[AVRG_FILTER_SIZE] = { 0 };

extern bool ota_flag, reminder_flag, init_learnflag, play_tune, TuneAdjustFlag, counter_format, counter_type, time_change;
extern uint8_t first_connect, reminder_index, auto_calibrate_flag, save_alignment_flag, standby_minute, standby_seconds, autotune_flag, auto_swipe_last, autotune_flag_last, AutoCalibrateInProgress, led_brightness, autoshutdown_flag, autoshutdown_hour, autoshutdown_minutes, dark_shutdown, shutdown_timer, counter_h_limit, counter_l_limit, time_day, time_hour, time_minute, time_seconds, gradient_speed, image_column, auto_swipe, textfaceColor;
extern uint16_t hsv_offset, gradient_max, gradient_min, image_array[11][23];
extern int16_t tune_value_last;
extern uint32_t face_enable;
extern char textfaceText[];
extern uint8_t image_color[31];

void IRAM_ATTR w_Timer() {
  w_timeflag = 1;

  image_row++;
  if ((image_row >= 11U) && (!sleep_flag)) {
    image_row = 0U;
    polarity_flap = !polarity_flap;
    if (polarity_flap) {
      gpio_set_level((gpio_num_t)COIL_PIN_1, 1);  // HIGH
      gpio_set_level((gpio_num_t)COIL_PIN_2, 0);  // LOW
    } else {
      gpio_set_level((gpio_num_t)COIL_PIN_1, 0);  // LOW
      gpio_set_level((gpio_num_t)COIL_PIN_2, 1);  // HIGH
    }
  }
}

void IRAM_ATTR clock_Timer() {
  second_timeflag = 1;
}

Wavelit::Wavelit() {
}

void Wavelit::Init() {
  uint8_t image_column = 0;

  uint8_t light_timer = 0;

  delay(1300); /*Uart Setup delay*/

  Serial.println(">> Wavelit: Booting Software v" SW_VERSION);

  /*Set the initial Coil Output to low*/
  pinMode(COIL_PIN_1, OUTPUT);
  pinMode(COIL_PIN_2, OUTPUT);
  gpio_set_level((gpio_num_t)COIL_PIN_1, 0);  // LOW
  gpio_set_level((gpio_num_t)COIL_PIN_2, 1);  // HIGH
  delay(500);

  while (Light_Init() == 1) {
    delay(10);
    Light_Reset();
    if (light_timer < 50U) {
      light_timer++;
    } else {
      Serial.println(">> Error: Light Sensor not found");
      esp_restart();
    }
  }

  Wavelit_Pixels.setBrightness(254);
  _level_last = led_brightness + 1;
  Wavelit_Pixels.begin();

  Init_Tone();

  Wavelit_Pixels.clear();
  Wavelit_Pixels.show();

  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println(">> Wavelit: Failed to initialize Auto-Tune EEPROM Region");
  }

  EEPROM.get(EEPROM_FIRST_CONNECT, first_connect);  // Read tuning value
  EEPROM.get(EEPROM_AUTO_TUNE, _learn_value);       // Read tuning value
  EEPROM.get(EEPROM_OFFSET_TUNE, tune_value_last);  // Read tuning value

  if (first_connect == FIRST_CONNECT_KEY) {
    if ((_learn_value > 2600) || (_learn_value < 1600)) {
      _learn_value = 1600;
      init_learnflag = 0;  //Frequency Learning Required
    } else {
      init_learnflag = 1;  //Learning Frequency Complete
    }
  } else {
    init_learnflag = 1;  //Skip Frequency Learning
    _learn_value = 1600;
  }

  EEPROM.get(EEPROM_AUTO_SWIPE, auto_swipe);
  EEPROM.get(EEPROM_FINE_TUNE, autotune_flag);
  autotune_flag_last = autotune_flag;
  auto_swipe_last = auto_swipe;
  uint8_t face1 = 0;
  uint8_t face2 = 0;
  uint8_t face3 = 0;
  uint8_t face4 = 0;

  EEPROM.get(EEPROM_FACE_EN1, face1);
  EEPROM.get(EEPROM_FACE_EN2, face2);
  EEPROM.get(EEPROM_FACE_EN3, face3);
  EEPROM.get(EEPROM_FACE_EN4, face4);
  EEPROM.get(EEPROM_IMAGE_NUM, image_num);

  if (((face1 == 255) && (face2 == 255) && (face3 == 255) && (face4 == 255)) || (image_num == 255)) {
    face_enable = 387186747;
    image_num = FACE_NUM_WAVE;
    EEPROM.put(EEPROM_IMAGE_NUM, image_num);
    EEPROM.put(EEPROM_FACE_EN1, face1);
    EEPROM.put(EEPROM_FACE_EN2, face2);
    EEPROM.put(EEPROM_FACE_EN3, face3);
    EEPROM.put(EEPROM_FACE_EN4, face4);
    EEP_write_flag = 1;
  } else {
    face_enable = face1 + (face2 << 8) + (face3 << 16) + (face4 << 24);
    if ((image_num >= FACE_NUM_REMINDER) && (image_num == 0)) {
      image_num = FACE_NUM_WAVE;
    }
  }

  EEPROM.get(EEPROM_COL_WAVE, image_color[FACE_NUM_WAVE]);
  EEPROM.get(EEPROM_COL_HEART, image_color[FACE_NUM_HEART]);
  EEPROM.get(EEPROM_COL_PI, image_color[FACE_NUM_PI]);
  EEPROM.get(EEPROM_COL_LIGHTNING, image_color[FACE_NUM_LIGHTNING]);
  EEPROM.get(EEPROM_COL_CROSS, image_color[FACE_NUM_CROSS]);
  EEPROM.get(EEPROM_COL_INFINITY, image_color[FACE_NUM_INFINITY]);
  EEPROM.get(EEPROM_COL_STOPWATCH, image_color[FACE_NUM_STOPWATCH]);
  EEPROM.get(EEPROM_COL_TIME, image_color[FACE_NUM_TIME]);
  EEPROM.get(EEPROM_COL_TEXT, image_color[FACE_NUM_TEXT]);
  EEPROM.get(EEPROM_COL_BRB, image_color[FACE_NUM_BRB]);

  for (uint8_t ccheck = 0; ccheck < FACE_NUM_REMINDER; ccheck++) {
    if (image_color[ccheck] > 11U) {
      image_color[ccheck] = 7U;
    }
  }

  // pinMode(3, INPUT); /*Set up Battery Voltage Read pin*/
  pinMode(4, INPUT); /*Set up Battery Voltage Read pin*/
  analogSetAttenuation(ADC_11db);

  delay(100);
  _val = analogRead(4);

  cctimer = timerBegin(1000000);           /*Set timer frequency to 1Mhz*/
  timerAttachInterrupt(cctimer, &w_Timer); /*Attach interrupt*/
  timerAlarm(cctimer, (_learn_value + tune_value_last), true, 0);
  _tune_bestscore = _learn_value + tune_value_last;

  clocktimer = timerBegin(1000000);               /*Set timer frequency to 1Mhz*/
  timerAttachInterrupt(clocktimer, &clock_Timer); /*Attach interrupt*/
  timerAlarm(clocktimer, 1000000, true, 0);       /*Set default timmer to trigger every 1s with auto-reload*/

  _val_last = _val;

  for (uint8_t ii = 0; ii < ADC_ARRAY_SIZE; ii++) {
    _adc_samples[ii] = 3521;
  }

  Light_Read();
  _prox_last = Light_ProximityRead();

  BLE_Init();


  if (first_connect != FIRST_CONNECT_KEY) {
    //App never connected - set wave as default
    image_num = FACE_NUM_WAVE;
    image_color[image_num] = 11U;
    ColorSet(image_color[image_num]);
  }

  Serial.println(">> Wavelit: Running ..");
}

void Wavelit::Init_Tone() {
  uint8_t display_tone_led = 0;
  uint16_t display_color_led = 0;

  /*Start by ringing initial sound*/
  for (uint16_t display_tone = 0; display_tone < 5000; display_tone++) {
    Flip();
    uint8_t tone_freq = 150U - (display_tone / 50);
    delayMicroseconds(tone_freq);
    display_tone_led++;
    if (display_tone_led > 50U) {
      display_tone_led = 0U;
      image_column++;
      display_color_led = display_color_led + 500U;
      if (display_color_led >= 0xFFF0U) {
        display_color_led = 0;
      }
      if (image_column >= 23U) {
        image_column = 0U;

      } else {
        /*skip*/
      }
      Wavelit_Pixels.setPixelColor(image_column, (Wavelit_Pixels.gamma32(Wavelit_Pixels.ColorHSV(display_color_led))));
      Wavelit_Pixels.show();
    } else {
      /*skip*/
    }
  }
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  delay(50);
  for (uint16_t display_tone = 0; display_tone < 4000; display_tone++) {
    Flip();
    delayMicroseconds(40);
  }
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  delay(50);
  for (uint16_t display_tone = 0; display_tone < 2000; display_tone++) {
    Flip();
    delayMicroseconds(40);
  }
}

void Wavelit::Flip() {
  polarity_flap = !polarity_flap;
  if (polarity_flap) {
    gpio_set_level((gpio_num_t)COIL_PIN_1, 1);  // HIGH
    gpio_set_level((gpio_num_t)COIL_PIN_2, 0);  // LOW
  } else {
    gpio_set_level((gpio_num_t)COIL_PIN_1, 0);  // LOW
    gpio_set_level((gpio_num_t)COIL_PIN_2, 1);  // HIGH
  }
}

void Wavelit::Image_Brightness(uint8_t level) {
  if (level != _level_last) {
    _level_last = level;
    Wavelit_Pixels.setBrightness(level + 1);
  }
}

void Wavelit::Run() {
  if (w_timeflag == 1) {
    w_timeflag = 0;
    if ((play_tune) && (init_learnflag)) {
      Alarm();
    } else if ((ota_flag) || ((sleep_flag) && (init_learnflag))) {
      Sleep_Handler();
    } else {
      Image_Handler();
    }
  } else {
    Time_Handler();
    if ((init_learnflag) && (first_connect == FIRST_CONNECT_KEY)) {
      BLEOTA.process();
    }
  }
}

void Wavelit::Image_Handler(void) {
  if (image_row == 0U) {
    if (polarity_flap) {
      Hall_Handler();
      if ((TuneAdjustFlag) || (save_alignment_flag == 0xFF)) {
        timerAlarm(cctimer, (_learn_value + tune_value_last), true, 0);
        if (save_alignment_flag == 0xFF) {
          EEPROM.put(EEPROM_OFFSET_TUNE, tune_value_last);
          EEP_write_flag = 1;
          save_alignment_flag = 0;
        }
        TuneAdjustFlag = 0;
      }
    } else {
      if (init_learnflag) {
        Image_Brightness(led_brightness);
        Light_Read();
        IR_Handler();
      } else {
        //skip
      }
    }
  } else {
    /*Skip*/
  }

  if (reminder_flag == 0) {
    if (!init_learnflag) {
      if (auto_calibrate_flag == 0xC) {
        Image_ShowCalb();
      } else {
        if (_first_connect_timer < 800) {
          _first_connect_timer++;
          hsv_offset = 4000;
          Image_ShowLogo();
        } else {
          Image_ShowBLE();
        }
      }
    } else {
      if (_print_enable == 1) {
        Image_ShowText();
      } else if (_print_enable == 2) {
        Image_ShowPrintImage(_ImageArray, _ImageSize);
      } else {
        switch (image_num) {
          case FACE_NUM_WAVE:
            Image_ShowWave();
            break;
          case FACE_NUM_TIME:
            Image_ShowTime();
            break;
          case FACE_NUM_STOPWATCH:
            Image_ShowStopWatch();
            break;
          case FACE_NUM_TEXT:
            Image_ShowText();
            break;
          case FACE_NUM_HEART:
            Image_ShowIcon(0);
            break;
          case FACE_NUM_PI:
            Image_ShowIcon(1);
            break;
          case FACE_NUM_CROSS:
            Image_ShowIcon(2);
            break;
          case FACE_NUM_LIGHTNING:
            Image_ShowIcon(3);
            break;
          case FACE_NUM_INFINITY:
            Image_ShowIcon(4);
            break;
          case FACE_NUM_GAMING_XOST:
            Image_ShowGaming();
            break;
          case FACE_NUM_CANDLE:
            Image_ShowCandle();
            break;
          case FACE_NUM_SPINNING_STICK:
            Image_ShowWheel();
            break;
          case FACE_NUM_TRIPLE_WAVE:
            Image_ShowTripleWave();
            break;
          case FACE_NUM_SQUARE:
            Image_ShowSquare();
            break;
          case FACE_NUM_BARS:
            Image_ShowBars();
            break;
          case FACE_NUM_TWISTED_BARS:
            Image_ShowLines();
            break;
          case FACE_NUM_BOUNCE:
            Image_ShowBounce();
            break;
          case FACE_NUM_DUCK:
            Image_ShowDuck();
            break;
          case FACE_NUM_ROBOT:
            Image_ShowRobot();
            break;
          case FACE_NUM_ALIENS:
            Image_ShowAlien();
            break;
          case FACE_NUM_DNA:
            Image_ShowDNA();
            break;
          case FACE_NUM_LOGO:
            Image_ShowLogo();
            break;
          case FACE_NUM_RAINBOW:
            Image_ShowRainbow();
            break;
          case FACE_NUM_SNOWMAN:
            Image_ShowSnowman();
            break;
          case FACE_NUM_DOLPHIN:
            Image_ShowDolphin();
            break;
          case FACE_NUM_BUTTERFLY:
            Image_ShowButterfly();
            break;
          case FACE_NUM_CAT:
            Image_ShowCat();
            break;
          case FACE_NUM_UFO:
            Image_ShowUfo();
            break;
          case FACE_NUM_ROCKET:
            Image_ShowRocket();
            break;
          case FACE_NUM_BRB:
            Image_ShowIcon(5);
            break;
        }
      }
    }
  } else {
    //Reminder Active
  }


  Wavelit_Pixels.clear();
  if (!polarity_flap) {
    for (image_column = 0; image_column < COL_NUM; image_column++) {
      if (image_array[image_row][image_column] == 0U) {
        Wavelit_Pixels.setPixelColor(image_column, 0U);
      } else {
        Wavelit_Pixels.setPixelColor(image_column, (Wavelit_Pixels.gamma32(Wavelit_Pixels.ColorHSV(image_array[image_row][image_column]))));
      }
    }
    Wavelit_Pixels.show();
  } else {
    if (image_num != 0) {
      Wavelit_Pixels.show();
    }
  }
}

void Wavelit::Alarm(void) {
  if (reminder_flag == 1) {
    sleep_flag = 0;
    if (_tune_melody) {
      while (_tune_melody == 1) {
        Time_Handler();
        Flip();
        delayMicroseconds(3);
        image_column++;
        if (image_column >= 23U) {
          image_column = 0U;
        } else {
          /*skip*/
        }
        Wavelit_Pixels.setPixelColor(image_column, 0, 100, 0);
        Wavelit_Pixels.show();
      }
    } else {
      Image_ShowReminder(reminder_index);
      Image_Handler();
    }

  } else {
    while (sleep_flag) {
      if (_tune_melody) {
        Flip();
        delayMicroseconds(3);
        image_column++;
        if (image_column >= 23U) {
          image_column = 0U;
        } else {
          /*skip*/
        }
        Wavelit_Pixels.setPixelColor(image_column, 100, 0, 0);
      } else {
        digitalWrite(7, LOW);
        digitalWrite(6, LOW);
        Wavelit_Pixels.clear();
      }
      Wavelit_Pixels.show();
      Sleep_Handler();
      Time_Handler();
    }
    play_tune = 0;
    Image_Swipe();
  }
}

void Wavelit::Image_Swipe(void) {
  bool slide_flag = 0;
  uint32_t display_bit = 0;
  while (slide_flag != 1) {
    if (first_connect == FIRST_CONNECT_KEY) {
      if (!reminder_flag) {
        image_num++;
      }
    }
    if (image_num >= FACE_NUM_REMINDER) {
      image_num = FACE_NUM_WAVE;
    }
    if (face_enable == 0) {
      face_enable = FACE_ID_WAVE;
      image_num = FACE_NUM_WAVE;
    }

    display_bit = 1 << (image_num - 1);
    if ((face_enable & display_bit) == display_bit) {
      slide_flag = 1;
    }
  }
  if (first_connect == FIRST_CONNECT_KEY) {
    Face_Sync();
    if (auto_swipe != 0xFF) {
      EEPROM.put(EEPROM_IMAGE_NUM, image_num);
      EEP_write_flag = 1;
      Clear_Reminder();
    }
    ColorSet(image_color[image_num]);
    if (image_num == FACE_NUM_STOPWATCH) {
      swtimer_flag = 1;
    }
    standby_minute = 0;
    standby_seconds = 0;
  }
}
void Wavelit::Sleep_Handler(void) {
  if (!play_tune) {
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    Wavelit_Pixels.clear();
    Wavelit_Pixels.show();
  }
  Light_Read();
  uint16_t prox_value = Light_ProximityRead();

  _ir_timer++;
  if (_ir_timer > 5) {
    _ir_timer = 0;
    if ((reminder_flag == 1) || ((prox_value > (_prox_last + 50)) || ((autoshutdown_flag == 0xFF) && (time_hour == autoshutdown_hour) && (time_minute == autoshutdown_minutes)))) {
      first_time_flag = 0;
      light_index = 0;
      standby_minute = 0;
      standby_seconds = 0;
      sleep_flag = 0;  //Wakeup
      _val = analogRead(4);
      _val_last = analogRead(4);
      Init_Tone();
    }
    _prox_last = prox_value;
  } else {
    /*Skip*/
  }
}

void Wavelit::Time_Handler(void) {
  if (second_timeflag) {
    second_timeflag = 0;
    time_seconds++;
    standby_seconds++;

    if (auto_swipe == 0xFF) {
      _auto_swipe_timer++;
      if (_auto_swipe_timer >= 5) {
        _auto_swipe_timer = 0;
        Image_Swipe();
      }
    }

    if (EEP_write_flag == 1) {
      Serial.println("Wrting...");
      EEP_write_flag = 0;
      EEPROM.commit();
    }
    _tune_melody = !_tune_melody;
    if (standby_seconds >= 60) {
      standby_seconds = 0;
      standby_minute++;
    }
    if (time_seconds >= 60) {
      time_seconds = 0;
      time_minute++;

      if (time_minute >= 60) {
        time_minute = 0;
        time_hour++;
        if (time_hour >= 24) {
          time_day++;
          if (time_day >= 8) {
            time_day = 1;
          }
          time_hour = 0;
        }
      }
      if (reminder_flag == 0) {
        Check_Reminder(time_hour, time_minute, time_day);
      }
    }
    swtime_seconds++;
    if (swtime_seconds >= 60) {
      swtime_seconds = 0;
      swtime_minute++;
      if (swtime_minute >= 60) {
        swtime_minute = 0;
        swtime_hour++;
        if (swtime_hour >= 24) {
          swtime_hour = 0;
        }
      }
    }
    second_flip = !second_flip;
  } else {
    if ((standby_minute >= shutdown_timer) && (shutdown_timer != 0)) {
      sleep_flag = 1;
    }
  }
}

void Wavelit::Hall_Handler(void) {
  uint32_t adc_avrg = 0;

  //Read Hall sensor
  _val = analogRead(4);

  if ((abs(_val - _val_last) > TOUCH_THRESHOLD) && (_val < 1000U) && (init_learnflag)) {  //   /*Touch detected - Change color*/
    ClearDisplay();
    Wavelit_Pixels.clear();
    Wavelit_Pixels.show();

    if (first_connect == FIRST_CONNECT_KEY) {
      if (!reminder_flag) {
        image_color[image_num]++;
      }

      standby_minute = 0;
      standby_seconds = 0;
      if (image_color[image_num] > 11) {
        image_color[image_num] = 0;
      }

      Init_Sync();
      ColorUpdate();
      Clear_Reminder();
    }
    for (uint16_t display_tone = 0; display_tone < 2000; display_tone++) {
      Flip();
      uint8_t tone_freq = 150U - (display_tone / 50);
      delayMicroseconds(50);
    }
    delay(200);
  } else {
    //Start by Averaging Sample
    _adc_samples[_adc_counter] = _val;
    for (int a = 0; a < ADC_ARRAY_SIZE; a++) {
      adc_avrg = _adc_samples[a] + adc_avrg;
    }
    adc_avrg = adc_avrg / ADC_ARRAY_SIZE;

    _adc_counter++;
    if (_adc_counter >= ADC_ARRAY_SIZE) {
      _adc_counter = 0;
    }
    if (auto_calibrate_flag == 0xFF) {
      auto_calibrate_flag = 0xC;
      init_learnflag = 0;
      _tune_index = 0;
      _tune_bestscore = 0;
      image_num = FACE_NUM_LOGO;
      hsv_offset = 6000;
      tune_value_last = 0;
      EEPROM.put(EEPROM_OFFSET_TUNE, tune_value_last);
    }

    //Start Auto Screen Adjustment Algorithm
    if ((!init_learnflag) && (auto_calibrate_flag == 0xC)) {
      //Search for the best intiail frequency
      if (_score_timer < SCORE_TIMING) {
        if (_score_timer == 0) {
          if (_tune_bestscore == 0) {
            _tune_bestscore = init_tune_adjust[_tune_index];
          }
          _learn_value = _tune_bestscore;
          timerAlarm(cctimer, _learn_value, true, 0);
          // Serial.print(" BROW: ");
          // Serial.print(_learn_value);
        }
      } else if (_score_timer < (SCORE_TIMING * 2)) {
        _high_score += adc_avrg;
      } else if (_score_timer < (SCORE_TIMING * 3)) {
        //Wait for filtering
        if (_score_timer == (SCORE_TIMING * 2)) {
          //Test current timing
          _learn_value = init_tune_adjust[_tune_index];
          timerAlarm(cctimer, _learn_value, true, 0);
          // Serial.print(" CROW: ");
          // Serial.print(_learn_value);
        }
      } else if (_score_timer < (SCORE_TIMING * 4)) {
        _current_score += adc_avrg;
      } else {
        //skip
      }

      if (_score_timer <= (SCORE_TIMING * 4)) {
        _score_timer++;
      } else {
        _score_timer = 0;
        // Serial.print(" I: ");
        // Serial.print(_tune_index);
        // Serial.print(" C: ");
        // Serial.print(_current_score);
        // Serial.print(" H: ");
        // Serial.print(_high_score);

        _tune_index++;
        if (_tune_index >= SCORE_INDEX) {
          _tune_index = 0;
          init_learnflag = 1;  //Init Learning has completed
          _learn_value = _tune_bestscore;
          timerAlarm(cctimer, _learn_value, true, 0);

          AutoCalibrateInProgress = 0x0;
          Init_Sync();
          // Serial.print("DONE - B: ");
          // Serial.println(_tune_bestscore);
          EEPROM.put(EEPROM_AUTO_TUNE, _learn_value);
          EEP_write_flag = 1;
          Image_Swipe();
          auto_calibrate_flag = 0;
          // Serial.println(_learn_value);

        } else {
          if (_high_score > _current_score) {
            //Update Current as best score
            _tune_bestscore = init_tune_adjust[_tune_index - 1];
          } else {
            //skip
          }
        }
        // Serial.print(" B: ");
        // Serial.println(_tune_bestscore);
        _high_score = 0;
        _current_score = 0;
      }
    } else {
      if (autotune_flag == 0xFF) {
        //Auto-Tune is on - Search for the best intiail frequency
        _finetune_timer++;
        _tune_score += adc_avrg;
        if (_finetune_timer >= 5) {
          _finetune_timer = 0;
          if (_finetune_freq == 0) {
            _low_score = _tune_score;
          } else if (_finetune_freq == 1) {
            _high_score = _tune_score;
          } else if (_finetune_freq == 2) {
            _current_score = _tune_score;
          } else {
            //skip
          }

          _finetune_freq++;
          if (_finetune_freq >= 3) {
            _finetune_freq = 0;
            //Select best frequency
            uint32_t max_score = _low_score;  // assume first as max
            uint8_t best_freq = 0;

            if (_current_score > max_score) {
              max_score = _current_score;
              best_freq = 2;
            }
            if (_high_score > max_score) {
              max_score = _high_score;
              best_freq = 1;
            }

            if (best_freq == 0) {
              _tune_bestscore = _tune_bestscore - 5;
            } else if (best_freq == 1) {
              _tune_bestscore = _tune_bestscore + 5;
            } else {
              //Current value is the best
            }
          }

          _tune_score = 0;
          if (_finetune_freq == 0) {
            _tune_bestscore = _tune_bestscore - 5;
          } else if (_finetune_freq == 1) {
            _tune_bestscore = _tune_bestscore + 10;
          } else if (_finetune_freq == 2) {
            _tune_bestscore = _tune_bestscore - 5;
          } else {
            //skip
          }

          if (_tune_bestscore > init_tune_adjust[SCORE_INDEX - 1U]) {
            _tune_bestscore = init_tune_adjust[SCORE_INDEX - 1U];
          } else if (_tune_bestscore < init_tune_adjust[0U]) {
            _tune_bestscore = init_tune_adjust[0U];
          } else {
            //Within Range
          }
          timerAlarm(cctimer, _tune_bestscore, true, 0);
        }
      } else {
        //Skip
      }
    }
  }
  _val_last = _val;
}

void Wavelit::IR_Handler(void) {
  light_avrg[light_index] = Light_WhiteRead();
  prox_avrg[light_index] = Light_ProximityRead();

  light_index++;
  if (light_index >= AVRG_FILTER_SIZE) {
    light_index = 0;
    first_time_flag = 1;
  }
  uint32_t light_avrg_total = 0;
  uint32_t prox_avrg_total = 0;
  for (uint8_t i = 0; i < AVRG_FILTER_SIZE; i++) {
    light_avrg_total += light_avrg[i];
    prox_avrg_total += prox_avrg[i];
  }
  light_avrg_total = light_avrg_total / AVRG_FILTER_SIZE;
  prox_avrg_total = prox_avrg_total / AVRG_FILTER_SIZE;

  _ir_timer++;
  if (_ir_timer > 5) {
    _ir_timer = 0;
    _ir_flag = 1;
  }
  if (first_time_flag) {
    bool load_flag = 0;
    uint8_t load_color = 0;
    if (_ir_flag == 1) {
      _ir_flag = 0;
      if ((light_avrg_total <= LIGHT_THRESHOLD) && (dark_shutdown == 0xFF)) {
        /*Turn-off Display unless swiped*/
        sleep_flag = 1;
        light_index = 0;
        first_time_flag = 0;
      } else if (prox_avrg_total > (_prox_last + 50)) {
        /*Hand Proximity detected - check every 1sec*/
        first_time_flag = 0;
        light_index = 0;
        sleep_flag = 0;
        image_column = 0;
        _swipe_timer = 0;

        for (uint16_t display_tone = 0; display_tone < 300; display_tone++) {
          Flip();
          uint8_t tone_freq = 150U - (display_tone / 3);
          delayMicroseconds(tone_freq);
        }
        digitalWrite(7, LOW);
        digitalWrite(6, LOW);
        while (prox_avrg_total > (_prox_last + 50)) {
          Light_Read();  //Read new proximity reading
          prox_avrg_total = Light_ProximityRead();

          _level_last = led_brightness + 1;

          delay(1);
          uint16_t off_timer = 301 - ((_swipe_timer * 2) / 10);
          if (off_timer > 0xFF) {
            off_timer = 0xFF;
          }
          Wavelit_Pixels.setBrightness((uint8_t)off_timer);
          if (_swipe_timer < 1500) {
            _swipe_timer++;
            Wavelit_Pixels.clear();
            load_color++;
            image_column++;
            if (load_flag) {
              if (image_column % 2 == 0) {
              } else {
                image_column++;
              }
            } else {
              if (image_column % 2 == 0) {
                image_column++;
              } else {
              }
            }
            if (image_column >= 23U) {
              load_flag = !load_flag;
              if (load_flag) {
                image_column = 0U;
              } else {
                image_column = 1U;
              }
              Wavelit_Pixels.clear();
            } else {
              /*skip*/
            }
            if (load_color == 1) {
              Wavelit_Pixels.setPixelColor(image_column, 100, 0, 0);
            } else if (load_color == 2) {
              Wavelit_Pixels.setPixelColor(image_column, 0, 100, 0);
            } else {
              Wavelit_Pixels.setPixelColor(image_column, 0, 0, 100);
              load_color = 0;
            }
            Wavelit_Pixels.show();
          } else {
            Wavelit_Pixels.clear();
            Wavelit_Pixels.show();
            if (!sleep_flag) {
              for (uint16_t display_tone = 0; display_tone < 5000; display_tone++) {
                Flip();
                uint8_t tone_freq = 50U + (display_tone / 50);
                delayMicroseconds(tone_freq);
              }
            }
            sleep_flag = 1;
          }
        }
        Wavelit_Pixels.setBrightness(254);
        _level_last = led_brightness + 1;
        if (!sleep_flag) {
          Image_Swipe();
          first_time_flag = 0;

          for (uint16_t display_tone = 0; display_tone < 600; display_tone++) {
            Flip();
            uint8_t tone_freq = 50U + (display_tone / 3);
            delayMicroseconds(tone_freq);
          }
        } else {
          //skip
        }
      } else {
        /*Skip*/
      }
      _prox_last = prox_avrg_total;
    }
  }
}

uint16_t Wavelit::Light_AmbientRead() {
  return _light_data[2];
}

uint16_t Wavelit::Light_WhiteRead() {
  return _light_data[1];
}

uint16_t Wavelit::Light_ProximityRead() {
  return _light_data[0];
}

void Wavelit::Light_Read() {
  uint16_t value = 0;
  Wire.beginTransmission(VCNL4040_ADDRESS);
  Wire.write(VCNL4040_PROX_REG);
  Wire.endTransmission(false); /*Don't release the bus*/
  I2CRead(VCNL4040_ADDRESS, _i2c_read_array, 2);
  Wire.endTransmission(false);
  value = _i2c_read_array[0];         /*LSB*/
  value |= (_i2c_read_array[1] << 8); /*MSB*/
  _light_data[0] = value;

  value = 0;
  Wire.beginTransmission(VCNL4040_ADDRESS);
  Wire.write(VCNL4040_WHITE_REG);
  Wire.endTransmission(false); /*Don't release the bus*/
  I2CRead(VCNL4040_ADDRESS, _i2c_read_array, 2);
  Wire.endTransmission(false);
  value = _i2c_read_array[0];         /*LSB*/
  value |= (_i2c_read_array[1] << 8); /*MSB*/
  _light_data[1] = value;

  value = 0;
  Wire.beginTransmission(VCNL4040_ADDRESS);
  Wire.write(VCNL4040_AMBIENT_REG);
  Wire.endTransmission(false); /*Don't release the bus*/
  I2CRead(VCNL4040_ADDRESS, _i2c_read_array, 2);
  Wire.endTransmission(false);
  value = _i2c_read_array[0];         /*LSB*/
  value |= (_i2c_read_array[1] << 8); /*MSB*/
  _light_data[2] = value;
}

bool Wavelit::Light_Init() {
  bool light_error = 0;
  uint8_t error_timer = 0;

  Wire.begin(8, 9, 400000);  // SDA on GPIO8, SCL on GPIO9, 400kHz speed

  Wire.beginTransmission(VCNL4040_ADDRESS);
  if (Wire.endTransmission() != 0) {
    while (error_timer < 100) {
      delay(10);
      Wire.beginTransmission(VCNL4040_ADDRESS);
      if (Wire.endTransmission() != 0) {
        error_timer++;
        if (error_timer >= 99) {
          Serial.println();
          Serial.println(">> Error: Light Sensor not found - Check Hardware");
          while (1) {
            delay(10);  //Wait
          }
        }
      } else {
        error_timer = 200;
      }
    }
  }

  /*Configure - Continuous conversion mode, high dynamic range, integration time of 80 ms*/
  _i2c_write_array[_i2c_write_size++] = VCNL4040_ALS_CONF_REG; /*Address*/
  _i2c_write_array[_i2c_write_size++] = (0x000 & 0xFF);        /*LSB*/
  _i2c_write_array[_i2c_write_size++] = ((0x000 >> 8) & 0xFF); /*MSB*/
  if (!I2CWrite(VCNL4040_ADDRESS, _i2c_write_array, _i2c_write_size)) {
    light_error = 1;
  }
  _i2c_write_size = 0;
  /*Configure - duty cycle to 1/40, 16-bit resolution*/
  _i2c_write_array[_i2c_write_size++] = VCNL4040_PS_CONF1_2_REG; /*Address*/
  _i2c_write_array[_i2c_write_size++] = (0x80E & 0xFF);          /*LSB*/
  _i2c_write_array[_i2c_write_size++] = ((0x80E >> 8) & 0xFF);   /*MSB*/
  if (!I2CWrite(VCNL4040_ADDRESS, _i2c_write_array, _i2c_write_size)) {
    light_error = 1;
  }
  _i2c_write_size = 0;
  /*Configure - Set LED current to 200 mA, No interrupt settings*/
  _i2c_write_array[_i2c_write_size++] = VCNL4040_PS_CONF3_MS_REG; /*Address*/
  _i2c_write_array[_i2c_write_size++] = (0x4710 & 0xFF);          /*LSB*/
  _i2c_write_array[_i2c_write_size++] = ((0x4710 >> 8) & 0xFF);   /*MSB*/
  if (!I2CWrite(VCNL4040_ADDRESS, _i2c_write_array, _i2c_write_size)) {
    light_error = 1;
  }
  _i2c_write_size = 0;
  Wire.endTransmission();


  return light_error;
}

void Wavelit::Light_Reset() {
  _i2c_write_array[_i2c_write_size++] = VCNL4040_ALS_CONF_REG; /*Address*/
  _i2c_write_array[_i2c_write_size++] = (0x01 & 0xFF);         /*LSB*/
  if (!I2CWrite(VCNL4040_ADDRESS, _i2c_write_array, _i2c_write_size)) {
    Serial.println(">> Error: Light Sensor not found");
  }
  _i2c_write_size = 0;
  _i2c_write_array[_i2c_write_size++] = VCNL4040_PS_CONF1_2_REG; /*Address*/
  _i2c_write_array[_i2c_write_size++] = (0x01 & 0xFF);           /*LSB*/
  if (!I2CWrite(VCNL4040_ADDRESS, _i2c_write_array, _i2c_write_size)) {
    Serial.println(">> Error: Light Sensor not found");
  }
  _i2c_write_size = 0;
}

bool I2CWrite(uint8_t add, uint8_t *buffer, size_t size) {
  Wire.beginTransmission(add);
  // Write the data buffer
  if (Wire.write(buffer, size) != size) {
    // If the number of bytes written is not equal to the length, return false
    Wire.endTransmission();  // Ensure to end transmission even if writing fails
    return false;
  }

  if (Wire.endTransmission() == 0) {
    return true;
  } else {
    return false;
  }
}

bool I2CRead(uint8_t add, uint8_t *buffer, size_t size) {
  size_t pos = 0;
  while (pos < size) {
    size_t read_size;
    if ((size - pos) > maxBufferSize()) {
      read_size = maxBufferSize();
    } else {
      read_size = size - pos;
    }

    size_t recv = Wire.requestFrom(add, read_size);

    if (recv != size) {
      return false;
    }

    for (uint16_t iii = 0; iii < size; iii++) {
      buffer[iii] = Wire.read();
    }
    pos += read_size;
  }
  return true;
}

size_t maxBufferSize() {
  return _maxBufferSize;
}


void Wavelit::PrintText(char word[]) {
  _print_enable = 1;
  int len = strlen(word);

  for (uint8_t aa = 0; aa < len; aa++) {
    textfaceText[aa] = (char)word[aa];
  }
  textfaceText[len] = '\0';  // terminate string
}

void Wavelit::PrintImage(const uint8_t* arr1, uint16_t size) {
  _print_enable = 2;
  _ImageSize = size;
  for (uint16_t iii = 0; iii < _ImageSize; iii++) {
    _ImageArray[iii] = arr1[iii];
  }
}

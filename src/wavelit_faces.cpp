#include "wavelit_faces.h"
#include "wavelit_reminders.h"
#include "wavelit_ble.h"
#include "wavelit.h"
#include <Adafruit_NeoPixel.h>

bool tree_flag = 0, sw_enable_flag = 0, play_tune = 0, dna_dir = 0, hsv_dir = 0, text_dir = 1;
uint16_t delay_num = 0, textslide_timer = 0, image_speed = 0U, gradient_max = 0xD555, gradient_min = 0xAAAB, hsv_offset = 0, wheel_speed = 8, wheel_timer = 0U;
uint8_t image_thr = 0, outline_pixle = 0, outline_index = 0, text_pos = 0, image_column = 0U, gradient_speed = 4, image_offset = 0U, image_face = 0, image_slide = 0;
uint8_t x_pixel = 0, offpixel_row = 1, offpixel_col = 0;
signed int text_offset = 0;

extern bool sleep_flag, text_change, time_change, counter_format, counter_type, second_flip, swtimer_flag;
extern uint8_t time_hour, time_minute, time_seconds, counter_h_limit, counter_l_limit, swtime_seconds, swtime_minute, swtime_hour;
extern char textfaceText[];

uint16_t image_array[11][23] = { { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U },
                                 { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U },
                                 { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U },
                                 { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U },
                                 { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U },
                                 { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U },
                                 { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U },
                                 { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U },
                                 { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U },
                                 { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U },
                                 { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U } };

bool time_array[11][23] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

bool text_array[11][276] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

void ClearDisplay(void) {
  for (uint8_t ii = 0; ii < ROW_NUM; ii++) {
    for (uint8_t iii = 0; iii < COL_NUM; iii++) {
      image_array[ii][iii] = 0U;
      time_array[ii][iii] = 0;
    }
  }
}
void Image_ShowTripleWave(void) {
  image_speed++;
  if (image_speed >= 25U) {
    image_speed = 0U;
    image_offset++;
    if (image_offset >= 23U) {
      image_offset = 0U;
    }
  }

  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (image_column = 0; image_column < COL_NUM; image_column++) {
      uint8_t temp_column_r = image_column + image_offset;
      if (temp_column_r >= COL_NUM) {
        temp_column_r = temp_column_r - COL_NUM;
      }
      image_array[i][image_column] = ((uint16_t)wave_array[i][temp_column_r]) * (100);
      uint8_t temp_column_g = image_column + image_offset + 6U;
      if (temp_column_g >= COL_NUM) {
        temp_column_g = temp_column_g - COL_NUM;
      }
      if ((wave_array[i][temp_column_r]) == (wave_array[i][temp_column_g])) {
        //skip
      } else {
        image_array[i][image_column] = image_array[i][image_column] + ((uint16_t)wave_array[i][temp_column_g]) * (20000);
      }
      uint8_t temp_column_b = image_column + image_offset + 12U;
      if (temp_column_b >= COL_NUM) {
        temp_column_b = temp_column_b - COL_NUM;
      }
      if ((wave_array[i][temp_column_b]) == (wave_array[i][temp_column_g])) {
        //skip
      } else {
        image_array[i][image_column] = image_array[i][image_column] + ((uint16_t)wave_array[i][temp_column_b]) * (40000);
      }
    }
  }
}
void Image_ShowWave(void) {
  image_speed++;
  if (image_speed >= 25U) {
    image_speed = 0U;
    image_offset++;
  }
  if (image_offset >= 23U) {
    image_offset = 0U;
  }

  for (image_column = 0; image_column < COL_NUM; image_column++) {
    for (uint8_t i = 0; i < ROW_NUM; i++) {
      image_array[i][image_column] = 0;
      if (hsv_dir) {
        hsv_offset = hsv_offset + gradient_speed;
        if (hsv_offset >= gradient_max) {
          hsv_dir = !hsv_dir;
        }
      } else {
        hsv_offset = hsv_offset - gradient_speed;
        if (hsv_offset <= gradient_min) {
          hsv_dir = !hsv_dir;
        }
      }
      uint8_t temp_column = image_column + image_offset;
      if (temp_column >= COL_NUM) {
        temp_column = temp_column - COL_NUM;
      }
      image_array[i][image_column] = ((uint16_t)wave_array[i][temp_column]) * hsv_offset;
    }
  }
}
void Image_ShowIcon(uint8_t icon_num) {
  image_speed++;
  if (icon_num == 4) {
    if (image_speed >= 15U) {
      image_speed = 0U;
      outline_index++;
    }
  } else {
    if (image_speed >= 38U) {
      image_speed = 0U;
      outline_index++;
    }
  }
  outline_pixle = 0;
  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (image_column = 0; image_column < COL_NUM; image_column++) {
      outline_pixle++;
      image_array[i][image_column] = 0;
      if (hsv_dir) {
        hsv_offset = hsv_offset + gradient_speed;
        if (hsv_offset >= gradient_max) {
          hsv_dir = !hsv_dir;
        }
      } else {
        hsv_offset = hsv_offset - gradient_speed;
        if (hsv_offset <= gradient_min) {
          hsv_dir = !hsv_dir;
        }
      }
      switch (icon_num) {
        case 0:
          if (outline_index >= 30) {
            outline_index = 0;
          }
          if ((outline_pixle == heart_outline[outline_index]) || (outline_pixle == heart_outline[outline_index + 1]) || (outline_pixle == heart_outline[outline_index + 2]) || (outline_pixle == heart_outline[outline_index + 3])) {
            image_array[i][image_column] = 0;
          } else {
            image_array[i][image_column] = ((uint16_t)heart_array[i][image_column]) * hsv_offset;
          }
          break;
        case 2:
          if (outline_index >= 48) {
            outline_index = 0;
          }
          if ((outline_pixle == cross_outline[outline_index]) || (outline_pixle == cross_outline[outline_index + 1]) || (outline_pixle == cross_outline[outline_index + 2]) || (outline_pixle == cross_outline[outline_index + 3])) {
            image_array[i][image_column] = 0;
          } else {
            image_array[i][image_column] = ((uint16_t)cross_array[i][image_column]) * hsv_offset;
          }
          break;
        case 1:
          image_array[i][image_column] = ((uint16_t)pi_array[i][image_column]) * hsv_offset;
          break;
        case 5:
          image_array[i][image_column] = ((uint16_t)brb_array[i][image_column]) * hsv_offset;
          break;
        case 3:
          if (outline_index >= 32) {
            outline_index = 0;
          }
          if ((outline_pixle == lightening_outline[outline_index]) || (outline_pixle == lightening_outline[outline_index + 1]) || (outline_pixle == lightening_outline[outline_index + 2]) || (outline_pixle == lightening_outline[outline_index + 3])) {
            image_array[i][image_column] = 0;
          } else {
            image_array[i][image_column] = ((uint16_t)lightening_array[i][image_column]) * hsv_offset;
          }
          break;
        case 4:
          if (outline_index >= 43) {
            outline_index = 0;
          }
          if ((outline_pixle == infinty_outline[outline_index]) || (outline_pixle == infinty_outline[outline_index + 1]) || (outline_pixle == infinty_outline[outline_index + 2]) || (outline_pixle == infinty_outline[outline_index + 3])) {
            image_array[i][image_column] = 0;
          } else {
            image_array[i][image_column] = ((uint16_t)infinty_array[i][image_column]) * hsv_offset;
          }
          break;
      }
    }
  }
}
void Image_ShowDuck(void) {
  image_speed++;
  if (image_speed >= 50) {
    x_pixel = (x_pixel + 1) % 69U;  // Scroll full width, wrap at 69
    image_face = (image_face + 1) % 2;
    image_speed = 0U;
  }

  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (uint8_t image_column = 0; image_column < COL_NUM; image_column++) {
      uint8_t src_col = (x_pixel + image_column) % 69U;
      uint16_t pic_index = (i * 69U) + src_col;
      switch (image_face) {
        case 0:
          image_array[i][image_column] = convertColors(duck1_array[pic_index]);
          break;
        case 1:
          image_array[i][image_column] = convertColors(duck2_array[pic_index]);
          break;
      }
    }
  }
}
void Image_ShowRobot(void) {
  image_speed++;
  if (image_speed >= image_thr) {
    image_thr = random(50, 150);
    image_face++;
    if (image_face >= 12) {
      image_face = 0;
    }
    image_speed = 0U;
  }
  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (image_column = 0; image_column < COL_NUM; image_column++) {
      x_pixel++;
      if ((x_pixel >= 253) || ((i == 0) && (image_column == 0))) {
        x_pixel = 0;
      }
      switch (image_face) {
        case 0:
          image_array[i][image_column] = (convertColors(robot1_array[x_pixel]));
          break;
        case 1:
          image_array[i][image_column] = (convertColors(robot2_array[x_pixel]));
          break;
        case 2:
          image_array[i][image_column] = (convertColors(robot3_array[x_pixel]));
          break;
        case 3:
          image_array[i][image_column] = (convertColors(robot4_array[x_pixel]));
          break;
        case 4:
          image_array[i][image_column] = (convertColors(robot3_array[x_pixel]));
          break;
        case 5:
          image_array[i][image_column] = (convertColors(robot2_array[x_pixel]));
          break;
        case 6:
          image_array[i][image_column] = (convertColors(robot1_array[x_pixel]));
          break;
        case 7:
          image_array[i][image_column] = (convertColors(robot5_array[x_pixel]));
          break;
        case 8:
          image_array[i][image_column] = (convertColors(robot6_array[x_pixel]));
          break;
        case 9:
          image_array[i][image_column] = (convertColors(robot7_array[x_pixel]));
          break;
        case 10:
          image_array[i][image_column] = (convertColors(robot6_array[x_pixel]));
          break;
        case 11:
          image_array[i][image_column] = (convertColors(robot5_array[x_pixel]));
          break;
      }
    }
  }
}

void Image_ShowPrintImage(const uint8_t* arr, uint16_t image_width) {
 uint8_t image_t_width = image_width / 11;
 uint16_t pic_index = 0;
  image_speed++;
  if (image_speed >= 100) {
    x_pixel = (x_pixel + 1) % image_t_width;  // Scroll full width, wrap at 69
    image_face = (image_face + 1) % 2;
    image_speed = 0U;
  }

  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (uint8_t image_column = 0; image_column < COL_NUM; image_column++) {
    if(image_t_width>23){
      uint8_t src_col = (x_pixel + image_column) % image_t_width;
       pic_index = (i * image_t_width) + src_col;
      }
      else{
       pic_index++;
      }
      image_array[i][image_column] = convertColors(arr[pic_index]);
    }
  }
}

void Image_ShowAlien(void) {
  image_speed++;
  if (image_speed >= 100) {
    x_pixel = (x_pixel + 1) % 46U;  // Scroll full width, wrap at 69
    image_face = (image_face + 1) % 2;
    image_speed = 0U;
  }

  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (uint8_t image_column = 0; image_column < COL_NUM; image_column++) {
      uint8_t src_col = (x_pixel + image_column) % 46U;
      uint16_t pic_index = (i * 46U) + src_col;
      switch (image_face) {
        case 0:
          image_array[i][image_column] = convertColors(alien1_array[pic_index]);
          break;
        case 1:
          image_array[i][image_column] = convertColors(alien2_array[pic_index]);
          break;
      }
    }
  }
}

void Image_ShowRainbow(void) {
  uint16_t p_num = 0U;
  image_speed++;
  if (image_speed >= delay_num) {
    image_face++;
    if (image_face >= 11) {
      image_face = 0;
    }
    image_speed = 0U;
  }

  if ((image_face == 0) || (image_face == 6)) {
    delay_num = random(600, 1000);
  } else {
    delay_num = random(40, 70);
  }
  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (uint8_t image_column = 0; image_column < COL_NUM; image_column++) {
      switch (image_face) {
        case 0:
          image_array[i][image_column] = convertColors(rainbow1_array[p_num]);
          break;
        case 1:
          image_array[i][image_column] = convertColors(rainbow2_array[p_num]);
          break;
        case 2:
          image_array[i][image_column] = convertColors(rainbow3_array[p_num]);
          break;
        case 3:
          image_array[i][image_column] = convertColors(rainbow4_array[p_num]);
          break;
        case 4:
          image_array[i][image_column] = convertColors(rainbow5_array[p_num]);
          break;
        case 5:
          image_array[i][image_column] = convertColors(rainbow6_array[p_num]);
          break;
        case 6:
          image_array[i][image_column] = 0;
          break;
        case 7:
          image_array[i][22 - image_column] = convertColors(rainbow6_array[p_num]);
          break;
        case 8:
          image_array[i][22 - image_column] = convertColors(rainbow5_array[p_num]);
          break;
        case 9:
          image_array[i][22 - image_column] = convertColors(rainbow4_array[p_num]);
          break;
        case 10:
          image_array[i][22 - image_column] = convertColors(rainbow3_array[p_num]);
          break;
      }
      p_num++;
    }
  }
}
void Image_ShowDolphin(void) {
  uint16_t p_num = 0U;
  image_speed++;
  if (image_speed >= delay_num) {
    image_face++;
    if (image_face >= 6) {
      image_face = 0;
    }
    image_speed = 0U;
  }

  if (image_face == 0) {
    delay_num = random(500, 2000);
  } else {
    delay_num = random(90, 110);
  }
  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (uint8_t image_column = 0; image_column < COL_NUM; image_column++) {
      switch (image_face) {
        case 0:
          image_array[i][image_column] = convertColors(dolphin1_array[p_num]);
          break;
        case 1:
          image_array[i][image_column] = convertColors(dolphin2_array[p_num]);
          break;
        case 2:
          image_array[i][image_column] = convertColors(dolphin3_array[p_num]);
          break;
        case 3:
          image_array[i][image_column] = convertColors(dolphin4_array[p_num]);
          break;
        case 4:
          image_array[i][image_column] = convertColors(dolphin5_array[p_num]);
          break;
        case 5:
          image_array[i][image_column] = convertColors(dolphin6_array[p_num]);
          break;
      }
      p_num++;
    }
  }
}
void Image_ShowButterfly(void) {
  uint16_t p_num = 0U;
  image_speed++;
  if (image_speed >= delay_num) {
    image_face++;
    if (image_face >= 6) {
      image_face = 0;
    }
    image_speed = 0U;
  }

  delay_num = random(100, 300);
  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (uint8_t image_column = 0; image_column < COL_NUM; image_column++) {
      switch (image_face) {
        case 0:
          image_array[i][image_column] = convertColors(butterfly4_array[p_num]);
          break;
        case 1:
          image_array[i][image_column] = convertColors(butterfly1_array[p_num]);
          break;
        case 2:
          image_array[i][image_column] = convertColors(butterfly2_array[p_num]);
          break;
        case 3:
          image_array[i][image_column] = convertColors(butterfly3_array[p_num]);
          break;
        case 4:
          image_array[i][image_column] = convertColors(butterfly2_array[p_num]);
          break;
        case 5:
          image_array[i][image_column] = convertColors(butterfly1_array[p_num]);
          break;
      }
      p_num++;
    }
  }
}
void Image_ShowCat(void) {
  uint16_t p_num = 0U;
  image_speed++;
  if (image_speed >= delay_num) {
    image_face++;
    if (image_face >= 15) {
      image_face = 0;
    }
    image_speed = 0U;
  }

  if (image_face == 0) {
    delay_num = random(500, 2000);
  } else {
    delay_num = random(90, 300);
  }

  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (uint8_t image_column = 0; image_column < COL_NUM; image_column++) {
      switch (image_face) {
        case 0:
          image_array[i][image_column] = convertColors(cat1_array[p_num]);
          break;
        case 1:
          image_array[i][image_column] = convertColors(cat2_array[p_num]);
          break;
        case 2:
          image_array[i][image_column] = convertColors(cat3_array[p_num]);
          break;
        case 3:
          image_array[i][image_column] = convertColors(cat4_array[p_num]);
          break;
        case 4:
          image_array[i][image_column] = convertColors(cat5_array[p_num]);
          break;
        case 5:
          image_array[i][image_column] = convertColors(cat6_array[p_num]);
          break;
        case 6:
          image_array[i][image_column] = convertColors(cat7_array[p_num]);
          break;
        case 7:
          image_array[i][image_column] = convertColors(cat8_array[p_num]);
          break;
        case 8:
          image_array[i][image_column] = convertColors(cat9_array[p_num]);
          break;
        case 9:
          image_array[i][image_column] = convertColors(cat8_array[p_num]);
          break;
        case 10:
          image_array[i][image_column] = convertColors(cat7_array[p_num]);
          break;
        case 11:
          image_array[i][image_column] = convertColors(cat6_array[p_num]);
          break;
        case 12:
          image_array[i][image_column] = convertColors(cat5_array[p_num]);
          break;
        case 13:
          image_array[i][image_column] = convertColors(cat4_array[p_num]);
          break;
        case 14:
          image_array[i][image_column] = convertColors(cat3_array[p_num]);
          break;
      }
      p_num++;
    }
  }
}
// void Image_ShowAlien(void) {//void Image_ShowTree(void) {
//   uint16_t p_num = 0U;

//   for (uint8_t i = 0; i < ROW_NUM; i++) {
//     for (uint8_t image_column = 0; image_column < COL_NUM; image_column++) {
//       if (tree1_array[p_num] == 0x1c) {
//         image_speed++;
//         if (image_speed >= random(2,6)) {
//           image_speed = 0;
//           tree_flag = 1;
//         }
//         if (tree_flag) {
//           image_array[i][image_column] = random(1000, 64000);
//         } else {
//           image_array[i][image_column] = convertColors(0x1c);
//         }
//         tree_flag = 0;
//       } else {
//         image_array[i][image_column] = convertColors(tree1_array[p_num]);
//       }
//       p_num++;
//     }
//   }
//   tree_flag = !tree_flag;
// }
void Image_ShowSnowman(void) {
  uint16_t p_num = 0U;
  image_speed++;
  if (image_speed >= delay_num) {
    image_face++;
    if (image_face >= 4) {
      image_face = 0;
    }
    image_speed = 0U;
  }
  delay_num = 80;
  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (uint8_t image_column = 0; image_column < COL_NUM; image_column++) {
      switch (image_face) {
        case 0:
          image_array[i][image_column] = convertColors(snowman1_array[p_num]);
          break;
        case 1:
          image_array[i][image_column] = convertColors(snowman2_array[p_num]);
          break;
        case 2:
          image_array[i][image_column] = convertColors(snowman3_array[p_num]);
          break;
        case 3:
          image_array[i][image_column] = convertColors(snowman2_array[p_num]);
          break;
      }
      p_num++;
    }
  }
}
void Image_ShowUfo(void) {
  uint16_t p_num = 0U;
  image_speed++;
  if (image_speed >= delay_num) {
    image_face++;
    if (image_face >= 24) {
      image_face = 0;
    }
    image_speed = 0U;
  }

  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (uint8_t image_column = 0; image_column < COL_NUM; image_column++) {
      switch (image_face) {
        case 0:
          delay_num = 50;
          image_array[i][image_column] = convertColors(ufo1_array[p_num]);
          break;
        case 1:
          image_array[i][image_column] = convertColors(ufo2_array[p_num]);
          break;
        case 2:
          image_array[i][image_column] = convertColors(ufo3_array[p_num]);
          break;
        case 3:
          image_array[i][image_column] = convertColors(ufo4_array[p_num]);
          break;
        case 4:
          image_array[i][image_column] = convertColors(ufo5_array[p_num]);
          break;
        case 5:
          image_array[i][image_column] = convertColors(ufo6_array[p_num]);
          break;
        case 6:
          image_array[i][image_column] = convertColors(ufo7_array[p_num]);
          break;
        case 7:
          image_array[i][image_column] = convertColors(ufo8_array[p_num]);
          break;
        case 8:
          image_array[i][image_column] = convertColors(ufo9_array[p_num]);
          break;
        case 9:
          image_array[i][image_column] = convertColors(ufo10_array[p_num]);
          break;
        case 10:
          delay_num = 300;
          image_array[i][image_column] = convertColors(ufo11_array[p_num]);
          break;
        case 11:
          delay_num = 100;
          image_array[i][image_column] = convertColors(ufo13_array[p_num]);
          break;
        case 12:
          image_array[i][image_column] = convertColors(ufo14_array[p_num]);
          break;
        case 13:
          image_array[i][image_column] = convertColors(ufo15_array[p_num]);
          break;
        case 14:
          image_array[i][image_column] = convertColors(ufo11_array[p_num]);
          break;
        case 15:
          image_array[i][image_column] = convertColors(ufo12_array[p_num]);
          break;
        default:
          image_array[i][image_column] = 0;
          break;
      }
      p_num++;
    }
  }
}
void Image_ShowRocket(void) {
  uint16_t p_num = 0U;

  image_speed++;
  if (image_speed >= delay_num) {
    image_face++;
    if (image_face >= 3) {
      image_face = 0;
    }
    image_speed = 0U;
  }
  delay_num = random(5, 30);
  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (uint8_t image_column = 0; image_column < COL_NUM; image_column++) {
      switch (image_face) {
        case 0:
          image_array[i][image_column] = convertColors(rocket1_array[p_num]);
          break;
        case 1:
          image_array[i][image_column] = convertColors(rocket2_array[p_num]);
          break;
        case 2:
          image_array[i][image_column] = convertColors(rocket3_array[p_num]);
          break;
      }
      p_num++;
    }
  }
}
uint16_t convertColors(uint8_t pcolor) {
  if (pcolor == 0x00) {
    return 0x0000;  // Keep 0 as 0
  } else if (pcolor == 0xff) {
    return 35000;
  } else if (pcolor == 19) {
    return 37479;
  } else if (pcolor == 31) {
    return 32767;
  } else if (pcolor == 236) {
    return 4668;
  } else if (pcolor == 248) {
    return 9337;
  } else if (pcolor == 92) {
    return 18761;
  } else if (pcolor == 227) {
    return 182;
  } else {
    // Extract RGB components
    uint16_t r = (pcolor & 0xE0) >> 5;  // 3 bits for Red
    uint16_t g = (pcolor & 0x1C) >> 2;  // 3 bits for Green
    uint16_t b = (pcolor & 0x03);       // 2 bits for Blue

    // Scale RGB to 8-bit (0–255)
    r = (r * 255) / 7;
    g = (g * 255) / 7;
    b = (b * 255) / 3;

    // Convert RGB to HSV Hue (0–65535)
    uint8_t max_val = max(r, max(g, b));
    uint8_t min_val = min(r, min(g, b));
    uint8_t delta = max_val - min_val;

    float hue = 0;

    if (delta == 0) {
      hue = 0;  // Achromatic (grey)
    } else if (max_val == r) {
      hue = 60 * (((float)(g - b) / delta));
    } else if (max_val == g) {
      hue = 60 * (((float)(b - r) / delta) + 2);
    } else {
      hue = 60 * (((float)(r - g) / delta) + 4);
    }

    if (hue <= 0) {
      hue = 1;
    }

    // Map 0–360 degrees to 0–65535 for NeoPixel
    uint16_t neo_hue = (uint16_t)((hue * 65535) / 360);

    return neo_hue;
  }
}
void ColorSet(int color_val) {
  color_val = 11 - color_val;
  switch (color_val) {
    case 11:
      //Rainbow pixels
      gradient_min = 0x2;     // 2 -> RGB(255, 0, 4) -> #FF0004 (Very Dark Red)
      gradient_max = 0xD555;  // 54613 -> RGB(0, 255, 246) -> #00FFF6 (Cyan)
      gradient_speed = 20;
      break;
    case 10:
      gradient_min = 0x1222;  // 4642 -> RGB(255, 98, 0) -> #FF6200 (Orange)
      gradient_max = 0xFFED;  // 65517 -> RGB(4, 0, 255) -> #0400FF (Blue)
      gradient_speed = 11;
      break;
    case 9:
      gradient_min = 0x1222;  // 4642 -> RGB(255, 98, 0) -> #FF6200 (Orange)
      gradient_max = 0x2FAD;  // 12205 -> RGB(255, 249, 0) -> #FFF900 (Yellow)
      gradient_speed = 1;
      break;
    case 8:
      gradient_min = 0x3;     // 3 -> RGB(255, 0, 8) -> #FF0008 (Red)
      gradient_max = 0x2AAD;  // 10925 -> RGB(255, 180, 0) -> #FFB400 (Golden Yellow)
      gradient_speed = 4;
      break;
    case 7:
      gradient_min = 0x2AAD;  // 10925 -> RGB(255, 180, 0) -> #FFB400 (Golden Yellow)
      gradient_max = 0x5557;  // 21847 -> RGB(128, 255, 0) -> #80FF00 (Lime Green)
      gradient_speed = 4;
      break;
    case 6:
      gradient_min = 0x5557;  // 21847 -> RGB(255, 255, 0) -> #FFFF00 (Yellow)
      gradient_max = 0x8001;  // 32769 -> RGB(0, 255, 128) -> #00FF80 (Spring Green)
      gradient_speed = 4;
      break;
    case 5:
      gradient_min = 0x8001;  // 32769 -> RGB(0, 255, 128) -> #00FF80 (Spring Green)
      gradient_max = 0xAAAB;  // 43691 -> RGB(0, 128, 255) -> #0080FF (Sky Blue)
      gradient_speed = 4;
      break;
    case 3:
      gradient_min = 0xAB63;  // 43875 -> RGB(0, 123, 255) -> #007BFF (Azure Blue)
      gradient_max = 0xB333;  // 45875 -> RGB(0, 77, 255) -> #004DFF (Bright Blue)
      gradient_speed = 1;
      break;
    case 2:
      gradient_min = 0x5E96;  // 24214 -> RGB(0, 255, 23) -> #00FF17 (Bright Green)
      gradient_max = 0x6666;  // 26214 -> RGB(0, 255, 78) -> #00FF4E (Neon Green)
      gradient_speed = 1;
      break;
    case 1:
      gradient_min = 0x2;    // 2 -> RGB(255, 0, 4) -> #FF0004 (Very Dark Red)
      gradient_max = 0x7D2;  // 2002 -> RGB(255, 0, 195) -> #FF00C3 (Magenta)
      gradient_speed = 1;
      break;
    case 0:
      //Rainbow solid
      gradient_min = 0x2;     // 2 -> RGB(255, 0, 4) -> #FF0004 (Very Dark Red)
      gradient_max = 0xD555;  // 54613 -> RGB(0, 255, 246) -> #00FFF6 (Cyan)
      gradient_speed = 1;
      break;
    default:
    case 4:
      gradient_min = 0xAAAB;  // 43691 -> RGB(0, 128, 255) -> #0080FF (Sky Blue)
      gradient_max = 0xD555;  // 54613 -> RGB(0, 255, 246) -> #00FFF6 (Cyan)
      gradient_speed = 4;
      break;
  }
}
void Image_ShowText(void) {
  Image_GetText(textfaceText);

  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (image_column = 0; image_column < COL_NUM; image_column++) {
      if (hsv_dir) {
        hsv_offset = hsv_offset + gradient_speed;
        if (hsv_offset >= gradient_max) {
          hsv_dir = !hsv_dir;
        }
      } else {
        hsv_offset = hsv_offset - gradient_speed;
        if (hsv_offset <= gradient_min) {
          hsv_dir = !hsv_dir;
        }
      }

      if (text_pos >= 25) {  //23
        textslide_timer++;
        if (textslide_timer > 12000) {
          textslide_timer = 0;
          if (text_dir) {
            text_offset++;
            if (text_offset > (text_pos - 22)) {
              text_dir = 0;
            }
          } else {
            text_offset--;
            if (text_offset <= 0) {
              text_dir = 1;
            }
          }
        }
      } else {
        text_offset = 0;
      }
      image_array[i][image_column] = ((uint16_t)text_array[i][22 - image_column + text_offset]) * hsv_offset;
    }
  }
}
void Image_ShowCandle(void) {
  image_speed++;
  if (image_speed >= 50U) {
    image_speed = 0U;
    image_face = random(0, 4);
  }
  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (image_column = 0; image_column < COL_NUM; image_column++) {
      hsv_offset = random(0, 3000);
      switch (image_face) {
        case 0:
          if (candle0_array[i][image_column] == 0) {
            image_array[i][image_column] = 0;
          } else {
            image_array[i][image_column] = candle0_array[i][image_column] - hsv_offset;
          }
          break;
        case 1:
          if (candle1_array[i][image_column] == 0) {
            image_array[i][image_column] = 0;
          } else {
            image_array[i][image_column] = candle1_array[i][image_column] - hsv_offset;
          }
          break;
        case 2:
          if (candle2_array[i][image_column] == 0) {
            image_array[i][image_column] = 0;
          } else {
            image_array[i][image_column] = candle2_array[i][image_column] - hsv_offset;
          }
          break;
        default:
          if (candle3_array[i][image_column] == 0) {
            image_array[i][image_column] = 0;
          } else {
            image_array[i][image_column] = candle3_array[i][image_column] - hsv_offset;
          }
          break;
      }
    }
  }
}
void Image_ShowBounce(void) {
  image_speed++;
  if (image_speed >= 60) {
    image_face++;
    if (image_face >= 8) {
      image_face = 0;
    }
    image_speed = 0U;
    uint8_t FACE_ID_offset = 0;
    for (uint8_t i = 0; i < ROW_NUM; i++) {
      for (image_column = 0; image_column < COL_NUM; image_column++) {
        FACE_ID_offset = 0;
        if ((image_column <= 2) || (image_column >= 21)) {
          hsv_offset = 0;
        } else if (image_column <= 6) {
          hsv_offset = 45000;
          FACE_ID_offset = 0;
        } else if (image_column <= 9) {
          hsv_offset = 0;
        } else if (image_column <= 13) {
          hsv_offset = 25000;
          FACE_ID_offset = 4;
        } else if (image_column <= 16) {
          hsv_offset = 0;
        } else {
          hsv_offset = 100;
          FACE_ID_offset = 6;
        }
        FACE_ID_offset = FACE_ID_offset + image_face;
        if (FACE_ID_offset >= 8) {
          FACE_ID_offset = FACE_ID_offset - 8;
        }
        switch (FACE_ID_offset) {
          case 0:
            image_array[i][image_column] = ((uint16_t)b1_array[i]) * hsv_offset;
            break;
          case 1:
            image_array[i][image_column] = ((uint16_t)b2_array[i]) * hsv_offset;
            break;
          case 2:
            image_array[i][image_column] = ((uint16_t)b3_array[i]) * hsv_offset;
            break;
          case 3:
            image_array[i][image_column] = ((uint16_t)b4_array[i]) * hsv_offset;
            break;
          case 4:
            image_array[i][image_column] = ((uint16_t)b5_array[i]) * hsv_offset;
            break;
          case 5:
            image_array[i][image_column] = ((uint16_t)b4_array[i]) * hsv_offset;
            break;
          case 6:
            image_array[i][image_column] = ((uint16_t)b3_array[i]) * hsv_offset;
            break;
          default:
            image_array[i][image_column] = ((uint16_t)b2_array[i]) * hsv_offset;
            break;
        }

        if (hsv_dir) {
          hsv_offset = hsv_offset + gradient_speed;
          if (hsv_offset >= gradient_max) {
            hsv_dir = !hsv_dir;
          }
        } else {
          hsv_offset = hsv_offset - gradient_speed;
          if (hsv_offset <= gradient_min) {
            hsv_dir = !hsv_dir;
          }
        }
      }
    }
  }
}
void Image_ShowDNA(void) {
  image_speed++;
  if (image_speed >= 40) {
    image_face++;
    if (image_face >= 8) {
      image_face = 0;
    }
    image_speed = 0U;
    uint8_t FACE_ID_offset = 0;
    for (image_column = 0; image_column < COL_NUM; image_column++) {
      for (uint8_t i = 0; i < ROW_NUM; i++) {
        FACE_ID_offset = 0;
        if ((image_column <= 2) || (image_column >= 21)) {
          hsv_offset = 0;
        } else if (image_column <= 4) {
          hsv_offset = 60000;
          FACE_ID_offset = 0;
        } else if (image_column <= 6) {
          hsv_offset = 0;
        } else if (image_column <= 8) {
          hsv_offset = 50000;
          FACE_ID_offset = 1;
        } else if (image_column <= 10) {
          hsv_offset = 0;
        } else if (image_column <= 12) {
          hsv_offset = 40000;
          FACE_ID_offset = 2;
        } else if (image_column <= 14) {
          hsv_offset = 0;
        } else if (image_column <= 16) {
          hsv_offset = 50000;
          FACE_ID_offset = 3;
        } else if (image_column <= 18) {
          hsv_offset = 0;
        } else if (image_column <= 20) {
          hsv_offset = 60000;
          FACE_ID_offset = 4;
        } else {
          hsv_offset = 0;
        }
        if ((i >= 5) && (hsv_offset != 0)) {
          hsv_offset = hsv_offset - 6000;
        }
        FACE_ID_offset = FACE_ID_offset + image_face;
        if (FACE_ID_offset >= 8) {
          FACE_ID_offset = FACE_ID_offset - 8;
        }
        switch (FACE_ID_offset) {
          case 0:
            image_array[i][image_column] = ((uint16_t)d1_array[i]) * (hsv_offset);
            break;
          case 1:
            image_array[i][image_column] = ((uint16_t)d2_array[i]) * (hsv_offset);
            break;
          case 2:
            image_array[i][image_column] = ((uint16_t)d3_array[i]) * (hsv_offset);
            break;
          case 3:
            image_array[i][image_column] = ((uint16_t)d4_array[i]) * (hsv_offset);
            break;
          case 4:
            image_array[i][image_column] = ((uint16_t)d5_array[i]) * hsv_offset;
            break;
          case 5:
            image_array[i][image_column] = ((uint16_t)d4_array[i]) * (hsv_offset);
            break;
          case 6:
            image_array[i][image_column] = ((uint16_t)d3_array[i]) * (hsv_offset);
            break;
          default:
            image_array[i][image_column] = ((uint16_t)d2_array[i]) * (hsv_offset);
            break;
        }

        if (hsv_dir) {
          hsv_offset = hsv_offset + gradient_speed;
          if (hsv_offset >= gradient_max) {
            hsv_dir = !hsv_dir;
          }
        } else {
          hsv_offset = hsv_offset - gradient_speed;
          if (hsv_offset <= gradient_min) {
            hsv_dir = !hsv_dir;
          }
        }
      }
    }
  }
}
void Image_ShowSquare(void) {
  image_speed++;
  if (image_speed >= 15) {
    image_face++;
    if (image_face >= 36) {
      image_face = 0;
    }
    image_speed = 0U;
    hsv_offset = 60000;

    for (uint8_t i = 0; i < ROW_NUM; i++) {
      for (image_column = 0; image_column < COL_NUM; image_column++) {
        switch (image_face) {
          case 0:
            if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s1_array[i]) * hsv_offset;
            } else if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s1_array[ROW_NUM - i - 1]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 1:
            if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s2_array[i]) * hsv_offset;
            } else if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s2_array[ROW_NUM - i - 1]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 2:
            if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s3_array[i]) * hsv_offset;
            } else if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s3_array[ROW_NUM - i - 1]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 3:
            if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s4_array[i]) * hsv_offset;
            } else if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s4_array[ROW_NUM - i - 1]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 4:
            if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s5_array[i]) * hsv_offset;
            } else if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s5_array[ROW_NUM - i - 1]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 5:
            if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s4_array[ROW_NUM - i - 1]) * hsv_offset;
            } else if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s4_array[i]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 6:
            if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s3_array[ROW_NUM - i - 1]) * hsv_offset;
            } else if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s3_array[i]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 7:
            if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s2_array[ROW_NUM - i - 1]) * hsv_offset;
            } else if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s2_array[i]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 8:
            if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s1_array[ROW_NUM - i - 1]) * hsv_offset;
            } else if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s1_array[i]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 9:
            if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c1_array[image_column]) * hsv_offset;
            } else if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c1_array[COL_NUM - 1 - image_column]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 10:
            if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c2_array[image_column]) * hsv_offset;
            } else if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c2_array[COL_NUM - 1 - image_column]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 11:
            if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c3_array[image_column]) * hsv_offset;
            } else if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c3_array[COL_NUM - 1 - image_column]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 12:
            if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c4_array[image_column]) * hsv_offset;
            } else if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c4_array[COL_NUM - 1 - image_column]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 13:
            if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c5_array[image_column]) * hsv_offset;
            } else if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c5_array[COL_NUM - 1 - image_column]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 14:
            if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c4_array[COL_NUM - image_column - 1]) * hsv_offset;
            } else if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c4_array[image_column]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 15:
            if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c3_array[COL_NUM - image_column - 1]) * hsv_offset;
            } else if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c3_array[image_column]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 16:
            if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c2_array[COL_NUM - image_column - 1]) * hsv_offset;
            } else if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c2_array[image_column]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 17:
            if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c1_array[COL_NUM - image_column - 1]) * hsv_offset;
            } else if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c1_array[image_column]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 18:
            if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s1_array[ROW_NUM - 1 - i]) * hsv_offset;
            } else if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s1_array[i]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 19:
            if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s2_array[ROW_NUM - 1 - i]) * hsv_offset;
            } else if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s2_array[i]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 20:
            if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s3_array[ROW_NUM - 1 - i]) * hsv_offset;
            } else if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s3_array[i]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 21:
            if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s4_array[ROW_NUM - 1 - i]) * hsv_offset;
            } else if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s4_array[i]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 22:
            if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s5_array[ROW_NUM - 1 - i]) * hsv_offset;
            } else if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s5_array[i]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 23:
            if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s4_array[i]) * hsv_offset;
            } else if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s4_array[ROW_NUM - 1 - i]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 24:
            if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s3_array[i]) * hsv_offset;
            } else if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s3_array[ROW_NUM - 1 - i]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 25:
            if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s2_array[i]) * hsv_offset;
            } else if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s2_array[ROW_NUM - 1 - i]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 26:
            if (image_column == 16) {
              image_array[i][image_column] = ((uint16_t)s1_array[i]) * hsv_offset;
            } else if (image_column == 6) {
              image_array[i][image_column] = ((uint16_t)s1_array[ROW_NUM - 1 - i]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 27:
            if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c1_array[COL_NUM - image_column - 1]) * hsv_offset;
            } else if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c1_array[image_column]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 28:
            if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c2_array[COL_NUM - image_column - 1]) * hsv_offset;
            } else if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c2_array[image_column]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 29:
            if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c3_array[COL_NUM - image_column - 1]) * hsv_offset;
            } else if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c3_array[image_column]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 30:
            if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c4_array[COL_NUM - image_column - 1]) * hsv_offset;
            } else if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c4_array[image_column]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 31:
            if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c5_array[COL_NUM - image_column - 1]) * hsv_offset;
            } else if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c5_array[image_column]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 32:
            if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c4_array[image_column]) * hsv_offset;
            } else if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c4_array[COL_NUM - image_column - 1]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 33:
            if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c3_array[image_column]) * hsv_offset;
            } else if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c3_array[COL_NUM - image_column - 1]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 34:
            if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c2_array[image_column]) * hsv_offset;
            } else if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c2_array[COL_NUM - image_column - 1]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
          case 35:
            if (i == 10) {
              image_array[i][image_column] = ((uint16_t)c1_array[image_column]) * hsv_offset;
            } else if (i == 0) {
              image_array[i][image_column] = ((uint16_t)c1_array[COL_NUM - image_column - 1]) * (hsv_offset / 2);
            } else {
              image_array[i][image_column] = 0;
            }
            break;
        }
      }
    }
  }
}



void Image_ShowBars(void) {
  image_speed++;
  if (image_speed >= 60) {
    image_face++;
    if (image_face >= 8) {
      image_face = 0;
    }
    image_speed = 0U;
    uint8_t FACE_ID_offset = 0;
    for (uint8_t i = 0; i < ROW_NUM; i++) {
      for (image_column = 0; image_column < COL_NUM; image_column++) {
        FACE_ID_offset = 0;
        if ((image_column <= 2) || (image_column >= 21)) {
          hsv_offset = 0;
        } else if (image_column <= 4) {
          hsv_offset = 60000;
          FACE_ID_offset = 0;
        } else if (image_column <= 6) {
          hsv_offset = 0;
        } else if (image_column <= 8) {
          hsv_offset = 55000;
          FACE_ID_offset = 1;
        } else if (image_column <= 10) {
          hsv_offset = 0;
        } else if (image_column <= 12) {
          hsv_offset = 50000;
          FACE_ID_offset = 2;
        } else if (image_column <= 14) {
          hsv_offset = 0;
        } else if (image_column <= 16) {
          hsv_offset = 45000;
          FACE_ID_offset = 3;
        } else if (image_column <= 18) {
          hsv_offset = 0;
        } else if (image_column <= 20) {
          hsv_offset = 40000;
          FACE_ID_offset = 4;
        } else {
          hsv_offset = 0;
        }
        FACE_ID_offset = FACE_ID_offset + image_face;
        if (FACE_ID_offset >= 8) {
          FACE_ID_offset = FACE_ID_offset - 8;
        }
        switch (FACE_ID_offset) {
          case 0:
            image_array[i][image_column] = ((uint16_t)s1_array[i]) * hsv_offset;
            break;
          case 1:
            image_array[i][image_column] = ((uint16_t)s2_array[i]) * hsv_offset;
            break;
          case 2:
            image_array[i][image_column] = ((uint16_t)s3_array[i]) * hsv_offset;
            break;
          case 3:
            image_array[i][image_column] = ((uint16_t)s4_array[i]) * hsv_offset;
            break;
          case 4:
            image_array[i][image_column] = ((uint16_t)s5_array[i]) * hsv_offset;
            break;
          case 5:
            image_array[i][image_column] = ((uint16_t)s4_array[i]) * hsv_offset;
            break;
          case 6:
            image_array[i][image_column] = ((uint16_t)s3_array[i]) * hsv_offset;
            break;
          default:
            image_array[i][image_column] = ((uint16_t)s2_array[i]) * hsv_offset;
            break;
        }
      }
    }
  }
}

void Image_ShowBLE(void) {
  uint16_t p_num = 0U;
  image_speed++;
  if (image_speed >= delay_num) {
    image_face++;
    if (image_face >= 4) {
      image_face = 0;
    }
    image_speed = 0U;
  }

  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (uint8_t image_column = 0; image_column < COL_NUM; image_column++) {
      switch (image_face) {
        case 0:
          delay_num = random(250, 450);
          image_array[i][image_column] = convertColors(ble4_array[p_num]);
          break;
        case 1:
          delay_num = 70;
          image_array[i][image_column] = convertColors(ble1_array[p_num]);
          break;
        case 2:
          image_array[i][image_column] = convertColors(ble2_array[p_num]);
          break;
        case 3:
          image_array[i][image_column] = convertColors(ble3_array[p_num]);
          break;
      }
      p_num++;
    }
  }
}

void Image_ShowCalb(void) {
  image_speed++;
  if (image_speed >= 25U) {
    image_speed = 0U;
    image_offset++;
  }
  if (image_offset >= 23U) {
    image_offset = 0U;
  }

  for (image_column = 0; image_column < COL_NUM; image_column++) {
    for (uint8_t i = 0; i < ROW_NUM; i++) {
      image_array[i][image_column] = 0;
      if (hsv_dir) {
        hsv_offset = hsv_offset + gradient_speed;
        if (hsv_offset >= gradient_max) {
          hsv_dir = !hsv_dir;
        }
      } else {
        hsv_offset = hsv_offset - gradient_speed;
        if (hsv_offset <= gradient_min) {
          hsv_dir = !hsv_dir;
        }
      }
      uint8_t temp_column = image_column + image_offset;
      if (temp_column >= COL_NUM) {
        temp_column = temp_column - COL_NUM;
      }
      image_array[i][image_column] = ((uint16_t)wave_array[i][temp_column]) * 4000;
    }
  }
}


void Image_ShowLines(void) {
  image_speed++;
  if (image_speed >= 60) {
    image_face++;
    if (image_face >= 8) {
      image_face = 0;
    }
    image_speed = 0U;
    uint8_t FACE_ID_offset = 0;
    for (uint8_t i = 0; i < ROW_NUM; i++) {
      for (image_column = 0; image_column < COL_NUM; image_column++) {
        FACE_ID_offset = 0;
        if ((image_column <= 2) || (image_column >= 21)) {
          hsv_offset = 0;
        } else if (image_column <= 4) {
          hsv_offset = 20000;
          FACE_ID_offset = 0;
        } else if (image_column <= 6) {
          hsv_offset = 0;
        } else if (image_column <= 8) {
          hsv_offset = 40000;
          FACE_ID_offset = 1;
        } else if (image_column <= 10) {
          hsv_offset = 0;
        } else if (image_column <= 12) {
          hsv_offset = 60000;
          FACE_ID_offset = 2;
        } else if (image_column <= 14) {
          hsv_offset = 0;
        } else if (image_column <= 16) {
          hsv_offset = 40000;
          FACE_ID_offset = 3;
        } else if (image_column <= 18) {
          hsv_offset = 0;
        } else if (image_column <= 20) {
          hsv_offset = 20000;
          FACE_ID_offset = 4;
        } else {
          hsv_offset = 0;
        }
        FACE_ID_offset = FACE_ID_offset + image_face;
        if (FACE_ID_offset >= 8) {
          FACE_ID_offset = FACE_ID_offset - 8;
        }
        switch (FACE_ID_offset) {
          case 0:
            image_array[i][image_column] = ((uint16_t)e1_array[i]) * hsv_offset;
            break;
          case 1:
            image_array[i][image_column] = ((uint16_t)e2_array[i]) * hsv_offset;
            break;
          case 2:
            image_array[i][image_column] = ((uint16_t)e3_array[i]) * hsv_offset;
            break;
          case 3:
            image_array[i][image_column] = ((uint16_t)e4_array[i]) * hsv_offset;
            break;
          case 4:
            image_array[i][image_column] = ((uint16_t)e5_array[i]) * hsv_offset;
            break;
          case 5:
            image_array[i][image_column] = ((uint16_t)e4_array[i]) * hsv_offset;
            break;
          case 6:
            image_array[i][image_column] = ((uint16_t)e3_array[i]) * hsv_offset;
            break;
          default:
            image_array[i][image_column] = ((uint16_t)e2_array[i]) * hsv_offset;
            break;
        }
      }
    }
  }
}
void Image_ShowLogo(void) {
  image_speed++;
  if (image_speed >= 55U) {
    image_face++;
    if (image_face == 3) {
      if (!hsv_dir) {
        hsv_offset = hsv_offset + 2000;
        if (hsv_offset >= 5000) {
          hsv_dir = !hsv_dir;
          hsv_offset = 6000;
        }
      } else {
        hsv_offset = hsv_offset - 2000;
        if (hsv_offset <= 3000) {
          hsv_dir = !hsv_dir;
          hsv_offset = 2000;
        }
      }
    }
    if (image_face > 10) {
      image_face = 0;
    }
    image_speed = 0U;
    for (uint8_t i = 0; i < ROW_NUM; i++) {
      for (image_column = 0; image_column < COL_NUM; image_column++) {
        switch (image_face) {
          case 0:
            image_array[i][image_column] = ((uint16_t)microbots_array[i][image_column]) * hsv_offset;
            break;
          case 1:
            image_array[i][image_column] = ((uint16_t)microbots_array1[i][image_column]) * hsv_offset;
            break;
          case 2:
            image_array[i][image_column] = ((uint16_t)microbots_array2[i][image_column]) * hsv_offset;
            break;
          case 3:
            image_array[i][image_column] = ((uint16_t)microbots_array3[i][image_column]) * hsv_offset;
            break;
          case 4:
            image_array[i][image_column] = ((uint16_t)microbots_array2[i][image_column]) * hsv_offset;
            break;
          case 5:
            image_array[i][image_column] = ((uint16_t)microbots_array1[i][image_column]) * hsv_offset;
            break;
          default:
            image_array[i][image_column] = ((uint16_t)microbots_array[i][image_column]) * hsv_offset;
            break;
        }
      }
    }
  }
}
void Image_ShowWheel(void) {
  gradient_max = 0xFFFF;
  gradient_min = 0;
  gradient_speed = 30;
  image_speed++;


  if (image_speed >= wheel_speed) {
    image_face++;
    if (image_face > 4) {
      image_face = 0;
      wheel_timer++;
      if (wheel_timer < 25) {
        wheel_speed++;
      } else if (wheel_timer < 50) {
        wheel_speed--;
      } else {
        wheel_timer = 0;
        wheel_speed = 5;
      }
    }
    image_speed = 0U;
    for (uint8_t i = 0; i < ROW_NUM; i++) {
      for (image_column = 0; image_column < COL_NUM; image_column++) {
        switch (image_face) {
          case 0:
            image_array[i][image_column] = ((uint16_t)line0_array[i][image_column]) * hsv_offset;
            break;
          case 1:
            image_array[i][image_column] = ((uint16_t)line1_array[i][image_column]) * hsv_offset;
            break;
          case 2:
            image_array[i][image_column] = ((uint16_t)line2_array[i][image_column]) * hsv_offset;
            break;
          case 3:
            image_array[i][image_column] = ((uint16_t)line3_array[i][image_column]) * hsv_offset;
            break;
        }
        if (hsv_dir) {
          hsv_offset = hsv_offset + gradient_speed;
          if (hsv_offset >= gradient_max) {
            hsv_dir = !hsv_dir;
          }
        } else {
          hsv_offset = hsv_offset - gradient_speed;
          if (hsv_offset <= gradient_min) {
            hsv_dir = !hsv_dir;
          }
        }
      }
    }
  }
}
void Image_ShowGaming(void) {
  image_speed++;
  if (image_speed >= 80U) {
    image_speed = 0U;
    image_offset++;
    if (image_offset >= 13U) {
      image_offset = 1U;
    }
  }

  switch (image_offset) {
    // case 0:
    //   for (uint8_t i = 0; i < ROW_NUM; i++) {
    //     for (image_column = 0; image_column < COL_NUM; image_column++) {
    //       if (image_column < 6) {
    //         hsv_offset = 57684;
    //       } else if (image_column < (COL_NUM - 12)) {
    //         hsv_offset = 41869;
    //       } else if (image_column < (COL_NUM - 7)) {
    //         hsv_offset = 500;
    //       } else {
    //         hsv_offset = 20197;
    //       }
    //       image_array[i][image_column] = ((uint16_t)gaming_array[i][image_column]) * hsv_offset;
    //     }
    //   }
    //   break;
    case 1:
      for (uint8_t i = 0; i < ROW_NUM; i++) {
        for (image_column = 0; image_column < COL_NUM; image_column++) {
          if (image_column < (COL_NUM - 12)) {
            hsv_offset = 41869;
          } else {
            hsv_offset = 500;
          }
          image_array[i][image_column] = ((uint16_t)xo_array[i][image_column]) * hsv_offset;
        }
      }
      break;
    case 11:
      for (uint8_t i = 0; i < ROW_NUM; i++) {
        for (image_column = 0; image_column < COL_NUM; image_column++) {

          if (image_column < 10) {
            hsv_offset = 41869;
            image_array[i][image_column] = ((uint16_t)xo_array[i][image_column]) * hsv_offset;
          } else {
            hsv_offset = 20197;
            image_array[i][image_column] = ((uint16_t)st_array[i][image_column]) * hsv_offset;
          }
        }
      }
      break;
    case 8:
      for (uint8_t i = 0; i < ROW_NUM; i++) {
        for (image_column = 0; image_column < COL_NUM; image_column++) {

          if (image_column < 13) {
            hsv_offset = 41869;
            image_array[i][image_column] = ((uint16_t)xo_array[i][image_column]) * hsv_offset;
          } else {
            hsv_offset = 57684;
            image_array[i][image_column] = ((uint16_t)st_array[i][image_column - 13]) * hsv_offset;
          }
        }
      }
      break;
    case 2:
      for (uint8_t i = 0; i < ROW_NUM; i++) {
        for (image_column = 0; image_column < COL_NUM; image_column++) {

          if (image_column < 10) {
            hsv_offset = 500;
            image_array[i][image_column] = ((uint16_t)xo_array[i][image_column - 10]) * hsv_offset;
          } else {
            hsv_offset = 20197;
            image_array[i][image_column] = ((uint16_t)st_array[i][image_column]) * hsv_offset;
          }
        }
      }
      break;
    case 6:
      for (uint8_t i = 0; i < ROW_NUM; i++) {
        for (image_column = 0; image_column < COL_NUM; image_column++) {

          if (image_column < 10) {
            hsv_offset = 500;
            image_array[i][image_column] = ((uint16_t)xo_array[i][image_column - 10]) * hsv_offset;
          } else if (image_column < 13) {
            image_array[i][image_column] = 0;
          } else {
            hsv_offset = 57684;
            image_array[i][image_column] = ((uint16_t)st_array[i][image_column - 13]) * hsv_offset;
          }
        }
      }
      break;
    case 9:
      for (uint8_t i = 1; i < ROW_NUM; i++) {
        for (image_column = 0; image_column < COL_NUM; image_column++) {

          if (image_column < 10) {
            hsv_offset = 500;
            image_array[i][image_column] = ((uint16_t)xo_array[i][image_column - 10]) * hsv_offset;
          } else if (image_column < 13) {
            image_array[i][image_column] = 0;
          } else {
            hsv_offset = 41869;
            image_array[i][image_column] = ((uint16_t)xo_array[i][image_column - 13]) * hsv_offset;
          }
        }
      }
      break;
    case 3:
      for (uint8_t i = 0; i < ROW_NUM; i++) {
        for (image_column = 0; image_column < COL_NUM; image_column++) {

          if (image_column < 13) {
            hsv_offset = 20197;
            image_array[i][image_column] = ((uint16_t)st_array[i][image_column + 10]) * hsv_offset;
          } else if (image_column < 15) {
            image_array[i][image_column] = 0;
          } else {
            hsv_offset = 57684;
            image_array[i][image_column] = ((uint16_t)st_array[i][image_column - 15]) * hsv_offset;
          }
        }
      }
      break;
    case 7:
      for (uint8_t i = 0; i < ROW_NUM; i++) {
        for (image_column = 0; image_column < COL_NUM; image_column++) {

          if (image_column < 13) {
            hsv_offset = 20197;
            image_array[i][image_column] = ((uint16_t)st_array[i][image_column + 10]) * hsv_offset;
          } else if (image_column < 15) {
            image_array[i][image_column] = 0;
          } else {
            hsv_offset = 41869;
            image_array[i][image_column] = ((uint16_t)xo_array[i][image_column - 15]) * hsv_offset;
          }
        }
      }
      break;
    case 10:
      for (uint8_t i = 0; i < ROW_NUM; i++) {
        for (image_column = 0; image_column < COL_NUM; image_column++) {

          if (image_column < 13) {
            hsv_offset = 20197;
            image_array[i][image_column] = ((uint16_t)st_array[i][image_column + 10]) * hsv_offset;
          } else if (image_column < 14) {
            image_array[i][image_column] = 0;
          } else {
            hsv_offset = 500;
            image_array[i][image_column] = ((uint16_t)xo_array[i][image_column - 1]) * hsv_offset;
          }
        }
      }
      break;
    case 4:
      for (uint8_t i = 0; i < ROW_NUM; i++) {
        for (image_column = 0; image_column < COL_NUM; image_column++) {

          if (image_column < 10) {
            hsv_offset = 57684;
            image_array[i][image_column] = ((uint16_t)st_array[i][image_column]) * hsv_offset;
          } else if (image_column < 12) {
            image_array[i][image_column] = 0;
          } else {
            hsv_offset = 41869;
            image_array[i][image_column] = ((uint16_t)xo_array[i][image_column - 12]) * hsv_offset;
          }
        }
      }
      break;
    case 12:
      for (uint8_t i = 0; i < ROW_NUM; i++) {
        for (image_column = 0; image_column < COL_NUM; image_column++) {

          if (image_column < 10) {
            hsv_offset = 57684;
            image_array[i][image_column] = ((uint16_t)st_array[i][image_column]) * hsv_offset;
          } else if (image_column < 12) {
            image_array[i][image_column] = 0;
          } else {
            hsv_offset = 500;
            image_array[i][image_column] = ((uint16_t)xo_array[i][image_column]) * hsv_offset;
          }
        }
      }
      break;
    case 5:
      for (uint8_t i = 0; i < ROW_NUM; i++) {
        for (image_column = 0; image_column < COL_NUM; image_column++) {

          if (image_column < 10) {
            hsv_offset = 57684;
            image_array[i][image_column] = ((uint16_t)st_array[i][image_column]) * hsv_offset;
          } else {
            hsv_offset = 20197;
            image_array[i][image_column] = ((uint16_t)st_array[i][image_column]) * hsv_offset;
          }
        }
      }
      break;
  }
}
void Image_GetNum(int num, int pos) {
  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (image_column = 0; image_column < 4; image_column++) {
      switch (num) {
        case 0:
          time_array[i][22 - (image_column + pos)] = char_0[i][image_column];
          break;
        case 1:
          time_array[i][22 - (image_column + pos)] = char_1[i][image_column];
          break;
        case 2:
          time_array[i][22 - (image_column + pos)] = char_2[i][image_column];
          break;
        case 3:
          time_array[i][22 - (image_column + pos)] = char_3[i][image_column];
          break;
        case 4:
          time_array[i][22 - (image_column + pos)] = char_4[i][image_column];
          break;
        case 5:
          time_array[i][22 - (image_column + pos)] = char_5[i][image_column];
          break;
        case 6:
          time_array[i][22 - (image_column + pos)] = char_6[i][image_column];
          break;
        case 7:
          time_array[i][22 - (image_column + pos)] = char_7[i][image_column];
          break;
        case 8:
          time_array[i][22 - (image_column + pos)] = char_8[i][image_column];
          break;
        case 9:
          time_array[i][22 - (image_column + pos)] = char_9[i][image_column];
          break;
      }
    }
  }
}
void Image_GetText(char word[]) {
  uint8_t _text_size = 0;
  int l = 0;
  char modifiedWord[64];
  modifiedWord[0] = ' ';

  for (l = 0; word[l] != '\0'; l++) {
    modifiedWord[l + 1] = word[l];
  }
  modifiedWord[l + 1] = ' ';
  modifiedWord[l + 2] = '\0';

  text_pos = 0;

  if (text_change) {
    text_change = 0;
    for (uint8_t i = 0; i < ROW_NUM; i++) {
      for (uint16_t cc = 0; cc < 275; cc++) {
        text_array[i][cc] = '\0';
      }
    }
  }

  for (l = 0; modifiedWord[l] != '\0'; l++) {
    //Get Size of letter
    switch (modifiedWord[l]) {
      case '.':
        _text_size = sizeof(char_dot[0]);
        break;
      case ';':
        _text_size = sizeof(char_semicomma[0]);
        break;
      case '°':
      case 0xB0:
        _text_size = sizeof(char_degree[0]);
        break;
      case '@':
        _text_size = sizeof(char_at[0]);
        break;
      case '&':
        _text_size = sizeof(char_and[0]);
        break;
      case '=':
        _text_size = sizeof(char_eq[0]);
        break;
      case '+':
        _text_size = sizeof(char_pulse[0]);
        break;
      case '/':
        _text_size = sizeof(char_slash[0]);
        break;
      case ':':
        _text_size = sizeof(char_colon[0]);
        break;
      case '?':
        _text_size = sizeof(char_questionmark[0]);
        break;
      case '!':
        _text_size = sizeof(char_exclamationmark[0]);
        break;
      case ',':
        _text_size = sizeof(char_comma[0]);
        break;
      case '£':
      case 0xA3:
        _text_size = sizeof(char_pound[0]);
        break;
      case '$':
        _text_size = sizeof(char_dollar[0]);
        break;
      case '€':
      case 0xA4:
        _text_size = sizeof(char_euro[0]);
        break;
      case '-':
        _text_size = sizeof(char_dash[0]);
        break;
      case '%':
        _text_size = sizeof(char_percentage[0]);
        break;
      case 'A':
      case 'a':
        _text_size = sizeof(char_A[0]);
        break;
      case 'B':
      case 'b':
        _text_size = sizeof(char_B[0]);
        break;
      case 'C':
      case 'c':
        _text_size = sizeof(char_C[0]);
        break;
      case 'D':
      case 'd':
        _text_size = sizeof(char_D[0]);
        break;
      case 'E':
      case 'e':
        _text_size = sizeof(char_E[0]);
        break;
      case 'F':
      case 'f':
        _text_size = sizeof(char_F[0]);
        break;
      case 'G':
      case 'g':
        _text_size = sizeof(char_G[0]);
        break;
      case 'H':
      case 'h':
        _text_size = sizeof(char_H[0]);
        break;
      case 'I':
      case 'i':
        _text_size = sizeof(char_I[0]);
        break;
      case 'J':
      case 'j':
        _text_size = sizeof(char_J[0]);
        break;
      case 'K':
      case 'k':
        _text_size = sizeof(char_K[0]);
        break;
      case 'L':
      case 'l':
        _text_size = sizeof(char_L[0]);
        break;
      case 'M':
      case 'm':
        _text_size = sizeof(char_M[0]);
        break;
      case 'N':
      case 'n':
        _text_size = sizeof(char_N[0]);
        break;
      case 'O':
      case 'o':
        _text_size = sizeof(char_O[0]);
        break;
      case 'P':
      case 'p':
        _text_size = sizeof(char_P[0]);
        break;
      case 'Q':
      case 'q':
        _text_size = sizeof(char_Q[0]);
        break;
      case 'R':
      case 'r':
        _text_size = sizeof(char_R[0]);
        break;
      case 'S':
      case 's':
        _text_size = sizeof(char_S[0]);
        break;
      case 'T':
      case 't':
        _text_size = sizeof(char_T[0]);
        break;
      case 'U':
      case 'u':
        _text_size = sizeof(char_U[0]);
        break;
      case 'V':
      case 'v':
        _text_size = sizeof(char_V[0]);
        break;
      case 'W':
      case 'w':
        _text_size = sizeof(char_W[0]);
        break;
      case 'X':
      case 'x':
        _text_size = sizeof(char_X[0]);
        break;
      case 'Y':
      case 'y':
        _text_size = sizeof(char_Y[0]);
        break;
      case 'Z':
      case 'z':
        _text_size = sizeof(char_Z[0]);
        break;
      case ' ':
        _text_size = sizeof(char_space[0]);
        break;
      case '0':
        _text_size = sizeof(char_0[0]);
        break;
      case '1':
        _text_size = sizeof(char_1[0]);
        break;
      case '2':
        _text_size = sizeof(char_2[0]);
        break;
      case '3':
        _text_size = sizeof(char_3[0]);
        break;
      case '4':
        _text_size = sizeof(char_4[0]);
        break;
      case '5':
        _text_size = sizeof(char_5[0]);
        break;
      case '6':
        _text_size = sizeof(char_6[0]);
        break;
      case '7':
        _text_size = sizeof(char_7[0]);
        break;
      case '8':
        _text_size = sizeof(char_8[0]);
        break;
      case '9':
        _text_size = sizeof(char_9[0]);
        break;
      default:
        break;
    }

    //Fill Array with Letters
    for (uint8_t i = 0; i < ROW_NUM; i++) {
      for (image_column = text_pos; image_column < (_text_size + text_pos); image_column++) {
        switch (modifiedWord[l]) {
          case '.':
            text_array[i][image_column] = char_dot[i][image_column - text_pos];
            break;
          case ';':
            text_array[i][image_column] = char_semicomma[i][image_column - text_pos];
            break;
          case '°':
          case 0xB0:
            text_array[i][image_column] = char_degree[i][image_column - text_pos];
            break;
          case '@':
            text_array[i][image_column] = char_at[i][image_column - text_pos];
            break;
          case '&':
            text_array[i][image_column] = char_and[i][image_column - text_pos];
            break;
          case '=':
            text_array[i][image_column] = char_eq[i][image_column - text_pos];
            break;
          case '+':
            text_array[i][image_column] = char_pulse[i][image_column - text_pos];
            break;
          case '/':
            text_array[i][image_column] = char_slash[i][image_column - text_pos];
            break;
          case ':':
            text_array[i][image_column] = char_colon[i][image_column - text_pos];
            break;
          case '?':
            text_array[i][image_column] = char_questionmark[i][image_column - text_pos];
            break;
          case '!':
            text_array[i][image_column] = char_exclamationmark[i][image_column - text_pos];
            break;
          case ',':
            text_array[i][image_column] = char_comma[i][image_column - text_pos];
            break;
          case '£':
          case 0xA3:
            text_array[i][image_column] = char_pound[i][image_column - text_pos];
            break;
          case '$':
            text_array[i][image_column] = char_dollar[i][image_column - text_pos];
            break;
          case '€':
          case 0xA4:
            text_array[i][image_column] = char_euro[i][image_column - text_pos];
            break;
          case '-':
            text_array[i][image_column] = char_dash[i][image_column - text_pos];
            break;
          case '%':
            text_array[i][image_column] = char_percentage[i][image_column - text_pos];
            break;
          case 'A':
          case 'a':
            text_array[i][image_column] = char_A[i][image_column - text_pos];
            break;
          case 'B':
          case 'b':
            text_array[i][image_column] = char_B[i][image_column - text_pos];
            break;
          case 'C':
          case 'c':
            text_array[i][image_column] = char_C[i][image_column - text_pos];
            break;
          case 'D':
          case 'd':
            text_array[i][image_column] = char_D[i][image_column - text_pos];
            break;
          case 'E':
          case 'e':
            text_array[i][image_column] = char_E[i][image_column - text_pos];
            break;
          case 'F':
          case 'f':
            text_array[i][image_column] = char_F[i][image_column - text_pos];
            break;
          case 'G':
          case 'g':
            text_array[i][image_column] = char_G[i][image_column - text_pos];
            break;
          case 'H':
          case 'h':
            text_array[i][image_column] = char_H[i][image_column - text_pos];
            break;
          case 'I':
          case 'i':
            text_array[i][image_column] = char_I[i][image_column - text_pos];
            break;
          case 'J':
          case 'j':
            text_array[i][image_column] = char_J[i][image_column - text_pos];
            break;
          case 'K':
          case 'k':
            text_array[i][image_column] = char_K[i][image_column - text_pos];
            break;
          case 'L':
          case 'l':
            text_array[i][image_column] = char_L[i][image_column - text_pos];
            break;
          case 'M':
          case 'm':
            text_array[i][image_column] = char_M[i][image_column - text_pos];
            break;
          case 'N':
          case 'n':
            text_array[i][image_column] = char_N[i][image_column - text_pos];
            break;
          case 'O':
          case 'o':
            text_array[i][image_column] = char_O[i][image_column - text_pos];
            break;
          case 'P':
          case 'p':
            text_array[i][image_column] = char_P[i][image_column - text_pos];
            break;
          case 'Q':
          case 'q':
            text_array[i][image_column] = char_Q[i][image_column - text_pos];
            break;
          case 'R':
          case 'r':
            text_array[i][image_column] = char_R[i][image_column - text_pos];
            break;
          case 'S':
          case 's':
            text_array[i][image_column] = char_S[i][image_column - text_pos];
            break;
          case 'T':
          case 't':
            text_array[i][image_column] = char_T[i][image_column - text_pos];
            break;
          case 'U':
          case 'u':
            text_array[i][image_column] = char_U[i][image_column - text_pos];
            break;
          case 'V':
          case 'v':
            text_array[i][image_column] = char_V[i][image_column - text_pos];
            break;
          case 'W':
          case 'w':
            text_array[i][image_column] = char_W[i][image_column - text_pos];
            break;
          case 'X':
          case 'x':
            text_array[i][image_column] = char_X[i][image_column - text_pos];
            break;
          case 'Y':
          case 'y':
            text_array[i][image_column] = char_Y[i][image_column - text_pos];
            break;
          case 'Z':
          case 'z':
            text_array[i][image_column] = char_Z[i][image_column - text_pos];
            break;
          case ' ':
            text_array[i][image_column] = char_space[i][image_column - text_pos];
            break;
          case '0':
            text_array[i][image_column] = char_0[i][image_column - text_pos];
            break;
          case '1':
            text_array[i][image_column] = char_1[i][image_column - text_pos];
            break;
          case '2':
            text_array[i][image_column] = char_2[i][image_column - text_pos];
            break;
          case '3':
            text_array[i][image_column] = char_3[i][image_column - text_pos];
            break;
          case '4':
            text_array[i][image_column] = char_4[i][image_column - text_pos];
            break;
          case '5':
            text_array[i][image_column] = char_5[i][image_column - text_pos];
            break;
          case '6':
            text_array[i][image_column] = char_6[i][image_column - text_pos];
            break;
          case '7':
            text_array[i][image_column] = char_7[i][image_column - text_pos];
            break;
          case '8':
            text_array[i][image_column] = char_8[i][image_column - text_pos];
            break;
          case '9':
            text_array[i][image_column] = char_9[i][image_column - text_pos];
            break;
          default:
            break;
        }
      }
    }

    text_pos = text_pos + _text_size;
    text_pos++;  // Add Space
  }
  text_pos = text_pos - _text_size - 1;  //eliminate last letter
}
void Image_ShowTime(void) {
  int new_char = 0;
  new_char = time_hour / 10;
  Image_GetNum(new_char, 0);

  new_char = time_hour % 10;
  Image_GetNum(new_char, 5);

  new_char = time_minute / 10;
  Image_GetNum(new_char, 13);

  new_char = time_minute % 10;
  Image_GetNum(new_char, 18);

  if (second_flip) {
    time_array[5][12] = 1;
    time_array[7][12] = 1;
    time_array[5][11] = 1;
    time_array[7][11] = 1;
  } else {
    time_array[5][12] = 0;
    time_array[7][12] = 0;
    time_array[5][11] = 0;
    time_array[7][11] = 0;
  }


  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (image_column = 0; image_column < COL_NUM; image_column++) {
      if (hsv_dir) {
        hsv_offset = hsv_offset + gradient_speed;
        if (hsv_offset >= gradient_max) {
          hsv_dir = !hsv_dir;
        }
      } else {
        hsv_offset = hsv_offset - gradient_speed;
        if (hsv_offset <= gradient_min) {
          hsv_dir = !hsv_dir;
        }
      }
      image_array[i][image_column] = ((uint16_t)time_array[i][image_column]) * hsv_offset;
    }
  }
}
void Image_ShowStopWatch(void) {
  bool p = 0;
  int new_char = 0;
  uint8_t sw_hour = 0;
  uint8_t sw_minute = 0;
  uint8_t sw_second = 0;

  if (swtimer_flag) {
    swtimer_flag = 0;
    swtime_seconds = 0;
    swtime_minute = 0;
    swtime_hour = 0;
    play_tune = 0;
  } else {
    if (sw_enable_flag) {
      if (!play_tune) {
        if (!counter_type) {
          sw_hour = swtime_hour;
          sw_minute = swtime_minute;
          sw_second = swtime_seconds;
        } else {
          if (!counter_format) {
            if (counter_h_limit >= swtime_minute) {
              sw_minute = counter_h_limit - swtime_minute;
            } else {
              sw_minute = 0;
            }
            if (counter_l_limit >= swtime_seconds) {
              sw_second = counter_l_limit - swtime_seconds;

              if ((sw_minute == 0) && (sw_second == 0)) {
                sw_minute = 0;
                sw_second = 0;
                play_tune = 1;
                ClearDisplay();
                sleep_flag = 1;
                return;
              }

            } else {
              if ((counter_h_limit - swtime_minute) != 0) {
                swtime_minute++;
                swtime_seconds = 0;
                counter_l_limit = 59;
              } else {
                sw_minute = 0;
                sw_second = 0;
              }
            }
          } else {
            if (counter_h_limit >= swtime_hour) {
              sw_hour = counter_h_limit - swtime_hour;
            } else {
              sw_hour = 0;
            }
            if (counter_l_limit >= swtime_minute) {
              sw_minute = counter_l_limit - swtime_minute;
            } else {
              sw_minute = 0;
            }
          }
        }
      }
    }
  }

  if (!counter_format) {
    new_char = sw_minute / 10;
    Image_GetNum(new_char, 0);

    new_char = sw_minute % 10;
    Image_GetNum(new_char, 5);

    new_char = sw_second / 10;
    Image_GetNum(new_char, 13);

    new_char = sw_second % 10;
    Image_GetNum(new_char, 18);
  } else {
    new_char = sw_hour / 10;
    Image_GetNum(new_char, 0);

    new_char = sw_hour % 10;
    Image_GetNum(new_char, 5);

    new_char = sw_minute / 10;
    Image_GetNum(new_char, 13);

    new_char = sw_minute % 10;
    Image_GetNum(new_char, 18);
  }

  time_array[5][12] = 1;
  time_array[7][12] = 1;
  time_array[5][11] = 1;
  time_array[7][11] = 1;

  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (image_column = 0; image_column < COL_NUM; image_column++) {

      if (hsv_dir) {
        hsv_offset = hsv_offset + gradient_speed;
        if (hsv_offset >= gradient_max) {
          hsv_dir = !hsv_dir;
        }
      } else {
        hsv_offset = hsv_offset - gradient_speed;
        if (hsv_offset <= gradient_min) {
          hsv_dir = !hsv_dir;
        }
      }
      image_array[i][image_column] = ((uint16_t)time_array[i][image_column]) * hsv_offset;
    }
  }
}

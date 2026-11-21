#include "wavelit.h"
#include "wavelit_reminders.h"
#include "wavelit_faces.h"

bool reminder_flag = 0;
uint8_t active_reminders = 0;
uint8_t reminder_index = 0;
extern bool play_tune;
extern uint16_t image_array[11][23];
extern uint8_t dataBuffer[REMINDER_BUFFER_SIZE];

struct Reminder {
  // uint8_t id;  we can use the reminder index as id
  bool enable;
  uint8_t timeHrs;
  uint8_t timeMins;
  uint8_t imageId;
  uint8_t type;
  uint8_t repeat;
  uint8_t daysByte;
  uint8_t imageData[255];  // Adjust size based on the image data size
};

Reminder reminders_set[MAX_REMINDERS];  // Array to store 10 reminders

void Save_Reminder(void) {
  active_reminders = dataBuffer[0];  //ID
  if (active_reminders < MAX_REMINDERS) {
    reminders_set[active_reminders].enable = (bool)dataBuffer[1];

    if (reminders_set[active_reminders].enable) {
      reminders_set[active_reminders].timeHrs = dataBuffer[2];   //Hour time of reminder
      reminders_set[active_reminders].timeMins = dataBuffer[3];  //Minute time of reminder
      reminders_set[active_reminders].imageId = dataBuffer[4];   //image id (not in use)
      reminders_set[active_reminders].type = dataBuffer[5];      // Reminder(1) vs Alarm (0)
      reminders_set[active_reminders].repeat = dataBuffer[6];    //Ontime (0) vs Every-1hr(1) vs Every-2hr(2) vs Every-3hr(3) vs Every-4hr(4)
      reminders_set[active_reminders].daysByte = dataBuffer[7];  //Repeat days: Monb1 Tueb2 Wedb3 Thrb4 Frib5 Satb6 Sunb7

      for (size_t i = 0; i < 253; i++) {
        reminders_set[active_reminders].imageData[i] = dataBuffer[8 + i];  // Starting from index 8
      }
    } else {
      //skip
    }
  }
}

void Image_ShowReminder(uint8_t reminder_num) {
  uint16_t rpixel = 0;
  for (uint8_t i = 0; i < ROW_NUM; i++) {
    for (uint8_t c = 0; c < COL_NUM; c++) {
      image_array[i][c] = convertColors(reminders_set[reminder_num].imageData[rpixel]);
      rpixel++;
    }
  }
}

void Check_Reminder(uint8_t Hour_time, uint8_t Minute_time, uint8_t Day_time) {
  if (reminder_flag == 0) {
    for (uint8_t r = 0; r < MAX_REMINDERS; r++) {
      if ((reminders_set[r].enable == true) && (reminders_set[r].timeHrs == Hour_time) && (reminders_set[r].timeMins == Minute_time) && ((reminders_set[r].daysByte == 0) || (reminders_set[r].daysByte & (1 << (Day_time - 1))))) {
        reminder_flag = 1;

        reminders_set[r].timeHrs += reminders_set[r].repeat;  //Ontime (0) vs Every-1hr(1) vs Every-2hr(2) vs Every-3hr(3) vs Every-4hr(4)
        if (reminders_set[r].timeHrs >= 24U) {
          reminders_set[r].timeHrs = reminders_set[r].timeHrs - 24U;
        }

        reminder_index = r;
        Image_ShowReminder(r);
        if (reminders_set[r].type == 0) {
          play_tune = 1;
        }
        return;
      }
    }
  }
}

void Clear_Reminder(void) {
  reminder_flag = 0;
  if ((reminders_set[reminder_index].repeat == 0U) && (reminders_set[reminder_index].daysByte == 0)) {
    Delete_Reminder(reminder_index);
  } else {
    //Skip
  }
}

void Delete_Reminder(uint8_t index) {
  if ((active_reminders == 0) || (index >= active_reminders)) {
    return;  // Nothing to delete
  } else {
    for (uint8_t i = index; i < (active_reminders - 1); i++) {
      reminders_set[i] = reminders_set[i + 1];  // Shift all reminders above the deleted one down by one
    }
    active_reminders--;
  }
}
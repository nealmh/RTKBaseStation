#include "ui/screens/calibration_screen.h"
#include <Arduino.h>
#include <lvgl.h>
#include "ui/screen_manager.h"

static float cal[6];
lv_point_t p[3];
uint8_t step;
lv_obj_t *label_main = nullptr;
lv_obj_t *start_btn = nullptr;
lv_obj_t *start_label = nullptr;
lv_timer_t* timer = nullptr;
lv_obj_t* circle = nullptr;
const char* helpText[3] = {"Click the circle in upper left-hand corner\nand hold down until the next circle appears.",
                           "Click the circle in uppper right-hand corner\nand hold down until the next circle appears.",
                           "Click the circle in lower right-hand corner\nand hold down until the text disappears."
                          };
lv_point_t circlePos[3];

void cal_start_cb(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  if(code == LV_EVENT_CLICKED) {
    lv_obj_t* pg = (lv_obj_t*)lv_event_get_user_data(e);
    // create help text label
    label_main = lv_label_create(pg);
    lv_obj_set_width(label_main, LV_PCT(100));
    //lv_obj_set_pos(label_main, 0, LV_VER_RES - 50);
    lv_obj_set_style_text_align(label_main, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_del(lv_event_get_current_target(e));
    timer = lv_timer_create(inputPolling, 50, pg);
  
    // show first circle and help text
    step = 0;
    nextStep(0, 0, pg);
  }
}

void CalibrationScreen::enter(ScreenManager* screenManager) {
  Serial.println("Entering the Calibration screen");
  // initialize reference positions
  circlePos[0].x = 50;
  circlePos[0].y = 50;
  circlePos[1].x = LV_HOR_RES - 50;
  circlePos[1].y = 50;
  circlePos[2].x = LV_HOR_RES - 50;
  circlePos[2].y = LV_VER_RES - 50;
  screen_ = lv_obj_create(NULL);  

  screen_ = lv_obj_create(NULL);
  screenManager->setPageLabel("Calibration");
  pageArea_ = lv_obj_create(screen_);
  lv_obj_set_size(pageArea_, 240, 320-35);
  lv_obj_set_pos(pageArea_, 0, 35);
  lv_obj_set_flex_flow(pageArea_, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(pageArea_, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  if (label_main) lv_obj_del(label_main);
  lv_obj_t* startBtn = lv_btn_create(pageArea_);
  lv_obj_t* startLabel = lv_label_create(startBtn);
  lv_label_set_text(startLabel, "Start");
  lv_obj_add_event_cb(startBtn, cal_start_cb, LV_EVENT_ALL, pageArea_);

  // set calibration coefficients to identy
  cal[0] = 1;
  cal[1] = 0;
  cal[2] = 0;
  cal[3] = 0;
  cal[4] = 1;
  cal[5] = 0;
  tft.setTouchCalibration(NULL);
};

void CalibrationScreen::exit(ScreenManager* screenManager) {
  Serial.println("Exiting the Calibration screen");

  tft.getTouchCalibration(cal);
  Serial.printf("New Cal aX=%f, bX=%f, dX=%f, aY=%f, bY=%f, dY=%f\n", cal[0], cal[1], cal[2], cal[3], cal[4], cal[5]);
  for (int i=0; i<6; i++){
    calibration.touchCal[i] = cal[i];
  }
  calibration._dirty = true;
  calibration.save();
  lv_obj_del(screen_);
};

void CalibrationScreen::update(ScreenManager* screenManager) {

}

Screen& CalibrationScreen::getInstance() {
	static CalibrationScreen singleton;
	return singleton;
}


void addCircle(int centerX, int centerY)
{
  if (circle) lv_obj_del(circle);

  // create style
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_radius(&style, 10);
  lv_style_set_bg_opa(&style, LV_OPA_COVER);
  lv_style_set_bg_color(&style, lv_color_hex(0));

  // create object for drawing a circle, set style, and remove clickable flag
  circle = lv_obj_create(lv_scr_act());
  lv_obj_remove_style_all(circle);
  lv_obj_add_style(circle, &style, 0);
  lv_obj_clear_flag(circle, LV_OBJ_FLAG_CLICKABLE);

  lv_obj_set_size(circle, 10, 10);
  lv_obj_set_pos(circle, centerX, centerY);
}

void nextStep(int x, int y, lv_obj_t* page)
{
  // save and show coordinates from last step
  if (step > 0) {
    p[step - 1].x = x;
    p[step - 1].y = y;
  }

  if (step == 3) {
    lv_obj_del(circle);
    circle = NULL;
    lv_timer_del(timer);

    Serial.printf("Point 1 X: %d, Y: %d\n", p[0].x, p[0].y);
    Serial.printf("Point 2 X: %d, Y: %d\n", p[1].x, p[1].y);
    Serial.printf("Point 3 X: %d, Y: %d\n", p[2].x, p[2].y);

    // calculate coefficients based on the algorithms described here:
    // https://www.ti.com/lit/an/slyt277/slyt277.pdf
    float x1 = circlePos[0].x;
    float y1 = circlePos[0].y;
    float x2 = circlePos[1].x;
    float y2 = circlePos[1].y;
    float x3 = circlePos[2].x;
    float y3 = circlePos[2].y;
    float xs1 = p[0].x;
    float ys1 = p[0].y;
    float xs2 = p[1].x;
    float ys2 = p[1].y;
    float xs3 = p[2].x;
    float ys3 = p[2].y;

    float delta = (xs1 - xs3) * (ys2 - ys3) - (xs2 - xs3) * (ys1 - ys3);

    float deltaX1 = (x1 - x3) * (ys2 - ys3) - (x2 - x3) * (ys1 - ys3);
    float deltaX2 = (xs1 - xs3) * (x2 - x3) - (xs2 - xs3) * (x1 - x3);
    float deltaX3 = x1 * (xs2 * ys3 - xs3 * ys2) - x2 * (xs1 * ys3 - xs3 * ys1) + x3 * (xs1 * ys2 - xs2 * ys1);

    float deltaY1 = (y1 - y3) * (ys2 - ys3) - (y2 - y3) * (ys1 - ys3);
    float deltaY2 = (xs1 - xs3) * (y2 - y3) - (xs2 - xs3) * (y1 - y3);
    float deltaY3 = y1 * (xs2 * ys3 - xs3 * ys2) - y2 * (xs1 * ys3 - xs3 * ys1) + y3 * (xs1 * ys2 - xs2 * ys1);

    cal[0] = deltaX1 / delta;
    cal[1] = deltaX2 / delta;
    cal[2] = deltaX3 / delta;

    cal[3] = deltaY1 / delta;
    cal[4] = deltaY2 / delta;
    cal[5] = deltaY3 / delta;
    tft.setTouchCalibration(cal);

    lv_label_set_text(label_main, "Calibration Done");
  } else {
    // show help text for next step
    lv_label_set_text(label_main, helpText[step]);

    // show circle
    addCircle(circlePos[step].x, circlePos[step].y);
    step++;
  }
}

void inputPolling(lv_timer_t * timer) {
  // get input device and read touch position
  lv_indev_t* indev = lv_indev_get_next(NULL);
  lv_indev_data_t data;
  indev->driver->read_cb(indev->driver, &data);


  // sample touch position after half a second delay after pressed, to get stable coordinates
  static int lastState = LV_INDEV_STATE_REL;
  static int counter = 0;
  if (lastState == LV_INDEV_STATE_REL && data.state == LV_INDEV_STATE_PR) {
    counter = 5;
  }
  if (counter) {
    counter--;
    if (counter == 0) {
      Serial.printf("Step %i X: %d, Y: %d\n", step, data.point.x, data.point.y);
      nextStep(data.point.x, data.point.y, (lv_obj_t*)(timer->user_data));
    }
  }
  lastState = data.state;
}

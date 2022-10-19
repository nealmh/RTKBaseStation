
#include <ILI9341_T4.h>
#include <lvgl.h>

#include "ui/gui_setup.h"
#include "ui/screen_manager.h"

// Display buffers etc:
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 40MHz SPI. Can do much better with short wires
static const uint32_t SPI_SPEED = 40000000;

// screen size in portrait mode
static const uint16_t LX = 240;
static const uint16_t LY = 320;

// 2 diff buffers with about 8K memory each
EXTMEM ILI9341_T4::DiffBuffStatic<8000> diff1;
EXTMEM ILI9341_T4::DiffBuffStatic<8000> diff2;

// the internal framebuffer for the ILI9341_T4 driver (150KB) 
// in EXTMEM to save space in the lower (faster) part of RAM. 
EXTMEM uint16_t internal_fb[LX * LY];
EXTMEM uint16_t fb[LX * LY];              // the main framebuffer we draw onto.
// number of lines in lvgl's internal draw buffer 
static const uint8_t BUF_LY = 40;

EXTMEM lv_color_t lvgl_buf[LX*BUF_LY]; // memory for lvgl draw buffer (25KB) 

EXTMEM lv_disp_draw_buf_t draw_buf;    // lvgl 'draw buffer' object
EXTMEM lv_disp_drv_t disp_drv;         // lvgl 'display driver'
EXTMEM lv_indev_drv_t indev_drv;       // lvgl 'input device driver'


//Screen Driver
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// screen SCK       = 27  mandatory
// screen MISO      = 1   mandatory
// screen MOSI      = 26  mandatory
// screen DC        = 0   mandatory, can be any pin but using pin 0 (or 38 on T4.1) provides greater performance
// screen CS        = 30  optional (but recommended), can be any pin.  
// screen RESET     = 29  optional (but recommended), can be any pin.  
// screen BACKLIGHT = 255 optional, set this only if the screen LED pin is connected directly to the Teensy. 

// touch DIN        = 26  Same as screen MOSI
// touch DO         = 1   Same as screen MISO
// touch CS         = 4   mandatory, can be any pin
// touch IRQ        = 3   optional, can be any digital pin with interrupt capabilities
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
ILI9341_T4::ILI9341Driver tft(30, 0, 27, 26, 1, 29, 4, 3);
IntervalTimer guiTimer;
ScreenManager screenManager;
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Display callbacks:
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p) {
  const bool redraw_now = lv_disp_flush_is_last(disp);  // check if when should update the screen (or just buffer the changes). 
  tft.updateRegion(redraw_now, (uint16_t*)color_p, area->x1, area->x2, area->y1, area->y2); // update the interval framebuffer and then redraw the screen if requested
  lv_disp_flush_ready(disp); // tell lvgl that we are done and that the lvgl draw buffer can be reused immediately  
}


/** Call back to read the touchpad */
void my_touchpad_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
  int touchX, touchY, touchZ;
  bool touched = tft.readTouch(touchX, touchY, touchZ); // read the touchpad
  if (!touched) { // nothing
    data->state = LV_INDEV_STATE_REL;
  }
  else { // pressed
    data->state = LV_INDEV_STATE_PR;
    data->point.x = touchX;
    data->point.y = touchY;
  } 
}

// callback to update lvgl's tick every ms. 
void guiInc() {
  lv_tick_inc(1);
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


void beginDisplay() {
  // ------------------------------
  // Init the ILI9341_T4 driver. 
  // ------------------------------
  tft.output(&Serial);                // send debug info to serial port.
  while (!tft.begin(SPI_SPEED));      // init
  tft.setFramebuffer(internal_fb);    // set the internal framebuffer
  tft.setDiffBuffers(&diff1, &diff2); // set the diff buffers
  tft.setRotation(0);                 // portrite mode 0 : 240x320
  tft.setDiffGap(4);                  // with have large 8K diff buffers so we can use a small gap. 
  tft.setVSyncSpacing(2);             // lvgl is already controlling framerate: we just set this to 1 to minimize screen tearing. 
  tft.setRefreshRate(60);             // 60Hz refresh, why not...
  for (int i = 0; i < LX * LY; i++) fb[i] = ILI9341_T4_COLOR_WHITE; //blank the screen
  tft.overlayText(fb, "GNSS Base Station", 3, 10, 10, ILI9341_T4_COLOR_BLACK, 1.0f, ILI9341_T4_COLOR_WHITE, 0.4f, 1); // draw text    
  tft.update(fb); // push the framebuffer to be displayed

  //touchscreen XPT2046 on the same SPI bus 
  //int touch_calib[4] = { 545, 3711, 3859, 554 }; 
  //float touch_calib[6] = { 1.138211, 0.0, -25.121952, 0.0, 1.139896, -20.673574 };   // Load the identity calibration data
  //tft.setTouchCalibration(touch_calib);          // if the values are already known...
  

  // ------------------------------
  // Init LVGL
  // ------------------------------
  lv_init();
  
  // initialize lvgl drawing buffer
  lv_disp_draw_buf_init(&draw_buf, lvgl_buf, nullptr, LX * BUF_LY);

  // Initialize lvgl display driver
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = LX;
  disp_drv.ver_res = LY;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // Initialize lvgl input device driver (the touch screen)
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  // set the interval timer that given lvgl ticks. 
  guiTimer.begin(guiInc, 1000);
  Serial.println("LVGL Initialized.");

  // ------------------------------
  // Init the GUI
  // ------------------------------
  screenManager.setup();
}

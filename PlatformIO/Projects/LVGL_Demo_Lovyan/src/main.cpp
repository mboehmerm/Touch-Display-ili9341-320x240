/////////////////////////////////////////////////////////////////
/*
  ESP32 | LVGL8 | Ep 0. GFX Setup (ft. LovyanGFX)
  Video Tutorial: https://youtu.be/IPCvQ4o_WP8
  Created by Eric N. (ThatProject)
  Modified by mboehmerm
*/
/////////////////////////////////////////////////////////////////

#include <lvgl.h>
//#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#include <demos/lv_demos.h>
#include <examples/lv_examples.h>  //#include <../examples/lv_examples.h>

//#define LGFX_ESP32_ILI9341
#ifdef LGFX_ESP32_ILI9341
  #include <../../LGFX_ESP32_ILI9341.hpp>  // Path Arduino/libraries/ or LVGL_Demo_PIO/.pio/libdeps/esp32dev/
#else  
//  Create a class that does your own settings, derived from LGFX_Device. 
class LGFX : public lgfx::LGFX_Device
{ 
  lgfx::Panel_ILI9341      _panel_instance;
  lgfx::Bus_SPI            _bus_instance;
  lgfx::Touch_XPT2046      _touch_instance;

public:

  LGFX(void) 
  { 
    {  //  Configure bus control settings. 
      auto cfg = _bus_instance.config();  //  Get the structure for the bus configuration. 

      //  SPI bus configuration 
      cfg.spi_host    = VSPI_HOST;        //  Select the SPI to use ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST 
      //  * Due to the ESP-IDF version upgrade, VSPI_HOST and HSPI_HOST descriptions are deprecated, so if an error occurs, use SPI2_HOST and SPI3_HOST instead. 
      cfg.spi_mode    = 0;                //  Set SPI communication mode (0 ~ 3)
      cfg.freq_write  = 40000000;         //  80000000 // SPI clock frequency when sending (max 80MHz, rounded to 80MHz divided by an integer)
      cfg.freq_read   = 16000000;         //  SPI clock frequency when receiving 
      cfg.spi_3wire   = true;             //  Set true if receiving on the MOSI 
      cfg.use_lock    = true;             //  set true to use transaction 
      cfg.dma_channel = SPI_DMA_CH_AUTO;  //  Set the DMA channel to use (0=not use DMA / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=auto setting) 
      //  * With the ESP-IDF version upgrade, SPI_DMA_CH_AUTO (automatic setting) is recommended for the DMA channel.   Specifying 1ch and 2ch is deprecated. 
      cfg.pin_sclk    = 18;               //  SPI SCLK
      cfg.pin_mosi    = 23;               //  SPI MOSI
      cfg.pin_miso    = 19;               //  SPI MISO      (-1 = disable) 
      cfg.pin_dc      =  2;               //  SPI D/C = A0  (-1 = disable) 
      //  When using the same SPI bus as the SD card, be sure to set MISO without omitting it. 
      
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    { // Set display panel controls.
      auto cfg = _panel_instance.config();

      cfg.pin_cs           =    15;  // CS    (-1 = disable)
      cfg.pin_rst          =    -1;  // RST   (-1 = disable)
      cfg.pin_busy         =    -1;  // BUSY  (-1 = disable)

      // * The following setting values ​​are general initial values ​​for each panel, so please try commenting out any unknown items. 

      cfg.panel_width      =   240;  // actual displayable width
      cfg.panel_height     =   320;  // actual displayable  height
      cfg.offset_x         =     0;  // Panel offset in X 
      cfg.offset_y         =     0;  // Panel offset in Y
      cfg.offset_rotation  =     2;  // Rotation value offset 0~7 (4~7 are upside down 
      cfg.dummy_read_pixel =     8;  // Number of dummy read bits before pixel read
      cfg.dummy_read_bits  =     1;  // Number of dummy read bits before non-pixel data read
      cfg.readable         =  true;  // Data can be read set to true
      cfg.invert           = false;  // if panel light and dark are inverted set to true
      cfg.rgb_order        = false;  // if panel red and blue are reversed set to true
      cfg.dlen_16bit       = false;  // Set to true for panels that transmit data length in 16-bit units with 16-bit parallel or SPI
      cfg.bus_shared       =  true;  // If the bus is shared with the SD card, set to true (bus control with drawJpgFile etc.)

      // Please set below only with drivers that can change the number of pixels such as ST7735 and ILI9163.
      // Please set the following only when the display shifts with a driver with a variable number of pixels such as ST7735 or ILI9163.
      //cfg.memory_width     =   240;  // Maximum width  supported by the driver IC
      //cfg.memory_height    =   320;  // Maximum height supported by the driver IC

      _panel_instance.config(cfg);
    }

    { // Configure settings for touch screen control.  (delete if not necessary)

      auto cfg = _touch_instance.config();

      // numbers for x_min etc. doesn't work
      cfg.x_min           =  340;  // Minimum X value (raw value) obtained from the touchscreen
      cfg.x_max           = 3900;  // Maximum X value (raw value) obtained from the touchscreen
      cfg.y_min           =  235;  // Minimum Y value obtained from touchscreen (raw value)
      cfg.y_max           = 3900;  // Maximum Y value (raw value) obtained from touchscreen
      cfg.pin_int         =   -1;  // Pin number to which INT is connected (-1 = not connected)
      cfg.bus_shared      = true;  // Set true when using a common bus with the screen
      cfg.offset_rotation =    0;  // Adjustment when the orientation does not match Set with a value from 0 to 7
      //cfg.offset_rotation =  6;  // 6 fits but double lines. why?

      // For SPI connection
      cfg.spi_host = VSPI_HOST;    // Select SPI to use (HSPI_HOST or VSPI_HOST)
      cfg.freq     = 2500000;      // SPI Clock frequency 1000000 -> 2500000
      cfg.pin_sclk = 18;           // SCLK
      cfg.pin_mosi = 23;           // MOSI
      cfg.pin_miso = 19;           // MISO
      cfg.pin_cs   =  4;           // CS

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }
    setPanel(&_panel_instance);
  }
};
#endif

LGFX tft;

#define SCREEN_ROTATION 0                                   // set the screen rotation

/*Change to your screen resolution*/
#if (SCREEN_ROTATION == 1) || (SCREEN_ROTATION == 3)
  static const uint16_t screenWidth  = 320;                 // rotation 1 or 3
  static const uint16_t screenHeight = 240;
#else  
  static const uint16_t screenWidth  = 240;                 // rotation 0 or 2
  static const uint16_t screenHeight = 320;
#endif

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 4 ];    // screen buffer size
//static lv_color_t buf[ screenWidth * 10 ];                // smaller if compile error

// ------------------------------------------------------------------------------------------ //
/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
   uint32_t w = ( area->x2 - area->x1 + 1 );
   uint32_t h = ( area->y2 - area->y1 + 1 );

   tft.startWrite();
   tft.setAddrWindow( area->x1, area->y1, w, h );
   //tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
   tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
   tft.endWrite();

   lv_disp_flush_ready( disp );
}
// ------------------------------------------------------------------------------------------ //
/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
   uint16_t touchX, touchY;
   bool touched = tft.getTouch( &touchX, &touchY);
   if( !touched )
   {
      data->state = LV_INDEV_STATE_REL;
   }
   else
   {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touchX;
      data->point.y = touchY;

      Serial.print( "Data x " );
      Serial.println( touchX );

      Serial.print( "Data y " );
      Serial.println( touchY );
   }
}
// ------------------------------------------------------------------------------------------ //
void setup()
{
  Serial.begin(115200);

  tft.begin();        
  tft.setRotation(SCREEN_ROTATION);
  tft.setBrightness(255);

  uint16_t calData[8] = {3890, 340, 3900, 3895, 235, 340, 235, 3900};
  tft.setTouchCalibrate(calData);

  lv_init();
  lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  // *** uncomment only **ONE** of these lines ( examples or demos ) ***
    
  // lv_example_get_started_1();
  // lv_example_btn_1();            
  // lv_example_msgbox_1();
  // lv_example_menu_1();
  // lv_example_tabview_1();
  // ...

  lv_demo_widgets();               // OK ( OK = enabled in lv_conf.h or platform.ini)
  // lv_demo_benchmark();          // OK weighted FPS/Max : 27/68fps (40MHz), 34/87fps(80MHz), ST7735 50/??fps
  // lv_demo_keypad_encoder();     // OK works, but I haven't an encoder
  // lv_demo_music();              // Ok ?
  // lv_demo_printer();            // MISSING
  // lv_demo_stress();             // OK
}
// ------------------------------------------------------------------------------------------ //
void loop()
{
   lv_timer_handler(); /* let the GUI do its work */
   delay( 5 );
}
// ------------------------------------------------------------------------------------------ //
// ------------------------------------------------------------------------------------------ //
/*
static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        // Get the first child of the button which is the label and change its text
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}
// ------------------------------------------------------------------------------------------ //
// Create a button with a label and react on click event.
void lv_example_get_started_1(void)
{
    lv_obj_t * btn = lv_btn_create(lv_scr_act());    
    lv_obj_set_size(btn, 120, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0,0);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);
}
*/
// ------------------------------------------------------------------------------------------ //
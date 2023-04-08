/*

This the original LVGL_demo.ino 8.3.6 with all modifications i needed to compile and 
get the touch working correctly with Arduino IDE 2.0.4 and ili9341.

screen rotation : 1

*/

/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

#include <lvgl.h>
#include <TFT_eSPI.h>

/*To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 *You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 Note that the `lv_examples` library is for LVGL v7 and you shouldn't install it for this version (since LVGL v8)
 as the examples and demos are now part of the main LVGL library. */

#include <demos/lv_demos.h>                   //// modified : added

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320;       //// original, rotation 1 or 3
static const uint16_t screenHeight = 240;       //// original, rotation 1 or 3
//static const uint16_t screenWidth  = 240;     //// modified, rotation 0 or 2
//static const uint16_t screenHeight = 320;     //// modified, rotation 0 or 2

static lv_disp_draw_buf_t draw_buf;
//static lv_color_t buf[ screenWidth * 10 ];    // screen buffer
static lv_color_t buf[ screenWidth * screenHeight / 4 ];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

/* Display flushing */
//void my_disp_flush( lv_disp_t *disp, const lv_area_t *area, lv_color_t *color_p )
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )   //// modified

{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
//void my_touchpad_read( lv_indev_t * indev_driver, lv_indev_data_t * data )
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )           //// modified
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch( &touchX, &touchY, 600 );

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        //data->point.x = touchX;                  //// ok if screenWidth=240 + screenHeight=320
        //data->point.y = touchY;                  //// ok if screenWidth=240 + screenHeight=320
        data->point.x = touchY;                    //// uncomment if screenWidth=320 + screenHeight=240
        data->point.y = touchX;                    //// uncomment if screenWidth=320 + screenHeight=240

        Serial.print( "Data x " );
        Serial.println( touchX );

        Serial.print( "Data y " );
        Serial.println( touchY );
    }
}

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );

    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif

    tft.begin();          /* TFT init */
    tft.setRotation( 1 );                                   //// modified

    /*Set the touchscreen calibration data,
     the actual data for your display can be acquired using
     the Generic -> Touch_calibrate example from the TFT_eSPI library*/

    // r=rotation, w=screenWidth, h=screenHeight
    //uint16_t calData[5] = { 230, 3580, 330, 3600, 0 };    //// modified : r=3, w=320, h=240, touchX + touchY swapped.
    uint16_t calData[5] = { 230, 3580, 330, 3600, 6 };  //// modified : r=1, w=320, h=240, touchX + touchY swapped.
    //uint16_t calData[5] = { 230, 3580, 330, 3600, 2 };  //// modified : r=2, w=240, h=320
    //uint16_t calData[5] = { 230, 3580, 330, 3600, 4 };  //// modified : r=0, w=240, h=320
    tft.setTouch( calData );

    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

    /*Initialize the display*/
    //static lv_disp_t disp_drv;
    static lv_disp_drv_t disp_drv;                                        //// modified
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /*Initialize the (dummy) input device driver*/
    //static lv_indev_t indev_drv;
    static lv_indev_drv_t indev_drv;                                        //// modified
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );

#if 0
    /* Create simple label */
    lv_obj_t *label = lv_label_create( lv_scr_act() );
    lv_label_set_text( label, LVGL_Arduino.c_str() );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
#else
    /* Try an example from the lv_examples Arduino library
       make sure to include it as written above.
    lv_example_btn_1();
   */

    // uncomment ONE of these demos   ( OK = enabled in Arduino/libraries/lv_conf.h )
    lv_demo_widgets();               // OK
    // lv_demo_benchmark();          // OK
    // lv_demo_keypad_encoder();     // OK works, but I haven't an encoder
    // lv_demo_music();              // NOT TESTED
    // lv_demo_printer();            // MISSING
    // lv_demo_stress();             // OK
#endif
    Serial.println( "Setup done" );
}

void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay( 5 );
}

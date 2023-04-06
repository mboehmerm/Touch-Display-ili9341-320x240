// section `.dram0.bss' will not fit in region `dram0_0_seg'
// DRAM segment data does not fit.
//region `dram0_0_seg' overflowed by 26104 bytes

#include <lvgl.h>
#include <TFT_eSPI.h>
/*If you want to use the LVGL examples,
  make sure to install the lv_examples Arduino library
  and uncomment the following line.
#include <lv_examples.h>
*/
#include <demos/lv_demos.h>
//#include <TFT_Touch.h>                                   //// modified !!!
//#include <lv_demo.h>

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320; //160; //320; //480;    //// modified !!!
static const uint16_t screenHeight = 240; //128; //240;//320;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 4 ]; //  4 works, 2 works only with Display 128x160

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

// These are the pins used to interface between the 2046 touch controller and Arduino Pro
//#define DOUT 19 // 39  /* MISO Data out pin (T_DO) of touch screen */
//#define DIN  23 // 32  /* MOSI Data in pin (T_DIN) of touch screen */
//#define DCS   4 // 33  /* Chip select pin (T_CS) of touch screen */
//#define DCLK 18 // 25  /* Clock pin (T_CLK) of touch screen */

/* Create an instance of the touch screen library */         //// modified !!!
//TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
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

void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )  //// modified !!!
{
    uint16_t touchX, touchY;

    //bool touched = touch.Pressed();//tft.getTouch( &touchX, &touchY, 600 );
    bool touched = tft.getTouch( &touchX, &touchY, 600 );

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {   
        //touchX = touch.X();
        //touchY = touch.Y();
        
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchY;           //// modified !!!
        data->point.y = touchX;           //// modified !!!

        //Serial.print( "Data x " );
        //Serial.println( touchX );

        //Serial.print( "Data y " );
        //Serial.println( touchY );
    }
}

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */

    String LVGL_Arduino = "Hy Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );

    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif

    tft.begin();          /* TFT init */
    tft.setRotation(1);   /* Landscape orientation, flipped */
    
    //Set the touchscreen calibration data //// modified                       
    uint16_t calData2[5] = { 230, 3580, 330, 3500, 6 };  // 7 -> 6 and in my_touchpad_read swap X and Y
    tft.setTouch(calData2);

    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush; //刷新
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
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

    //// modified !!!

    // uncomment only **one** of these demos
    lv_demo_widgets(); Serial.println( "lv_demo_widgets();" );                // OK
    // lv_demo_benchmark(); Serial.println( "lv_demo_benchmark();" );            // OK
    // lv_demo_keypad_encoder(); Serial.println( "lv_demo_keypad_encoder();" );  // works, but I haven't an encoder
    // lv_demo_music();              // NOK
    // lv_demo_printer();
    // lv_demo_stress();             // seems to be OK
#endif
    Serial.println( "Setup done" );
}

void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay( 5 );
}
//http://lvgl.100ask.org/8.2/index.html
//https://www.cnblogs.com/frozencandles/archive/2022/06/14/16375392.html

/*
 
*/

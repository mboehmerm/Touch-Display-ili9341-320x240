# LVGL with PlatfomIO and iili9341 320x240 

SPI Touch Display ili9341 2.8" 320x240 XPT2046


install PlatformIO

PlatformIO open New Project
Project ili9341_LVGL_Widgets
Board Espressif ESP32 Dev Module

PlatformIO Libraries
search lvgl
8.3.4  Add to Project ili9341_LVGL_Widgets

search TFT_eSPI
2.5.23  Add to Project ili9341_LVGL_Widgets

TFT_eSPI

copy or Edit PlatformIO\Projects\ili9341_LVGL_Widgets\.pio\libdeps\esp32dev\TFT_eSPI\User_Setup.h
or PlatformIO\Projects\ili9341_LVGL_Widgets\.pio\libdeps\esp32dev\TFT_eSPI\User_Setup_Select.h
  copy Setup402_ILI9341_Touch.h to PlatformIO\Projects\ili9341_LVGL_Widgets\.pio\libdeps\esp32dev\Setup402_ILI9341_Touch.h
or edit platform.ini
PlatformIO\Projects\ili9341_LVGL_Widgets\platformio.ini

PlatformIO\Projects\ili9341_LVGL_Widgets\.pio\libdeps\esp32dev\lvgl\lv_conf_template.h 

save as PlatformIO\Projects\ili9341_LVGL_Widgets\.pio\libdeps\esp32dev\lv_conf.h

copy PlatformIO\Projects\ili9341_LVGL_Widgets\.pio\libdeps\esp32dev\lvgl\demos
to PlatformIO\Projects\ili9341_LVGL_Widgets\.pio\libdeps\esp32dev\lvgl\src\demos

copy PlatformIO\Projects\ili9341_LVGL_Widgets\.pio\libdeps\esp32dev\lvgl\examples
to PlatformIO\Projects\ili9341_LVGL_Widgets\.pio\libdeps\esp32dev\lvgl\src\examples

<!-- unvisible -->

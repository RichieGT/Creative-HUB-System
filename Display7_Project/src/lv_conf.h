/**
 * @file lv_conf.h
 * @brief Configuration file for v9.1.0
 */

/*
 * Copy this file as `lv_conf.h`
 * 1. simply next to the `lvgl` folder
 * 2. or any other place and
 *    - define `LV_CONF_INCLUDE_SIMPLE`
 *    - add the path as include path
 */

/* clang-format off */
#if 1 /*Set it to "1" to enable content*/

#ifndef LV_CONF_H
#define LV_CONF_H

#if !defined(__ASSEMBLER__)
#include <stdint.h>
#endif

/*====================
   COLOR SETTINGS
 *====================*/

/*Color depth: 1 (1 byte per pixel), 8 (RGB332), 16 (RGB565), 24 (RGB888), 32 (ARGB8888)*/
#define LV_COLOR_DEPTH 16

/*=========================
   MEMORY SETTINGS
 *=========================*/

/*1: use custom malloc/free, 0: use the built-in `lv_malloc/lv_free`*/
#define LV_USE_STDLIB_MALLOC 0

/*1: use custom string copy/compare, 0: use the built-in `lv_strcpy/lv_strcmp` etc.*/
#define LV_USE_STDLIB_STRING 0

/*1: use custom sprintf, 0: use the built-in `lv_sprintf`*/
#define LV_USE_STDLIB_SPRINTF 0

#define LV_MEM_CUSTOM 0

/*====================
   HAL SETTINGS
 *====================*/

/*Default display refresh period. LVG will redraw changed areas with this period time*/
#define LV_DEF_REFR_PERIOD 33   /*[ms]*/

/*Input device read period in milliseconds*/
#define LV_DEF_INDEV_READ_PERIOD 33 /*[ms]*/

/*Use a custom tick source that tells the elapsed time in milliseconds.
 *It removes the need to manually update the tick with `lv_tick_inc()`)*/
#define LV_TICK_CUSTOM 1
#if LV_TICK_CUSTOM
    #define LV_TICK_CUSTOM_INCLUDE "Arduino.h"
    #define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())
#endif   /*LV_TICK_CUSTOM*/

/*================
 *  WIDGETS
 *===============*/
#define LV_USE_OBJ_PROPERTY 1 // Required for some widgets/animations

/*================
 *  FONTS
 *===============*/
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_24 0

#endif /*LV_CONF_H*/

#endif /*End of "Content enable"*/

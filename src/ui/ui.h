// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 9.1.0
// Project name: SquareLine

#ifndef _SQUARELINE_UI_H
#define _SQUARELINE_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "ui_events.h"

void slide_Animation(lv_obj_t * TargetObject, int delay);

// SCREEN: ui_Loading_Screen
void ui_Loading_Screen_screen_init(void);
extern lv_obj_t * ui_Loading_Screen;
extern lv_obj_t * ui_Loading_Screen_Logo_Image;
extern lv_obj_t * ui_Loading_Screen__Loading_Label;
// CUSTOM VARIABLES
extern lv_obj_t * uic_Loading_Screen;
extern lv_obj_t * uic_Loading_Screen_Logo_Image;
extern lv_obj_t * uic_Loading_Screen__Loading_Label;

// SCREEN: ui_Main_Screen
void ui_Main_Screen_screen_init(void);
extern lv_obj_t * ui_Main_Screen;
extern lv_obj_t * ui_Main_Screen_Artist;
extern lv_obj_t * ui_Main_Screen_Station;
extern lv_obj_t * ui_Main_Screen_Commercial;
extern lv_obj_t * ui_Main_Screen_Title;
void ui_event_Main_Screen_No_WIFI_Image(lv_event_t * e);
extern lv_obj_t * ui_Main_Screen_No_WIFI_Image;
void ui_event_Main_Screen_WIFI_Image(lv_event_t * e);
extern lv_obj_t * ui_Main_Screen_WIFI_Image;
extern lv_obj_t * ui_Main_Screen_Line_Image;
extern lv_obj_t * ui_Main_Screen_Clock_Label;
void ui_event_Main_Screen_Tune_Button(lv_event_t * e);
extern lv_obj_t * ui_Main_Screen_Tune_Button;
extern lv_obj_t * ui_Main_Screen_Tune_Button_Label;
void ui_event_Main_Screen_Mode_Button(lv_event_t * e);
extern lv_obj_t * ui_Main_Screen_Mode_Button;
extern lv_obj_t * ui_Main_Screen_Mode_Button_Label;
// CUSTOM VARIABLES
extern lv_obj_t * uic_Main_Screen;
extern lv_obj_t * uic_Main_Screen_Artist;
extern lv_obj_t * uic_Main_Screen_Station;
extern lv_obj_t * uic_Main_Screen_Commercial;
extern lv_obj_t * uic_Main_Screen_Title;
extern lv_obj_t * uic_Main_Screen_No_WIFI_Image;
extern lv_obj_t * uic_Main_Screen_WIFI_Image;
extern lv_obj_t * uic_Main_Screen_Line_Image;
extern lv_obj_t * uic_Main_Screen_Clock_Label;
extern lv_obj_t * uic_Main_Screen_Tune_Button;
extern lv_obj_t * uic_Main_Screen_Tune_Button_Label;
extern lv_obj_t * uic_Main_Screen_Mode_Button;
extern lv_obj_t * uic_Main_Screen_Mode_Button_Label;

// SCREEN: ui_Volume_Screen
void ui_Volume_Screen_screen_init(void);
void ui_event_Volume_Screen(lv_event_t * e);
extern lv_obj_t * ui_Volume_Screen;
extern lv_obj_t * ui_Volume_Screen_Slider;
// CUSTOM VARIABLES
extern lv_obj_t * uic_Volume_Screen_Slider;

// SCREEN: ui_Network_Screen
void ui_Network_Screen_screen_init(void);
extern lv_obj_t * ui_Network_Screen;
void ui_event_Network_Screen_Keyboard1(lv_event_t * e);
extern lv_obj_t * ui_Network_Screen_Keyboard1;
void ui_event_Network_Screen_SSID_Text_Area(lv_event_t * e);
extern lv_obj_t * ui_Network_Screen_SSID_Text_Area;
void ui_event_Network_Screen_Password_Text_Area(lv_event_t * e);
extern lv_obj_t * ui_Network_Screen_Password_Text_Area;
void ui_event_Network_Screen_Cancel_Button(lv_event_t * e);
extern lv_obj_t * ui_Network_Screen_Cancel_Button;
extern lv_obj_t * ui_Network_Screen_Cancel_Button_Label;
// CUSTOM VARIABLES
extern lv_obj_t * uic_Network_Screen;
extern lv_obj_t * uic_Network_Screen_Keyboard1;
extern lv_obj_t * uic_Network_Screen_SSID_Text_Area;
extern lv_obj_t * uic_Network_Screen_Password_Text_Area;
extern lv_obj_t * uic_Network_Screen_Cancel_Button;
extern lv_obj_t * uic_Network_Screen_Cancel_Button_Label;

// SCREEN: ui_Mode_Screen
void ui_Mode_Screen_screen_init(void);
extern lv_obj_t * ui_Mode_Screen;
void ui_event_Mode_Screen_Mode_Roller(lv_event_t * e);
extern lv_obj_t * ui_Mode_Screen_Mode_Roller;
void ui_event_Mode_Screen_Cancel_Button(lv_event_t * e);
extern lv_obj_t * ui_Mode_Screen_Cancel_Button;
extern lv_obj_t * ui_Mode_Screen_Cancel_Button_Label;
void ui_event_Mode_Screen_Select_Button(lv_event_t * e);
extern lv_obj_t * ui_Mode_Screen_Select_Button;
extern lv_obj_t * ui_Mode_Screen_Select_Button_Label;
// CUSTOM VARIABLES
extern lv_obj_t * uic_Mode_Screen;
extern lv_obj_t * uic_Mode_Screen_Mode_Roller;
extern lv_obj_t * uic_Mode_Screen_Cancel_Button_Label;
extern lv_obj_t * uic_Mode_Screen_Select_Button;
extern lv_obj_t * uic_Mode_Screen_Select_Button_Label;

// SCREEN: ui_Station_Selection_Screen
void ui_Station_Selection_Screen_screen_init(void);
extern lv_obj_t * ui_Station_Selection_Screen;
void ui_event_Station_Selection_Screen_Play_Button(lv_event_t * e);
extern lv_obj_t * ui_Station_Selection_Screen_Play_Button;
extern lv_obj_t * ui_Station_Selection_Screen_Play_Button_Label;
void ui_event_Station_Selection_Screen_Cancel(lv_event_t * e);
extern lv_obj_t * ui_Station_Selection_Screen_Cancel;
extern lv_obj_t * ui_Station_Selection_Screen_Cancel_Button_Label;
extern lv_obj_t * ui_Station_Selection_Screen_Roller;
// CUSTOM VARIABLES
extern lv_obj_t * uic_Station_Selection_Screen;
extern lv_obj_t * uic_Station_Selection_Screen_Play_Button;
extern lv_obj_t * uic_Station_Selection_Screen_Play_Button_Label;
extern lv_obj_t * uic_Station_Selection_Screen_Cancel;
extern lv_obj_t * uic_Station_Selection_Screen_Cancel_Button_Label;
extern lv_obj_t * uic_Station_Selection_Screen_Roller;

// SCREEN: ui_Clock_Screen
void ui_Clock_Screen_screen_init(void);
void ui_event_Clock_Screen(lv_event_t * e);
extern lv_obj_t * ui_Clock_Screen;
extern lv_obj_t * ui_Clock_Screen_Clock_Label;
// CUSTOM VARIABLES
extern lv_obj_t * uic_Clock_Screen;
extern lv_obj_t * uic_Clock_Screen_Clock_Label;

// SCREEN: ui_Message_Screen
void ui_Message_Screen_screen_init(void);
extern lv_obj_t * ui_Message_Screen;
extern lv_obj_t * ui_Message_Screen_Message_Label;
// CUSTOM VARIABLES
extern lv_obj_t * uic_Message_Screen;
extern lv_obj_t * uic_Message_Screen_Message_Label;

// EVENTS

extern lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS
LV_IMG_DECLARE(ui_img_bitdog_flattened_png);    // assets/Bitdog_flattened.png
LV_IMG_DECLARE(ui_img_474145878);    // assets/no-wifi-tiny-blue.png
LV_IMG_DECLARE(ui_img_239856029);    // assets/wifi-tiny.png
LV_IMG_DECLARE(ui_img_line_png);    // assets/line.png

// FONTS
LV_FONT_DECLARE(ui_font_Tech);
LV_FONT_DECLARE(ui_font_tech72);

// UI INIT
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

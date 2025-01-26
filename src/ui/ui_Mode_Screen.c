// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 9.1.0
// Project name: SquareLine

#include "ui.h"

void ui_Mode_Screen_screen_init(void)
{
    ui_Mode_Screen = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Mode_Screen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Mode_Screen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Mode_Screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Mode_Screen_Button6 = lv_button_create(ui_Mode_Screen);
    lv_obj_set_width(ui_Mode_Screen_Button6, 100);
    lv_obj_set_height(ui_Mode_Screen_Button6, 50);
    lv_obj_set_x(ui_Mode_Screen_Button6, -84);
    lv_obj_set_y(ui_Mode_Screen_Button6, -72);
    lv_obj_set_align(ui_Mode_Screen_Button6, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Mode_Screen_Button6, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Mode_Screen_Button6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Mode_Screen_Label8 = lv_label_create(ui_Mode_Screen_Button6);
    lv_obj_set_width(ui_Mode_Screen_Label8, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Mode_Screen_Label8, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Mode_Screen_Label8, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Mode_Screen_Label8, "Radio");

    ui_Mode_Screen_Button7 = lv_button_create(ui_Mode_Screen);
    lv_obj_set_width(ui_Mode_Screen_Button7, 100);
    lv_obj_set_height(ui_Mode_Screen_Button7, 50);
    lv_obj_set_x(ui_Mode_Screen_Button7, 71);
    lv_obj_set_y(ui_Mode_Screen_Button7, -72);
    lv_obj_set_align(ui_Mode_Screen_Button7, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Mode_Screen_Button7, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Mode_Screen_Button7, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Mode_Screen_Label9 = lv_label_create(ui_Mode_Screen_Button7);
    lv_obj_set_width(ui_Mode_Screen_Label9, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Mode_Screen_Label9, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Mode_Screen_Label9, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Mode_Screen_Label9, "Game");

    ui_Mode_Screen_Button8 = lv_button_create(ui_Mode_Screen);
    lv_obj_set_width(ui_Mode_Screen_Button8, 100);
    lv_obj_set_height(ui_Mode_Screen_Button8, 50);
    lv_obj_set_x(ui_Mode_Screen_Button8, -86);
    lv_obj_set_y(ui_Mode_Screen_Button8, 37);
    lv_obj_set_align(ui_Mode_Screen_Button8, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Mode_Screen_Button8, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Mode_Screen_Button8, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Mode_Screen_Label10 = lv_label_create(ui_Mode_Screen_Button8);
    lv_obj_set_width(ui_Mode_Screen_Label10, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Mode_Screen_Label10, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Mode_Screen_Label10, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Mode_Screen_Label10, "Proton");

    ui_Mode_Screen_Button9 = lv_button_create(ui_Mode_Screen);
    lv_obj_set_width(ui_Mode_Screen_Button9, 100);
    lv_obj_set_height(ui_Mode_Screen_Button9, 50);
    lv_obj_set_x(ui_Mode_Screen_Button9, 56);
    lv_obj_set_y(ui_Mode_Screen_Button9, 32);
    lv_obj_set_align(ui_Mode_Screen_Button9, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Mode_Screen_Button9, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Mode_Screen_Button9, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Mode_Screen_Label11 = lv_label_create(ui_Mode_Screen_Button9);
    lv_obj_set_width(ui_Mode_Screen_Label11, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Mode_Screen_Label11, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Mode_Screen_Label11, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Mode_Screen_Label11, "Clock");

    uic_Mode_Screen = ui_Mode_Screen;

}

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

    ui_Mode_Screen_Mode_Roller = lv_roller_create(ui_Mode_Screen);
    lv_roller_set_options(ui_Mode_Screen_Mode_Roller, "Radio\nClock\nGame\nProton", LV_ROLLER_MODE_INFINITE);
    lv_roller_set_selected(ui_Mode_Screen_Mode_Roller, 2, LV_ANIM_OFF);
    lv_obj_set_width(ui_Mode_Screen_Mode_Roller, 300);
    lv_obj_set_height(ui_Mode_Screen_Mode_Roller, 162);
    lv_obj_set_x(ui_Mode_Screen_Mode_Roller, -1);
    lv_obj_set_y(ui_Mode_Screen_Mode_Roller, -34);
    lv_obj_set_align(ui_Mode_Screen_Mode_Roller, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(ui_Mode_Screen_Mode_Roller, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Mode_Screen_Mode_Roller, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Mode_Screen_Mode_Roller, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Mode_Screen_Mode_Roller, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Mode_Screen_Cancel_Button = lv_button_create(ui_Mode_Screen);
    lv_obj_set_width(ui_Mode_Screen_Cancel_Button, 135);
    lv_obj_set_height(ui_Mode_Screen_Cancel_Button, 50);
    lv_obj_set_x(ui_Mode_Screen_Cancel_Button, -74);
    lv_obj_set_y(ui_Mode_Screen_Cancel_Button, 82);
    lv_obj_set_align(ui_Mode_Screen_Cancel_Button, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Mode_Screen_Cancel_Button, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Mode_Screen_Cancel_Button, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Mode_Screen_Cancel_Button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Mode_Screen_Cancel_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Mode_Screen_Cancel_Button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Mode_Screen_Cancel_Button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Mode_Screen_Cancel_Button, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_Mode_Screen_Cancel_Button, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_Mode_Screen_Cancel_Button, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_Mode_Screen_Cancel_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Mode_Screen_Cancel_Button_Label = lv_label_create(ui_Mode_Screen_Cancel_Button);
    lv_obj_set_width(ui_Mode_Screen_Cancel_Button_Label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Mode_Screen_Cancel_Button_Label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Mode_Screen_Cancel_Button_Label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Mode_Screen_Cancel_Button_Label, "Cancel");

    ui_Mode_Screen_Select_Button = lv_button_create(ui_Mode_Screen);
    lv_obj_set_width(ui_Mode_Screen_Select_Button, 135);
    lv_obj_set_height(ui_Mode_Screen_Select_Button, 50);
    lv_obj_set_x(ui_Mode_Screen_Select_Button, 71);
    lv_obj_set_y(ui_Mode_Screen_Select_Button, 82);
    lv_obj_set_align(ui_Mode_Screen_Select_Button, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Mode_Screen_Select_Button, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Mode_Screen_Select_Button, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Mode_Screen_Select_Button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Mode_Screen_Select_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Mode_Screen_Select_Button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Mode_Screen_Select_Button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Mode_Screen_Select_Button, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_Mode_Screen_Select_Button, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_Mode_Screen_Select_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Mode_Screen_Select_Button_Label = lv_label_create(ui_Mode_Screen_Select_Button);
    lv_obj_set_width(ui_Mode_Screen_Select_Button_Label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Mode_Screen_Select_Button_Label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Mode_Screen_Select_Button_Label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Mode_Screen_Select_Button_Label, "Select");
    lv_obj_set_style_border_color(ui_Mode_Screen_Select_Button_Label, lv_color_hex(0xFFFFFF),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Mode_Screen_Select_Button_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Mode_Screen_Select_Button_Label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Mode_Screen_Cancel_Button, ui_event_Mode_Screen_Cancel_Button, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Mode_Screen_Select_Button, ui_event_Mode_Screen_Select_Button, LV_EVENT_ALL, NULL);
    uic_Mode_Screen = ui_Mode_Screen;
    uic_Mode_Screen_Mode_Roller = ui_Mode_Screen_Mode_Roller;
    uic_Mode_Screen_Cancel_Button_Label = ui_Mode_Screen_Cancel_Button_Label;
    uic_Mode_Screen_Select_Button = ui_Mode_Screen_Select_Button;
    uic_Mode_Screen_Select_Button_Label = ui_Mode_Screen_Select_Button_Label;

}

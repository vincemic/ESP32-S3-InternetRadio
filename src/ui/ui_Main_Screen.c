// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 9.1.0
// Project name: SquareLine

#include "ui.h"

void ui_Main_Screen_screen_init(void)
{
    ui_Main_Screen = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Main_Screen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Main_Screen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Main_Screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Main_Screen_Artist = lv_label_create(ui_Main_Screen);
    lv_obj_set_width(ui_Main_Screen_Artist, 280);
    lv_obj_set_height(ui_Main_Screen_Artist, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Main_Screen_Artist, 16);
    lv_obj_set_y(ui_Main_Screen_Artist, 107);
    lv_label_set_long_mode(ui_Main_Screen_Artist, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_Main_Screen_Artist, "Beatles");
    lv_obj_remove_flag(ui_Main_Screen_Artist,
                       LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                       LV_OBJ_FLAG_SCROLLABLE);     /// Flags
    lv_obj_set_style_text_color(ui_Main_Screen_Artist, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Main_Screen_Artist, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Main_Screen_Artist, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Main_Screen_Station = lv_label_create(ui_Main_Screen);
    lv_obj_set_width(ui_Main_Screen_Station, 245);
    lv_obj_set_height(ui_Main_Screen_Station, 25);
    lv_obj_set_x(ui_Main_Screen_Station, 16);
    lv_obj_set_y(ui_Main_Screen_Station, 50);
    lv_label_set_long_mode(ui_Main_Screen_Station, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_Main_Screen_Station, "Station");
    lv_obj_remove_flag(ui_Main_Screen_Station,
                       LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_SCROLLABLE);     /// Flags
    lv_obj_set_scroll_dir(ui_Main_Screen_Station, LV_DIR_HOR);
    lv_obj_set_style_text_color(ui_Main_Screen_Station, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Main_Screen_Station, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Main_Screen_Station, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Main_Screen_Commercial = lv_label_create(ui_Main_Screen);
    lv_obj_set_width(ui_Main_Screen_Commercial, 280);
    lv_obj_set_height(ui_Main_Screen_Commercial, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Main_Screen_Commercial, 16);
    lv_obj_set_y(ui_Main_Screen_Commercial, 80);
    lv_label_set_long_mode(ui_Main_Screen_Commercial, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_Main_Screen_Commercial, "commercial");
    lv_obj_set_scroll_dir(ui_Main_Screen_Commercial, LV_DIR_LEFT);
    lv_obj_set_style_text_color(ui_Main_Screen_Commercial, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Main_Screen_Commercial, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Main_Screen_Commercial, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Main_Screen_Title = lv_label_create(ui_Main_Screen);
    lv_obj_set_width(ui_Main_Screen_Title, 280);
    lv_obj_set_height(ui_Main_Screen_Title, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Main_Screen_Title, 19);
    lv_obj_set_y(ui_Main_Screen_Title, 151);
    lv_label_set_long_mode(ui_Main_Screen_Title, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_Main_Screen_Title, "All you need is love");
    lv_obj_remove_flag(ui_Main_Screen_Title,
                       LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_scrollbar_mode(ui_Main_Screen_Title, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_text_color(ui_Main_Screen_Title, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Main_Screen_Title, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Main_Screen_Title, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Main_Screen_No_WIFI_Image = lv_image_create(ui_Main_Screen);
    lv_image_set_src(ui_Main_Screen_No_WIFI_Image, &ui_img_474145878);
    lv_obj_set_width(ui_Main_Screen_No_WIFI_Image, lv_pct(10));
    lv_obj_set_height(ui_Main_Screen_No_WIFI_Image, lv_pct(10));
    lv_obj_set_x(ui_Main_Screen_No_WIFI_Image, 291);
    lv_obj_set_y(ui_Main_Screen_No_WIFI_Image, -3);
    lv_obj_add_flag(ui_Main_Screen_No_WIFI_Image, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_Main_Screen_No_WIFI_Image, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_image_set_scale(ui_Main_Screen_No_WIFI_Image, 100);

    ui_Main_Screen_WIFI_Image = lv_image_create(ui_Main_Screen);
    lv_image_set_src(ui_Main_Screen_WIFI_Image, &ui_img_239856029);
    lv_obj_set_width(ui_Main_Screen_WIFI_Image, lv_pct(10));
    lv_obj_set_height(ui_Main_Screen_WIFI_Image, lv_pct(10));
    lv_obj_set_x(ui_Main_Screen_WIFI_Image, 291);
    lv_obj_set_y(ui_Main_Screen_WIFI_Image, -3);
    lv_obj_add_flag(ui_Main_Screen_WIFI_Image, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_Main_Screen_WIFI_Image, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_image_set_scale(ui_Main_Screen_WIFI_Image, 100);

    ui_Main_Screen_Line_Image = lv_image_create(ui_Main_Screen);
    lv_image_set_src(ui_Main_Screen_Line_Image, &ui_img_line_png);
    lv_obj_set_width(ui_Main_Screen_Line_Image, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Main_Screen_Line_Image, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Main_Screen_Line_Image, -6);
    lv_obj_set_y(ui_Main_Screen_Line_Image, -98);
    lv_obj_set_align(ui_Main_Screen_Line_Image, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Main_Screen_Line_Image, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_Main_Screen_Line_Image, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Main_Screen_Clock_Label = lv_label_create(ui_Main_Screen);
    lv_obj_set_width(ui_Main_Screen_Clock_Label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Main_Screen_Clock_Label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Main_Screen_Clock_Label, -125);
    lv_obj_set_y(ui_Main_Screen_Clock_Label, -111);
    lv_obj_set_align(ui_Main_Screen_Clock_Label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Main_Screen_Clock_Label, "10:10 pm");
    lv_obj_set_style_text_color(ui_Main_Screen_Clock_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Main_Screen_Clock_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Main_Screen_Clock_Label, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Main_Screen_Tune_Button = lv_button_create(ui_Main_Screen);
    lv_obj_set_width(ui_Main_Screen_Tune_Button, 100);
    lv_obj_set_height(ui_Main_Screen_Tune_Button, 38);
    lv_obj_set_x(ui_Main_Screen_Tune_Button, 95);
    lv_obj_set_y(ui_Main_Screen_Tune_Button, 87);
    lv_obj_set_align(ui_Main_Screen_Tune_Button, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Main_Screen_Tune_Button, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Main_Screen_Tune_Button, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Main_Screen_Tune_Button_Label = lv_label_create(ui_Main_Screen_Tune_Button);
    lv_obj_set_width(ui_Main_Screen_Tune_Button_Label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Main_Screen_Tune_Button_Label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Main_Screen_Tune_Button_Label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Main_Screen_Tune_Button_Label, "Tune");

    ui_Main_Screen_Mode_Button = lv_button_create(ui_Main_Screen);
    lv_obj_set_width(ui_Main_Screen_Mode_Button, 100);
    lv_obj_set_height(ui_Main_Screen_Mode_Button, 38);
    lv_obj_set_x(ui_Main_Screen_Mode_Button, -86);
    lv_obj_set_y(ui_Main_Screen_Mode_Button, 87);
    lv_obj_set_align(ui_Main_Screen_Mode_Button, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Main_Screen_Mode_Button, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Main_Screen_Mode_Button, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Main_Screen_Mode_Button_Label = lv_label_create(ui_Main_Screen_Mode_Button);
    lv_obj_set_width(ui_Main_Screen_Mode_Button_Label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Main_Screen_Mode_Button_Label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Main_Screen_Mode_Button_Label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Main_Screen_Mode_Button_Label, "Mode");

    ui_Main_Screen_Message_Label = lv_label_create(ui_Main_Screen);
    lv_obj_set_width(ui_Main_Screen_Message_Label, 200);
    lv_obj_set_height(ui_Main_Screen_Message_Label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Main_Screen_Message_Label, LV_ALIGN_CENTER);
    lv_label_set_long_mode(ui_Main_Screen_Message_Label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_Main_Screen_Message_Label, "Message");
    lv_obj_remove_flag(ui_Main_Screen_Message_Label,
                       LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                       LV_OBJ_FLAG_SNAPPABLE);     /// Flags
    lv_obj_set_style_text_color(ui_Main_Screen_Message_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Main_Screen_Message_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Main_Screen_Message_Label, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Main_Screen_No_WIFI_Image, ui_event_Main_Screen_No_WIFI_Image, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Main_Screen_WIFI_Image, ui_event_Main_Screen_WIFI_Image, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Main_Screen_Tune_Button, ui_event_Main_Screen_Tune_Button, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Main_Screen_Mode_Button, ui_event_Main_Screen_Mode_Button, LV_EVENT_ALL, NULL);
    uic_Main_Screen = ui_Main_Screen;
    uic_Main_Screen_Artist = ui_Main_Screen_Artist;
    uic_Main_Screen_Station = ui_Main_Screen_Station;
    uic_Main_Screen_Commercial = ui_Main_Screen_Commercial;
    uic_Main_Screen_Title = ui_Main_Screen_Title;
    uic_Main_Screen_No_WIFI_Image = ui_Main_Screen_No_WIFI_Image;
    uic_Main_Screen_WIFI_Image = ui_Main_Screen_WIFI_Image;
    uic_Main_Screen_Line_Image = ui_Main_Screen_Line_Image;
    uic_Main_Screen_Clock_Label = ui_Main_Screen_Clock_Label;
    uic_Main_Screen_Tune_Button = ui_Main_Screen_Tune_Button;
    uic_Main_Screen_Tune_Button_Label = ui_Main_Screen_Tune_Button_Label;
    uic_Main_Screen_Mode_Button = ui_Main_Screen_Mode_Button;
    uic_Main_Screen_Mode_Button_Label = ui_Main_Screen_Mode_Button_Label;
    uic_Main_Screen_Message_Label = ui_Main_Screen_Message_Label;

}

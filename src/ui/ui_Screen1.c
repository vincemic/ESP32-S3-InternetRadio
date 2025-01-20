// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 9.1.0
// Project name: SquareLine

#include "ui.h"

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Screen1_Title = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Screen1_Title, 280);
    lv_obj_set_height(ui_Screen1_Title, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Screen1_Title, 18);
    lv_obj_set_y(ui_Screen1_Title, 73);
    lv_label_set_long_mode(ui_Screen1_Title, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_Screen1_Title, "dfsdfsadfasdfasdfadsfasdfasdfasdfasdfasf");
    lv_obj_remove_flag(ui_Screen1_Title, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE);      /// Flags
    lv_obj_set_scrollbar_mode(ui_Screen1_Title, LV_SCROLLBAR_MODE_OFF);

    ui_Screen1_Artist = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Screen1_Artist, 280);
    lv_obj_set_height(ui_Screen1_Artist, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Screen1_Artist, 19);
    lv_obj_set_y(ui_Screen1_Artist, 50);
    lv_label_set_long_mode(ui_Screen1_Artist, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_Screen1_Artist, "123412341234123412341234123412341234123412341234123412341234123412341234");
    lv_obj_remove_flag(ui_Screen1_Artist,
                       LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                       LV_OBJ_FLAG_SCROLLABLE);     /// Flags

    ui_Screen1_Station = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Screen1_Station, 270);
    lv_obj_set_height(ui_Screen1_Station, 25);
    lv_obj_set_x(ui_Screen1_Station, 19);
    lv_obj_set_y(ui_Screen1_Station, 15);
    lv_label_set_long_mode(ui_Screen1_Station, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_Screen1_Station, "station");
    lv_obj_remove_flag(ui_Screen1_Station,
                       LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_SCROLLABLE);    /// Flags
    lv_obj_set_scroll_dir(ui_Screen1_Station, LV_DIR_HOR);
    lv_obj_set_style_text_font(ui_Screen1_Station, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Screen1_Button2 = lv_button_create(ui_Screen1);
    lv_obj_set_width(ui_Screen1_Button2, 100);
    lv_obj_set_height(ui_Screen1_Button2, 50);
    lv_obj_set_x(ui_Screen1_Button2, -5);
    lv_obj_set_y(ui_Screen1_Button2, 10);
    lv_obj_set_align(ui_Screen1_Button2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Screen1_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Screen1_Button2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Screen1_Label1 = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Screen1_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Screen1_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Screen1_Label1, -92);
    lv_obj_set_y(ui_Screen1_Label1, 75);
    lv_obj_set_align(ui_Screen1_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Screen1_Label1, "");

    lv_obj_add_event_cb(ui_Screen1_Button2, ui_event_Screen1_Button2, LV_EVENT_ALL, NULL);

}

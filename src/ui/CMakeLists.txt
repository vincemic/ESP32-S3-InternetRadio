SET(SOURCES ui_Loading_Screen.c
    ui_Main_Screen.c
    ui_Volume_Screen.c
    ui_Network_Screen.c
    ui_Mode_Screen.c
    ui_Station_Selection_Screen.c
    ui_Clock_Screen.c
    ui_Tune_Screen.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_bitdog_flattened_png.c
    ui_img_474145878.c
    ui_img_239856029.c
    ui_img_line_png.c
    fonts/ui_font_Tech.c
    fonts/ui_font_tech72.c)

add_library(ui ${SOURCES})

SET(SOURCES screens/ui_Home.c
    screens/ui_Sensor.c
    screens/ui_Debug.c
    # screens/ui_HearteRate.c
    ui.c
    ui_helpers.c
    # images/ui_img_189584499.c
    fonts/ui_font_Chinese.c
    fonts/ui_font_DateFont.c
    fonts/ui_font_H1.c
    fonts/ui_font_Number_big.c
    fonts/ui_font_Number_extra.c
    fonts/ui_font_SecondFont.c
    fonts/ui_font_Subtitle.c
    fonts/ui_font_TempFont.c
    fonts/ui_font_TempNumberFont.c
    fonts/ui_font_Title.c
    fonts/ui_font_YearFont.c)

add_library(ui ${SOURCES})

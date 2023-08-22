/**
 * @file main.c
 * @brief
 *
 * Copyright (c) 2021 Bouffalolab team
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 *
 */
#include <time.h>
#include "board.h"
#include "bflb_gpio.h"
#include "bflb_l1c.h"
#include "bflb_mtimer.h"

#include "lv_conf.h"
#include "lvgl.h"
// #include "demos/lv_demos.h"
#include "myproject/ui.h"

#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include "lcd.h"

#include "bflb_i2c.h"

#include "bl616_glb.h"
#include "bflb_dma.h"
#include "bflb_i2s.h"



/* lvgl log cb */
void lv_log_print_g_cb(const char *buf)
{
    printf("[LVGL] %s", buf);
}


int main(void)
{

    board_init();
    printf("lvgl case\r\n");
    
    /* lvgl init */
    lv_log_register_print_cb(lv_log_print_g_cb);
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
// #if LV_USE_DEMO_BENCHMARK
//     lv_demo_benchmark();
// #endif

// #if LV_USE_DEMO_WIDGETS
//     // lv_demo_widgets();
//     { // 图片显示
//         lv_obj_t *scr = lv_scr_act();
//         LV_IMG_DECLARE(img_demo_widgets_avatar);
//         lv_obj_t *avatar = lv_img_create(scr);
//         lv_img_set_src(avatar, &img_demo_widgets_avatar);
//     }
// #endif

    // srand((unsigned int)time(NULL));

    ui_init();


    lv_task_handler();

    printf("lvgl success\r\n");

    // ui_UpdateTempuratureChart(36.5);
    // ui_UpdateAccelerationChart(-13, 10, 0.2);

    while (1) {
        lv_task_handler();
        bflb_mtimer_delay_ms(5);

        // ui_UpdateTempuratureChart(rand() % 25 + 20);
        // ui_UpdateAccelerationChart(rand() % 20 - 20, rand() % 20 - 20, rand() % 20 - 20);
        // lv_obj_t *icon = lv_img_create(lv_scr_act());
        // /*From variable*/
        // LV_IMG_DECLARE(img_lvgl_logo);
        // lv_img_set_src(icon, &img_lvgl_logo);
        // lv_obj_align(icon, LV_ALIGN_CENTER, 50, 50);

        // lv_obj_set_size(icon, 80, 40);
    }
}

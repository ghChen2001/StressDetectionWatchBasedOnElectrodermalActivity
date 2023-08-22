cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

sdk_add_include_directories(.)

# sdk_add_compile_definitions(-DCONFIG_CLI_CMD_ENABLE)

sdk_add_include_directories(./AD5940)
sdk_add_include_directories(./LIS2DH12)
# sdk_add_include_directories(./M601)
# sdk_add_include_directories(./M117)
sdk_add_include_directories(./afe4404)
sdk_add_include_directories(./TMP117)
sdk_add_include_directories(./myproject)
# sdk_add_include_directories(./cvxEDA)
# sdk_add_include_directories(./tflite)
sdk_add_include_directories(./Algorithm/my_findpeaks)
# sdk_add_include_directories(./qpOASES/include)
sdk_add_include_directories(./Algorithm/CControl/headers)
# sdk_add_include_directories(./cJSON)


target_sources(app PRIVATE ./AD5940/ad5940.c)
target_sources(app PRIVATE ./AD5940/AD5940Port.c)
target_sources(app PRIVATE ./AD5940/ElectrodermalActivity.c)
target_sources(app PRIVATE ./LIS2DH12/lis2dh12_reg.c)
# target_sources(app PRIVATE ./M601/M601.c)
# target_sources(app PRIVATE ./M117/m117_drv.c)
target_sources(app PRIVATE ./TMP117/tmp117_drv.c)
target_sources(app PRIVATE ./afe4404/AFE4404.c)
target_sources(app PRIVATE ./afe4404/Calibration_AFE4404.c)
target_sources(app PRIVATE ./afe4404/hrm.c)
target_sources(app PRIVATE ./onenet/onenet_post.c)
target_sources(app PRIVATE ./embedded_pedometer/count_steps.c)
# target_sources(app PRIVATE ./Algorithm/sparsEDA/sparsEDA.c)
# target_sources(app PRIVATE ./cJSON/cJSON.c)

file(GLOB_RECURSE CControl "${CMAKE_CURRENT_SOURCE_DIR}/Algorithm/CControl/Sources/*.c")
target_sources(app PRIVATE ${CControl})
# file(GLOB_RECURSE cvxEDA "${CMAKE_CURRENT_SOURCE_DIR}/cvxEDA/*.c")
# target_sources(app PRIVATE ${cvxEDA})
# file(GLOB_RECURSE tflite "${CMAKE_CURRENT_SOURCE_DIR}/tflite/*.cc")
# target_sources(app PRIVATE ${tflite})
file(GLOB_RECURSE findpeaks "${CMAKE_CURRENT_SOURCE_DIR}/Algorithm/my_findpeaks/*.c")
target_sources(app PRIVATE ${findpeaks})

file(GLOB_RECURSE ui "${CMAKE_CURRENT_SOURCE_DIR}/myproject/*.c")
target_sources(app PRIVATE ${ui})

sdk_set_main_file(main.c)

project(ESDC_project)

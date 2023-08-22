cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

sdk_set_main_file(main.c)

sdk_add_include_directories(./AD5940)

target_sources(app PRIVATE ./AD5940/ad5940.c)
target_sources(app PRIVATE ./AD5940/AD5940Port.c)

project(spi_int)

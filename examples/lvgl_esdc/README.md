# lvgl_g0
该样例是在bouffalo_sdk中lvgl样例基础上添加摄像头和液晶显示屏完成图像采集并在LCD上显示，同时触摸可以滑动图片。


## Support CHIP

|      CHIP        | Remark |
|:----------------:|:------:|
|BL602/BL604       |        |
|BL702/BL704/BL706 |        |
|BL616/BL618       |        |
|BL808             |        |

## Compile

- BL602/BL604

```
make CHIP=bl602 BOARD=bl602dk
```

- BL702/BL704/BL706

```
make CHIP=bl702 BOARD=bl702dk
```

- BL616/BL618

```
make CHIP=bl616 BOARD=bl616dk
```

- BL808

```
make CHIP=bl808 BOARD=bl808dk CPU_ID=m0
make CHIP=bl808 BOARD=bl808dk CPU_ID=d0
```

## Flash

```
make flash CHIP=chip_name COMX=xxx # xxx is your com name
```
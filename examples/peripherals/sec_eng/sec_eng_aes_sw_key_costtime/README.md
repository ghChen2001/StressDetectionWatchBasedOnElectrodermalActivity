# sec_eng_aes_sw_key_costtime


## Support CHIP

|      CHIP        | Remark |
|:----------------:|:------:|
|BL602/BL604       | Only support ecb/cbc/ctr      |
|BL702/BL704/BL706 | Only support ecb/cbc/ctr      |
|BL702L/BL704L     | Only support 128bit           |
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

- BL702L/BL704L

```
make CHIP=bl702l BOARD=bl702ldk
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
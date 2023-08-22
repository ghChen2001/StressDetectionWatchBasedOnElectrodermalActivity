#include "M601.h"
#include "board.h"

////////////////////////////////////////////////////////////////////
// 主机发送复位脉冲（大于480us低电平），并检查存在脉冲
bool M601_ow_resetpresence(void)
{
    bool presence;
    int count = 0;
    // printf("Enter\r\n");
    M601_DQ_reset();        //主机拉低DQ总线
    bflb_mtimer_delay_us(480); // 拉低DQ总线持续480us
    M601_DQ_set();        // 主机释放总线，由上拉电阻拉高
    bflb_mtimer_delay_us(80);  // 等待80us后检查presence
    presence = M601_DQ_read(); // 检查存在脉冲
    while (presence && (count < 17)) {
        bflb_mtimer_delay_us(10);
        presence = M601_DQ_read();
        count++;
    }
    bflb_mtimer_delay_us(400); // 时序末尾等待
    return (presence);
} // presence=0时， M601存在, presence=1时，M601不存在

///////////////////////////////////////////////////////////////////////// READ_BIT – 从M601读取一位数
bool M601_read_bit(void)
{
    bool res;
    M601_DQ_reset(); //主机拉低DQ总线
    bflb_mtimer_delay_us(3); //拉低DQ总线持续3us
    M601_DQ_set(); //主机释放DQ总线，由上拉电阻拉高
    bflb_mtimer_delay_us(5); // 等待10us确保总线上数据稳定
    res = M601_DQ_read();
    bflb_mtimer_delay_us(55); //时序末尾等待
    return (res); // 返回读取数值
}

///////////////////////////////////////////////////////////////////////// WRITE_BIT – 主机写一位数据到M601
void M601_write_bit(char bitval)
{
    M601_DQ_reset(); //主机拉低DQ总线
    bflb_mtimer_delay_us(5); //拉低DQ总线持续5us
    if (bitval == 1)
        M601_DQ_set();       // 如果写1，此时拉高DQ总线
    bflb_mtimer_delay_us(60); // 如果写0，则持续拉低DQ总线60us
    M601_DQ_set(); //释放DQ总线
    bflb_mtimer_delay_us(10); //（写0）恢复时间10us
}

///////////////////////////////////////////////////////////////////////// READ_BYTE - 从M601读取一字节数据
unsigned char M601_read_byte(void)
{
    unsigned char i;
    uint8_t value = 0;
    for (i = 0; i < 8; i++) {
        if (M601_read_bit())
            value |= 0x01 << i; //每次读进1 位(低位先行), 然后左移
    }
    return (value);
}

///////////////////////////////////////////////////////////////////////// WRITE_BYTE - 主机写一字节数据到M601
void M601_write_byte(char val)
{
    unsigned char i;
    unsigned char temp;
    for (i = 0; i < 8; i++) { // 写字节,每次一位
        if(val & (0x01 << i))
            M601_write_bit(true);
        else
            M601_write_bit(false);
    }
}

////////////////////////////////////////////////////////////////////
// 计算字节序列的校验和
unsigned char M601_CRC8MY(unsigned char *serial, unsigned char length)
{
    unsigned char result = 0x00;
    unsigned char pDataBuf;
    unsigned char i;

    while (length--) {
        pDataBuf = *serial++;
        for (i = 0; i < 8; i++) {
            if ((result ^ (pDataBuf)) & 0x01) {
                result ^= 0x18;
                result >>= 1;
                result |= 0x80;
            } else {
                result >>= 1;
            }
            pDataBuf >>= 1;
        }
    }
    return result; //返回校验和
}

///////////////////////////////////////////////////////////////////////
void M601_Single_point_Read_Temperature(void)
{
    unsigned char get[9];
    unsigned char tpmsb, tplsb, i;
    short s_tem;
    short wtem;

    float f_tem;

    if (M601_ow_resetpresence()) {
        //建议保留该判断！可以有效排查硬件连接是否出现问题！
        printf("\nPresence Error\n");
    }
    M601_write_byte(0xCC); //Skip ROM，或使用匹配ROM序列号（见后文）
    M601_write_byte(0x44); // 转换温度
    bflb_mtimer_delay_ms(M601_Measure_Period);
    // 温度转换时间，应根据配置寄存器的重复性设置选择4ms，5.5ms或10.5ms。

    if (M601_ow_resetpresence()) {
        //建议保留该判断！可以有效排查硬件连接是否出现问题！
        printf("\nPresence Error\n");
    }
    M601_write_byte(0xCC); // Skip ROM，或使用匹配ROM序列号（见后文）
    M601_write_byte(0xBE); // 读取Scratch Pad中数值

    for (i = 0; i < 9; i++)
        get[i] = M601_read_byte();

    if (get[8] != M601_CRC8MY(get, 8)) {
        //建议保留该判断！可以有效排查软件时序是否出现问题！
        printf("\nCRC Error\n");
    } else {
        tpmsb = get[1]; // 温度高字节
        tplsb = get[0]; // 温度低字节

        wtem = (tpmsb << 8 | tplsb);
        s_tem = (short)wtem;
        f_tem = (s_tem * 1.0) / 256 + 40.0;
        printf("\nTemp= %f degrees C\n", f_tem); //打印摄氏温度
    }
}

///////////////////////////////////////////////////////////////////////
void M601_Read_ROMCode(void)
{
    int n;
    unsigned char rom[8];
    if (M601_ow_resetpresence()) {
        //建议保留该判断！可以有效排查硬件连接是否出现问题！
        printf("\nPresence Error\n");
    }
    M601_write_byte(0x33); //读取ROM指令

    for (n = 0; n < 8; n++) {
        rom[n] = M601_read_byte();
    }

    printf("ROM Code = %X%X%X%X\n", rom[7], rom[6], rom[5], rom[4], rom[3], rom[2], rom[1], rom[0]);
}
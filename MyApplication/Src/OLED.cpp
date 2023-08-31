/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : OLED.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/29 
*********************************************************************
*********
*/
//

#include "OLED.h"

OLED::OLED() {

}

OLED::OLED(GPIO *CS, GPIO *RST, GPIO *RS, GPIO *SCLK, GPIO *SDIN, GPIO *WR, GPIO *RD):
    CS(CS),RST(RST),RS(RS),SCLK(SCLK),SDIN(SDIN)
{
    this->WR=WR;
    this->RD=RD;
    MODE=MODE8080;
    GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOA_CLK_ENABLE();     /* 使能PORTA时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();     /* 使能PORTB时钟 */
    __HAL_RCC_GPIOC_CLK_ENABLE();     /* 使能PORTC时钟 */
    __HAL_RCC_GPIOD_CLK_ENABLE();     /* 使能PORTD时钟 */
    __HAL_RCC_GPIOE_CLK_ENABLE();     /* 使能PORTE时钟 */
    __HAL_RCC_GPIOG_CLK_ENABLE();     /* 使能PORTG时钟 */

    /* PA4 设置 */
    gpio_init_struct.Pin = GPIO_PIN_4;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;    /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;            /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;  /* 高速 */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    /* PB6, PB7 设置 */
    gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);

    /* PC6~9, PC11 设置*/
    gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_11;
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);

    /* PD6, PD7 设置 */
    gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOD, &gpio_init_struct);

    /* PE5, PE6 设置 */
    gpio_init_struct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);

    /* PG15 设置 */
    gpio_init_struct.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOG, &gpio_init_struct);
    oledInit();
}

OLED::OLED(GPIO *CS, GPIO *D0,GPIO * RS,GPIO* RST,GPIO* D1) : MySPI(CS, D0,NULL,D1) {
    MODE=MODESPI;
    SCLK=D0;
    this->SDIN=D1;
    this->CS=CS;
    this->RS=RS;
    this->RST=RST;
    SoftSPIModeSet(SPIMode3);
    oledInit();
}

OLED::OLED(GPIO *SDA, GPIO *SCL, uint8_t SlaveAddress, uint8_t REGAddress) : MyIIc(SDA, SCL, SlaveAddress, REGAddress) {
    MODE=MODEIIC;
    oledInit();
}

void OLED::oledWRByte(uint8_t data, uint8_t cmd) {
    switch (MODE) {
        case MODE8080:{
            oled8080WRByte(data,cmd);
        }
            break;
        case MODESPI:{
            oledSPIWRByte(data,cmd);
        }
            break;
        case MODEIIC:{
            oledIICWRByte(data,cmd);
        }
            break;
    }

}

void OLED::oled8080WRByte(uint8_t data, uint8_t cmd) {
    Dataout8080(data);
    cmd?RS->High():RS->Low();
    CS->Low();
    WR->Low();
    WR->High();
    CS->High();
    RS->High();
}

void OLED::oledSPIWRByte(uint8_t data, uint8_t cmd) {
//    uint8_t i;
    cmd?RS->High():RS->Low();
    CS->Low();
    uint8_t i;
    cmd?RS->High():RS->Low();
    CS->Low();
    for (i = 0; i < 8;i++) {
        SCLK->Low();
        (data&0x80)?SDIN->High():SDIN->Low();
        SCLK->High();
        data<<=1;
    }
    CS->High();
    RS->High();
  //  SoftSPIWR(data);
//    CS->High();
//    RS->High();
}

void OLED::oledIICWRByte(uint8_t data, uint8_t cmd) {

}

void OLED::oledRefreshGram() {
    uint8_t i,n;
    for (i = 0; i < 8; ++i) {
        oledWRByte(0xb0+i,OLED_CMD);
        oledWRByte(0x00,OLED_CMD);
        oledWRByte(0x10,OLED_CMD);
        for (n = 0; n < 128; ++n) {
            oledWRByte(g_oled_gram[n][i],OLED_DATA);
        }
    }
}

void OLED::Dataout8080(uint8_t data) {
    uint16_t dat = data & 0X0F;
    GPIOC->ODR &= ~(0XF << 6);          /* 清空6~9 */
    GPIOC->ODR |= dat << 6;             /* D[3:0]-->PC[9:6] */

    GPIOC->ODR &= ~(0X1 << 11);         /* 清空11 */
    GPIOC->ODR |= ((data >> 4) & 0x01) << 11;

    GPIOB->ODR &= ~(0X1 << 6);          /* 清空6 */
    GPIOB->ODR |= ((data >> 5) & 0x01) << 6;

    GPIOE->ODR &= ~(0X3 << 5);          /* 清空5,6 */
    GPIOE->ODR |= ((data >> 6) & 0x01) << 5;
    GPIOE->ODR |= ((data >> 7) & 0x01) << 6;
}
MyUsart myoledUsart(&huart1);
void OLED::oledInit() {
//    OLED_CS(1);
//    OLED_RS(1);
//
//    OLED_RST(0);
//    delay_ms(100);
//    OLED_RST(1);
    CS->High();
    RS->High();
    RST->Low();
    HAL_Delay(1000);
    RST->High();
    myoledUsart<<"1"<<endl;
    oledWRByte(0xAE, OLED_CMD);   /* 关闭显示 */

    oledWRByte(0xD5, OLED_CMD);   /* 设置时钟分频因子,震荡频率 */

    oledWRByte(80, OLED_CMD);     /* [3:0],分频因子;[7:4],震荡频率 */
    oledWRByte(0xA8, OLED_CMD);   /* 设置驱动路数 */
    oledWRByte(0X3F, OLED_CMD);   /* 默认0X3F(1/64) */
    oledWRByte(0xD3, OLED_CMD);   /* 设置显示偏移 */  myoledUsart<<"2"<<endl;
    oledWRByte(0X00, OLED_CMD);   /* 默认为0 */

    oledWRByte(0x40, OLED_CMD);   /* 设置显示开始行 [5:0],行数. */

    oledWRByte(0x8D, OLED_CMD);   /* 电荷泵设置 */
    oledWRByte(0x14, OLED_CMD);   /* bit2，开启/关闭 */
    oledWRByte(0x20, OLED_CMD);   /* 设置内存地址模式 */
    oledWRByte(0x02, OLED_CMD);   /* [1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10; */
    oledWRByte(0xA1, OLED_CMD);   /* 段重定义设置,bit0:0,0->0;1,0->127; */
    oledWRByte(0xC8, OLED_CMD);   /* 设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数 */
    oledWRByte(0xDA, OLED_CMD);   /* 设置COM硬件引脚配置 */
    oledWRByte(0x12, OLED_CMD);   /* [5:4]配置 */
    myoledUsart<<"3"<<endl;
    oledWRByte(0x81, OLED_CMD);   /* 对比度设置 */
    oledWRByte(0xEF, OLED_CMD);   /* 1~255;默认0X7F (亮度设置,越大越亮) */
    oledWRByte(0xD9, OLED_CMD);   /* 设置预充电周期 */
    oledWRByte(0xf1, OLED_CMD);   /* [3:0],PHASE 1;[7:4],PHASE 2; */
    oledWRByte(0xDB, OLED_CMD);   /* 设置VCOMH 电压倍率 */
    oledWRByte(0x30, OLED_CMD);   /* [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc; */
    myoledUsart<<4<<endl;
    oledWRByte(0xA4, OLED_CMD);   /* 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏) */
    oledWRByte(0xA6, OLED_CMD);   /* 设置显示方式;bit0:1,反相显示;0,正常显示 */
    oledWRByte(0xAF, OLED_CMD);   /* 开启显示 */
    oled_clear();
    myoledUsart<<5<<endl;
}

void OLED::oled_clear(void) {
    uint8_t i, n;
    for (i = 0; i < 8; i++)for (n = 0; n < 128; n++)g_oled_gram[n][i] = 0X00;
    oledRefreshGram();    /* 更新显示 */
}

void OLED::oled_display_on(void) {
    oledWRByte(0X8D, OLED_CMD);   /* SET DCDC命令 */
    oledWRByte(0X14, OLED_CMD);   /* DCDC ON */
    oledWRByte(0XAF, OLED_CMD);   /* DISPLAY ON */
}

void OLED::oled_display_off(void) {
   oledWRByte(0X8D, OLED_CMD);   /* SET DCDC命令 */
   oledWRByte(0X10, OLED_CMD);   /* DCDC OFF */
   oledWRByte(0XAE, OLED_CMD);   /* DISPLAY OFF */
}

void OLED::oled_draw_point(uint8_t x, uint8_t y, uint8_t dot) {
    uint8_t pos, bx, temp = 0;

    if (x > 127 || y > 63) return;  /* 超出范围了. */

    pos = y / 8;        /* 计算GRAM里面的y坐标所在的字节, 每个字节可以存储8个行坐标 */

    bx = y % 8;         /* 取余数,方便计算y在对应字节里面的位置,及行(y)位置 */
    temp = 1 << bx;     /* 高位表示低行号, 得到y对应的bit位置,将该bit先置1 */

    if (dot)                        /* 画实心点 */
    {
        g_oled_gram[x][pos] |= temp;
    }
    else                            /* 画空点,即不显示 */
    {
        g_oled_gram[x][pos] &= ~temp;
    }
    //oledRefreshGram();
}

void OLED::oled_fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot) {
    uint8_t x, y;

    for (x = x1; x <= x2; x++)
    {
        for (y = y1; y <= y2; y++)oled_draw_point(x, y, dot);
    }

    oledRefreshGram();    /* 更新显示 */
}



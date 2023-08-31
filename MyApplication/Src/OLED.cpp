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
    __HAL_RCC_GPIOA_CLK_ENABLE();     /* ʹ��PORTAʱ�� */
    __HAL_RCC_GPIOB_CLK_ENABLE();     /* ʹ��PORTBʱ�� */
    __HAL_RCC_GPIOC_CLK_ENABLE();     /* ʹ��PORTCʱ�� */
    __HAL_RCC_GPIOD_CLK_ENABLE();     /* ʹ��PORTDʱ�� */
    __HAL_RCC_GPIOE_CLK_ENABLE();     /* ʹ��PORTEʱ�� */
    __HAL_RCC_GPIOG_CLK_ENABLE();     /* ʹ��PORTGʱ�� */

    /* PA4 ���� */
    gpio_init_struct.Pin = GPIO_PIN_4;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;    /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;            /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;  /* ���� */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    /* PB6, PB7 ���� */
    gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);

    /* PC6~9, PC11 ����*/
    gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_11;
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);

    /* PD6, PD7 ���� */
    gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOD, &gpio_init_struct);

    /* PE5, PE6 ���� */
    gpio_init_struct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);

    /* PG15 ���� */
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
    GPIOC->ODR &= ~(0XF << 6);          /* ���6~9 */
    GPIOC->ODR |= dat << 6;             /* D[3:0]-->PC[9:6] */

    GPIOC->ODR &= ~(0X1 << 11);         /* ���11 */
    GPIOC->ODR |= ((data >> 4) & 0x01) << 11;

    GPIOB->ODR &= ~(0X1 << 6);          /* ���6 */
    GPIOB->ODR |= ((data >> 5) & 0x01) << 6;

    GPIOE->ODR &= ~(0X3 << 5);          /* ���5,6 */
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
    oledWRByte(0xAE, OLED_CMD);   /* �ر���ʾ */

    oledWRByte(0xD5, OLED_CMD);   /* ����ʱ�ӷ�Ƶ����,��Ƶ�� */

    oledWRByte(80, OLED_CMD);     /* [3:0],��Ƶ����;[7:4],��Ƶ�� */
    oledWRByte(0xA8, OLED_CMD);   /* ��������·�� */
    oledWRByte(0X3F, OLED_CMD);   /* Ĭ��0X3F(1/64) */
    oledWRByte(0xD3, OLED_CMD);   /* ������ʾƫ�� */  myoledUsart<<"2"<<endl;
    oledWRByte(0X00, OLED_CMD);   /* Ĭ��Ϊ0 */

    oledWRByte(0x40, OLED_CMD);   /* ������ʾ��ʼ�� [5:0],����. */

    oledWRByte(0x8D, OLED_CMD);   /* ��ɱ����� */
    oledWRByte(0x14, OLED_CMD);   /* bit2������/�ر� */
    oledWRByte(0x20, OLED_CMD);   /* �����ڴ��ַģʽ */
    oledWRByte(0x02, OLED_CMD);   /* [1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10; */
    oledWRByte(0xA1, OLED_CMD);   /* ���ض�������,bit0:0,0->0;1,0->127; */
    oledWRByte(0xC8, OLED_CMD);   /* ����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·�� */
    oledWRByte(0xDA, OLED_CMD);   /* ����COMӲ���������� */
    oledWRByte(0x12, OLED_CMD);   /* [5:4]���� */
    myoledUsart<<"3"<<endl;
    oledWRByte(0x81, OLED_CMD);   /* �Աȶ����� */
    oledWRByte(0xEF, OLED_CMD);   /* 1~255;Ĭ��0X7F (��������,Խ��Խ��) */
    oledWRByte(0xD9, OLED_CMD);   /* ����Ԥ������� */
    oledWRByte(0xf1, OLED_CMD);   /* [3:0],PHASE 1;[7:4],PHASE 2; */
    oledWRByte(0xDB, OLED_CMD);   /* ����VCOMH ��ѹ���� */
    oledWRByte(0x30, OLED_CMD);   /* [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc; */
    myoledUsart<<4<<endl;
    oledWRByte(0xA4, OLED_CMD);   /* ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����) */
    oledWRByte(0xA6, OLED_CMD);   /* ������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ */
    oledWRByte(0xAF, OLED_CMD);   /* ������ʾ */
    oled_clear();
    myoledUsart<<5<<endl;
}

void OLED::oled_clear(void) {
    uint8_t i, n;
    for (i = 0; i < 8; i++)for (n = 0; n < 128; n++)g_oled_gram[n][i] = 0X00;
    oledRefreshGram();    /* ������ʾ */
}

void OLED::oled_display_on(void) {
    oledWRByte(0X8D, OLED_CMD);   /* SET DCDC���� */
    oledWRByte(0X14, OLED_CMD);   /* DCDC ON */
    oledWRByte(0XAF, OLED_CMD);   /* DISPLAY ON */
}

void OLED::oled_display_off(void) {
   oledWRByte(0X8D, OLED_CMD);   /* SET DCDC���� */
   oledWRByte(0X10, OLED_CMD);   /* DCDC OFF */
   oledWRByte(0XAE, OLED_CMD);   /* DISPLAY OFF */
}

void OLED::oled_draw_point(uint8_t x, uint8_t y, uint8_t dot) {
    uint8_t pos, bx, temp = 0;

    if (x > 127 || y > 63) return;  /* ������Χ��. */

    pos = y / 8;        /* ����GRAM�����y�������ڵ��ֽ�, ÿ���ֽڿ��Դ洢8�������� */

    bx = y % 8;         /* ȡ����,�������y�ڶ�Ӧ�ֽ������λ��,����(y)λ�� */
    temp = 1 << bx;     /* ��λ��ʾ���к�, �õ�y��Ӧ��bitλ��,����bit����1 */

    if (dot)                        /* ��ʵ�ĵ� */
    {
        g_oled_gram[x][pos] |= temp;
    }
    else                            /* ���յ�,������ʾ */
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

    oledRefreshGram();    /* ������ʾ */
}



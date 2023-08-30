/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : OLED.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/29 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_OLED_H
#define MY_LIBARARY_OLED_H
#include "MySPI.h"
#include "MyIIc.h"
#define OLED_CMD        0       /* д���� */
#define OLED_DATA       1       /* д���� */

enum OLEDMode{
    MODE8080=0,
    MODESPI,
    MODEIIC,
};
class OLED :public MySPI, public MyIIc{
public:
    OLED();
    OLED(GPIO* CS,GPIO* RST,GPIO * RS ,GPIO * SCLK,GPIO* SDIN,GPIO * WR,GPIO* RD);//8080����
    OLED(GPIO *CS, GPIO *DO,GPIO * RS,GPIO* RST,GPIO* D1);//���spi
    void oled_clear(void);          /* OLED���� */
    void oled_display_on(void);     /* ����OLED��ʾ */
    void oled_display_off(void);    /* �ر�OLED��ʾ */
   // void oled_refresh_gram(void);
    void oled_draw_point(uint8_t x, uint8_t y, uint8_t dot);    /* OLED���� */
    void oled_fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);        /* OLED������� */
    void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode); /* OLED��ʾ�ַ� */
    void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);  /* OLED��ʾ���� */
    void oled_show_string(uint8_t x, uint8_t y, const char *p, uint8_t size);           /* OLED��ʾ�ַ��� */
#ifdef HAL_SPI_ERROR_NONE
    OLED(SPI_HandleTypeDef* SPISelect,GPIO *CS);//Ӳ��spi
#endif
    OLED(GPIO *SDA,GPIO* SCL,uint8_t SlaveAddress,uint8_t REGAddress);//���iic
protected:
    void oledWRByte(uint8_t data,uint8_t cmd);
private:
    void oledInit();
    void Dataout8080(uint8_t data);
    void oledRefreshGram(); /* �����Դ浽OLED */
    void oled8080WRByte(uint8_t data,uint8_t cmd);
    void oledSPIWRByte(uint8_t data,uint8_t cmd);
    void oledIICWRByte(uint8_t data,uint8_t cmd);
    uint8_t g_oled_gram[128][8];
    GPIO *CS;GPIO* RST;GPIO * RS ;GPIO * SCLK;
    GPIO *SDIN;GPIO* WR;GPIO *RD;
    OLEDMode MODE;
};


#endif //MY_LIBARARY_OLED_H

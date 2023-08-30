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
#define OLED_CMD        0       /* 写命令 */
#define OLED_DATA       1       /* 写数据 */

enum OLEDMode{
    MODE8080=0,
    MODESPI,
    MODEIIC,
};
class OLED :public MySPI, public MyIIc{
public:
    OLED();
    OLED(GPIO* CS,GPIO* RST,GPIO * RS ,GPIO * SCLK,GPIO* SDIN,GPIO * WR,GPIO* RD);//8080串行
    OLED(GPIO *CS, GPIO *DO,GPIO * RS,GPIO* RST,GPIO* D1);//软件spi
    void oled_clear(void);          /* OLED清屏 */
    void oled_display_on(void);     /* 开启OLED显示 */
    void oled_display_off(void);    /* 关闭OLED显示 */
   // void oled_refresh_gram(void);
    void oled_draw_point(uint8_t x, uint8_t y, uint8_t dot);    /* OLED画点 */
    void oled_fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);        /* OLED区域填充 */
    void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode); /* OLED显示字符 */
    void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);  /* OLED显示数字 */
    void oled_show_string(uint8_t x, uint8_t y, const char *p, uint8_t size);           /* OLED显示字符串 */
#ifdef HAL_SPI_ERROR_NONE
    OLED(SPI_HandleTypeDef* SPISelect,GPIO *CS);//硬件spi
#endif
    OLED(GPIO *SDA,GPIO* SCL,uint8_t SlaveAddress,uint8_t REGAddress);//软件iic
protected:
    void oledWRByte(uint8_t data,uint8_t cmd);
private:
    void oledInit();
    void Dataout8080(uint8_t data);
    void oledRefreshGram(); /* 更新显存到OLED */
    void oled8080WRByte(uint8_t data,uint8_t cmd);
    void oledSPIWRByte(uint8_t data,uint8_t cmd);
    void oledIICWRByte(uint8_t data,uint8_t cmd);
    uint8_t g_oled_gram[128][8];
    GPIO *CS;GPIO* RST;GPIO * RS ;GPIO * SCLK;
    GPIO *SDIN;GPIO* WR;GPIO *RD;
    OLEDMode MODE;
};


#endif //MY_LIBARARY_OLED_H

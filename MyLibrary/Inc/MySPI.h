/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MySPI.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/21 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_MYSPI_H
#define MY_LIBARARY_MYSPI_H
#include "main.h"
#include "MyGPIO.h"
#include "stm32f4xx_hal_spi.h"

typedef enum ModeSelect{
    HardwareSPI=0,
    SoftwareSPI,
}modeSelect;

class MySPI{
public:
    MySPI();
    void ModeSet(ModeSelect modeSelect);
    ~MySPI();
private:
    ModeSelect Mode;
};



#endif //MY_LIBARARY_MYSPI_H

/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : Startup.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/11 
*********************************************************************
*********
*/
#include "ConnectCplusplus.h"
#include "ALLfile.h"

using namespace stm32f407;
MyUsart myUart(&huart1);
void MyMain(){
    GPIO CS(PB,14,GPIO_MODE_OUTPUT_PP,GPIO_SPEED_FREQ_VERY_HIGH);
    MySPI mySpi(HardwareSpi1,&CS);
//    mySpi.GPIOInit();
//    mySpi.SPIInit();
    mySpi.SPISpeedSet(SPI_SPEED_4);
    uint16_t drivesId=0;
    CS.Low();
    mySpi.SPIWriteRead(0x90);
    mySpi.SPIWriteRead(0x00);
    mySpi.SPIWriteRead(0x00);
    mySpi.SPIWriteRead(0x00);
    drivesId=mySpi.SPIWriteRead(0xff)<<8;
    drivesId|=mySpi.SPIWriteRead(0xff);
    CS.High();
    myUart.print(drivesId,HEX);



    for (;;){


    }


}
void Interrupt_management(DMA_HandleTypeDef * hdma_usartx_rx){
    myUart.ReceiveDataCallBack(hdma_usartx_rx);
}
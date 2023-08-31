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
 char g_text_buf[] = {"STM32 SPI TEST"};
#define TEXT_SIZE   sizeof(g_text_buf)
MyUsart myUart(&huart1);
void MyMain(){
    uint16_t id=0;
    uint32_t flashsize=0;
    char datatemp[TEXT_SIZE];


    GPIO LED1(PF,9);
    GPIO CS(PB,14);
    GPIO SCK(PB,3);
    GPIO MISO(PB,4,GPIO_MODE_INPUT);
    GPIO MOSI(PB,5);
    Flash myFlash(&CS,&SCK,&MISO,&MOSI);
    myFlash.SoftSPIModeSet(SPIMode3);
   myUart<<"STM32 SPI test"<<endl;
   id=myFlash.norflash_read_id();
    while ((id==0)||(id==0xffff)){
        myUart<<"spi check fail"<<endl;
        HAL_Delay(200);
        myUart<<"please check"<<endl;
        LED1.TimingToggle();
    }
    myUart.print(id,HEX);
    myUart<<endl;
    myUart<<"SPI ready"<<endl;
   flashsize = 16 * 1024 * 1024;

    for (;;){
//        if (Key1.IsLow()){
//            HAL_Delay(100);
//            myUart<<"start write"<<endl;
//            myFlash.norflash_write(g_text_buf, flashsize - 100, TEXT_SIZE);
//            myUart<<"send ok"<<endl;
//
//        }
//        if(Key2.IsLow()){
//            HAL_Delay(100);
//            myUart<<"start read"<<endl;
//            myFlash.norflash_read(datatemp,flashsize-100,TEXT_SIZE);
//            myUart<<datatemp<<endl;
//            myUart<<"read ok"<<endl;
//        }

    }


}
void Interrupt_management(DMA_HandleTypeDef * hdma_usartx_rx){
    myUart.ReceiveDataCallBack(hdma_usartx_rx);
}
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
#include "touch.h"
#include "LVppDisplayer.h"
#include "lvglpp/lvglpp.h"

using namespace stm32f407;
using namespace lvgl;
using namespace lvgl::core;
using namespace lvgl::misc;
using namespace lvgl::widgets;
 char g_text_buf[] = {"STM32 SPI TEST"};
#define TEXT_SIZE   sizeof(g_text_buf)
MyUsart myUart(&huart1);
void MyMain(){
    

    for (;;){

    }


}
void Interrupt_management(DMA_HandleTypeDef * hdma_usartx_rx){
    myUart.ReceiveDataCallBack(hdma_usartx_rx);
}
void KeyTimeBenchmark(){
        buttonManage.buttonTick();
}
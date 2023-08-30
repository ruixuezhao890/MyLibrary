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



    for (;;){


    }


}
void Interrupt_management(DMA_HandleTypeDef * hdma_usartx_rx){
    myUart.ReceiveDataCallBack(hdma_usartx_rx);
}
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
void btn_event_callback(lv_event_t* event){
    myUart<<"screen button pressDown"<<endl;
    myUart<<"test OK"<<endl;
}
    void PressDownHandle(){
        myUart<<"button pressDown"<<endl;
        myUart<<"test OK"<<endl;
    }
    void Pressrepet(){


            myUart<<"repet"<<endl;

    }
    void Longpress(){
            myUart<<"Longpress"<<endl;
    }
void MyMain(){


    MyButton KEY1(PE,3,0);
    KEY1.MyButtonTaskRegister(PRESS_DOWN,PressDownHandle);
    KEY1.MyButtonTaskRegister(stm32f407::DOUBLE_CLICK,Pressrepet);
    KEY1.MyButtonTaskRegister(stm32f407::LONG_PRESS_HOLD,Longpress);
    KEY1.MyButtonEnable();


    for (;;){
        lv_timer_handler();

    }


}
void Interrupt_management(DMA_HandleTypeDef * hdma_usartx_rx){
    myUart.ReceiveDataCallBack(hdma_usartx_rx);
}
void KeyTimeBenchmark(){
        buttonManage.buttonTick();
    }
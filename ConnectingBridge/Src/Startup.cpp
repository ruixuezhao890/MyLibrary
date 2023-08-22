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
#include "MyGPIO.h"
#include "MyUsart.h"
#include "MyIIc.h"
#include "WString.h"
#include "tim.h"

using namespace stm32f407;
MyUsart myUart(&huart1);
void MyMain(){


    uint16_t i=0;
    uint8_t pouttChar=10;
    uint8_t pinputChar=0;
    GPIO SCL(PB,8);
    GPIO SDA(PB,9,GPIO_MODE_OUTPUT_OD);
    GPIO Sck(PA,2|3,GPIO_MODE_AF_PP);
    MyIIc myIIc(&SDA,&SCL,0xA0,0);
    myUart.ReceiveInit();
    uint8_t temp;
    //char SendData[]="1234";
    float SendData=5.14;
    float ReceiveData=0;
   // uint16_t strLen= sizeof(SendData);
    //char ReceiveData[12];
    char ReceiveData1[24]="hello\nxxx";
    temp=myIIc.SingleRead(myIIc.GetSlaveAddress(),myIIc.GetREGAddress());
    if (temp==0x55){
        myUart<<"Check ok1"<<endl;
    }
    else{
        myIIc.SingleWrite(myIIc.GetSlaveAddress(),myIIc.GetREGAddress(),0x55);
        temp=myIIc.SingleRead(myIIc.GetSlaveAddress(),myIIc.GetREGAddress());
        if (temp==0x55){
            myUart<<"Check ok2"<<endl;
        }
    }
    myIIc<<SendData;
    //myIIc.MultipleRead(myIIc.GetSlaveAddress(),myIIc.GetREGAddress(),ReceiveData1,myIIc.GetSize());
    HAL_Delay_ms(10);
    myIIc>>ReceiveData;
    myUart<<ReceiveData<<endl;
    //myUart<<3.14;



    for (;;){
        if (myUart.GetRECV()){
            myUart>>ReceiveData1;
            myUart<<ReceiveData1;
            myUart.ReceiveAgain();
        }

    }


}
void Interrupt_management(DMA_HandleTypeDef * hdma_usartx_rx){
    myUart.ReceiveDataCallBack(hdma_usartx_rx);
}
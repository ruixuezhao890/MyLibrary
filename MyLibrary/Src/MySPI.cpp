/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MySPI.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/21 
*********************************************************************
*********
*/
//

#include "MySPI.h"

MySPI::MySPI() {

}

MySPI::~MySPI() {

}
#ifdef HAL_SPI_ERROR_NONE
MySPI::MySPI(SPI_HandleTypeDef* SPISelect, GPIO *CS) {
    this->g_spi_handler=SPISelect;
    this->CS=CS;
    this->Mode=HardwareSPI;
    buff.write_index=buff.read_index=buff.data_size=0;
    for (int i = 0; i < 256; ++i)
        buff.buf[i]=0;
}
uint8_t MySPI::SPIWriteRead(uint8_t SendData) {

    uint8_t receiveData = 0;
    HAL_SPI_TransmitReceive((this->g_spi_handler), &SendData, &receiveData, 1, 1000);
    return receiveData;


}
#endif

MySPI::MySPI(GPIO *CS, GPIO *SCK, GPIO *MISO, GPIO *MOSI) {
    this->Mode=SoftwareSPI;
    this->CS=CS;
    this->SCK=SCK;
    this->MOSI=MOSI;
    this->MISO=MISO;
    SoftSPIInit();
}

void MySPI::SoftSPIInit() {
    if (Mode==SoftwareSPI){
        CS->High();
       SoftGPIOInit();
    }

}

void MySPI::SoftGPIOInit() {
    switch (SoftSPIMode) {
        case SPIMode0:{
            SCK->Low();
        }
            break;
        case SPIMode1:{
            SCK->Low();
        }
            break;
        case SPIMode2:{
            SCK->High();
        }
            break;
        case SPIMode3:{
            SCK->High();
        }
            break;

    }
}



size_t MySPI::write(uint8_t Send) {

    if (Mode==HardwareSPI){
#ifdef HAL_SPI_ERROR_NONE
        HAL_SPI_Transmit(this->g_spi_handler,&Send,1,1000);
         return 1;
#endif


    }
    if (Mode==SoftwareSPI){
        this->SoftSPIWR(Send);
        return 1;
    }
    return 0;
}

size_t MySPI::write(const uint8_t *SendData, size_t size) {
    if (Mode==HardwareSPI){
#ifdef HAL_SPI_ERROR_NONE
        HAL_SPI_Transmit(this->g_spi_handler,(uint8_t *)SendData,size,1000);
         return 1;
#endif

    }
    if (Mode==SoftwareSPI)
    {
        for (int i = 0; i < size; ++i) {
            this->SoftSPIWR(SendData[i]);
        }
        return 1;
    }
    return 0;
}

void MySPI::SetCS(uint8_t set) {
    if (set){
        CS->High();
        return;
    }
    CS->Low();
}

int MySPI::read() {
    uint8_t Trigger=0xff;
    if (Mode==HardwareSPI) {
#ifdef HAL_SPI_ERROR_NONE
        if (CMDFlag) {
//            for (int i = 0; i < 2; ++i) {
//                if (!i) {
//                    // buff.buf[buff.read_index]
//                    receive = this->SPIWriteRead(Trigger) << 8;
//                } else {
//                    // buff.buf[buff.read_index]
//                    receive |= this->SPIWriteRead(Trigger);
//                }
//            }
////        buff.read_index++;
//            return receive;
////        buff.buf[buff.read_index - 1];
//        } else {

            HAL_SPI_TransmitReceive(this->g_spi_handler, &Trigger, &buff.buf[buff.read_index++], 1, 1000);
            return buff.buf[buff.read_index - 1];
        }
#endif
    }
    if(Mode==SoftwareSPI)
    {
        buff.buf[buff.read_index++]=this->SoftSPIWR(Trigger);
        return buff.buf[buff.read_index-1];
    }
    return 0;
}

void MySPI::SoftSPIModeSet(SoftMode Set) {
    this->SoftSPIMode=Set;
}

SoftMode MySPI::GetSoftSPIMode() {
    return this->SoftSPIMode;
}

uint8_t MySPI::SoftSPIWR(uint8_t SendData) {
    uint8_t temp=0;
    switch (SoftSPIMode) {
        case SPIMode0:{
            temp=SoftSPIMode0WR(SendData);
        }
            break;
        case SPIMode1:{
            temp=SoftSPIMode1WR(SendData);
        }
            break;
        case SPIMode2:{
            temp= SoftSPIMode2WR(SendData);
        }
            break;
        case SPIMode3:{
            temp=SoftSPIMode3WR(SendData);
        }
            break;

    }
    return temp;
}
//CPOL=0;CPHA=0
uint8_t MySPI::SoftSPIMode0WR(uint8_t SendData) {
    uint8_t i,temp=0;
    for (i = 0; i < 8; ++i) {
        if (SendData&0x80)MOSI->High();
        else MOSI->Low();
        SendData<<=1;
        SPIDelay();
        SCK->High();
        temp<<=1;
        if (MISO->IsHigh()) temp++;
        SPIDelay();
        SCK->Low();
        SPIDelay();
    }
    return temp;
//    uint8_t i, read_dat;
//    for( i = 0; i < 8; i++ )
//    {
//        if( SendData & 0x80 )
//            MOSI->High();
//        else
//            MOSI->Low();
//        SendData <<= 1;
//        HAL_Delay_us(1);
//        SCK->High();
//        read_dat <<= 1;
//        if( MISO )
//            read_dat++;
//        HAL_Delay_us(1);
//        SCK->Low();
//        HAL_Delay_us(1);
//    }
//
//    return read_dat;

}
//CPOL=0;CPHA=1
uint8_t MySPI::SoftSPIMode1WR(uint8_t SendData) {
    uint8_t i,temp=0;
    for (i = 0; i < 8; ++i) {
        SCK->High();
        if (SendData&0x80)MOSI->High();
        else MOSI->Low();
        SendData<<=1;
        SPIDelay();
        SCK->Low();
        temp<<=1;
        if (MISO->IsHigh()) temp++;
        SPIDelay();
    }
    return temp;
}
//CPOL=1;CPHA=0
uint8_t MySPI::SoftSPIMode2WR(uint8_t SendData) {
    uint8_t i,temp=0;
    for (i = 0; i < 8; ++i) {
        if (SendData&0x80)MOSI->High();
        else MOSI->Low();
        SendData<<=1;
        SPIDelay();
        SCK->Low();
        temp<<=1;
        if (MISO->IsHigh()) temp++;
        SPIDelay();
        SCK->High();
//        HAL_Delay_us(1);
    }
    return temp;
}
//CPOL=1;CPHA=1
uint8_t MySPI::SoftSPIMode3WR(uint8_t SendData) {
    uint8_t i,temp=0;
    for (i = 0; i < 8; ++i) {
        SCK->Low();
        SPIDelay();
         (SendData&0x80)?MOSI->High():MOSI->Low();
        SendData<<=1;
        SPIDelay();
        SCK->High();
        SPIDelay();
        temp<<=1;
        if (MISO->IsHigh()) temp++;
        SPIDelay();
    }
    //SCK->High();
    return temp;
}

void MySPI::SoftSPISpeedSet(uint16_t set) {
    this->SoftSPISPEED=set;
}

void MySPI::SPIDelay() {
    HAL_Delay_us(SoftSPISPEED);
}

#ifdef HAL_SPI_ERROR_NONE
uint8_t MySPI::SPIWriteRead(uint8_t SendData) {
    uint8_t rxdata;
    HAL_SPI_TransmitReceive(&g_spi1_handler, &txdata, &rxdata, 1, 1000);
    return rxdata; /* 返回收到的数据 */
}
#endif

//void MySPI::SPISpeedSet(uint8_t speed) {
//
//    assert_param(IS_SPI_BAUDRATE_PRESCALER(speed)); /* 判断有效性 */
//    __HAL_SPI_DISABLE(&g_spi_handler);             /* 关闭SPI */
//    g_spi_handler.Instance->CR1 &= 0XFFC7;         /* 位3-5清零，用来设置波特率 */
//    g_spi_handler.Instance->CR1 |= speed << 3;     /* 设置SPI速度 */
//    __HAL_SPI_ENABLE(&g_spi_handler);              /* 使能SPI */
//}


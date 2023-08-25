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

MySPI::MySPI(spi SPISelect, GPIO *CS) {
    this->Mode=HardwareSPI;
    this->CS=CS;
    if (SPISelect==HardwareSpi1)
        this->NowSPIBase=GetBase.SPI_1;
    if (SPISelect==HardwareSpi2)
        this->NowSPIBase=GetBase.SPI_2;
    if (SPISelect==HardwareSpi3)
        this->NowSPIBase=GetBase.SPI_3;
    SPIInit();
}

MySPI::MySPI(GPIO *CS, GPIO *SCK, GPIO *MISO, GPIO *MOSI) {
    this->Mode=SoftwareSPI;
    this->CS=CS;
    this->SCK=SCK;
    this->MOSI=MOSI;
    this->MISO=MISO;
    SPIInit();
}

void MySPI::SPIInit() {


    if (Mode==HardwareSPI){
        HAL_SPI_DeInit(&hspi1);
        if (NowSPIBase==SPI1)
            __HAL_RCC_SPI1_CLK_ENABLE();
        if (NowSPIBase==SPI2)
            __HAL_RCC_SPI2_CLK_ENABLE();
        if (NowSPIBase==SPI3)
            __HAL_RCC_SPI3_CLK_ENABLE();
        g_spi_handler.Instance = NowSPIBase;                              /* SPIx*/
        g_spi_handler.Init.Mode = SPI_MODE_MASTER;                        /* ����SPI����ģʽ������Ϊ��ģʽ */
        g_spi_handler.Init.Direction = SPI_DIRECTION_2LINES;              /* ����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ */
        g_spi_handler.Init.DataSize = SPI_DATASIZE_8BIT;                  /* ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ */
        g_spi_handler.Init.CLKPolarity = SPI_POLARITY_HIGH;               /* ����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ */
        g_spi_handler.Init.CLKPhase = SPI_PHASE_2EDGE;                    /* ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ����� */
        g_spi_handler.Init.NSS = SPI_NSS_SOFT;                            /* NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ���� */
        g_spi_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; /* ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256 */
        g_spi_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;                   /* ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ */
        g_spi_handler.Init.TIMode = SPI_TIMODE_DISABLE;                   /* �ر�TIģʽ */
        g_spi_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;   /* �ر�Ӳ��CRCУ�� */
        g_spi_handler.Init.CRCPolynomial = 7;                             /* CRCֵ����Ķ���ʽ */
        HAL_SPI_Init(&g_spi_handler);                                     /* ��ʼ�� */
        __HAL_SPI_ENABLE(&g_spi_handler); /* ʹ��SPI */
        SPIWriteRead(0XFF);
        GPIOInit();

    }
    if (Mode==SoftwareSPI){

    }

}

void MySPI::GPIOInit() {
    if (NowSPIBase==SPI1){
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);
//        SCK=new GPIO(PA,5,GPIO_MODE_AF_PP,GPIO_SPEED_FREQ_VERY_HIGH,GPIO_AF5_SPI1);
//        MISO=new GPIO(PA,6,GPIO_MODE_AF_PP,GPIO_SPEED_FREQ_VERY_HIGH,GPIO_AF5_SPI1);
//        MOSI=new GPIO(PA,7,GPIO_MODE_AF_PP,GPIO_SPEED_FREQ_VERY_HIGH,GPIO_AF5_SPI1);
        SPI1_SCK_GPIO_CLK_ENABLE();  /* SPI1_SCK��ʱ��ʹ�� */
        SPI1_MISO_GPIO_CLK_ENABLE(); /* SPI1_MISO��ʱ��ʹ�� */
        SPI1_MOSI_GPIO_CLK_ENABLE(); /* SPI1_MOSI��ʱ��ʹ�� */

        /* SCK����ģʽ����(�������) */
        gpio_init_struct.Pin = SPI1_SCK_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_NOPULL;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(SPI1_SCK_GPIO_PORT, &gpio_init_struct);

        /* MISO����ģʽ����(�������) */
        gpio_init_struct.Pin = SPI1_MISO_GPIO_PIN;
        HAL_GPIO_Init(SPI1_MISO_GPIO_PORT, &gpio_init_struct);

        /* MOSI����ģʽ����(�������) */
        gpio_init_struct.Pin = SPI1_MOSI_GPIO_PIN;
        HAL_GPIO_Init(SPI1_MOSI_GPIO_PORT, &gpio_init_struct);
    }

    if (NowSPIBase==SPI2){

//        SCK=new GPIO(PB,3,GPIO_MODE_AF_PP,GPIO_SPEED_FREQ_VERY_HIGH,GPIO_AF5_SPI2);
//        MISO=new GPIO(PB,4,GPIO_MODE_AF_PP,GPIO_SPEED_FREQ_VERY_HIGH,GPIO_AF5_SPI2);
//        MOSI=new GPIO(PB,5,GPIO_MODE_AF_PP,GPIO_SPEED_FREQ_VERY_HIGH,GPIO_AF5_SPI2);
    }

    if (NowSPIBase==SPI3){

    }


}

uint8_t MySPI::SPIWriteRead(uint8_t SendData) {
    uint8_t receiveData=0;
    HAL_SPI_TransmitReceive(&(this->g_spi_handler),&SendData,&receiveData,1,1000);
    return receiveData;
}

void MySPI::SPISpeedSet(uint8_t speed) {

    assert_param(IS_SPI_BAUDRATE_PRESCALER(speed)); /* �ж���Ч�� */
    __HAL_SPI_DISABLE(&g_spi_handler);             /* �ر�SPI */
    g_spi_handler.Instance->CR1 &= 0XFFC7;         /* λ3-5���㣬�������ò����� */
    g_spi_handler.Instance->CR1 |= speed << 3;     /* ����SPI�ٶ� */
    __HAL_SPI_ENABLE(&g_spi_handler);              /* ʹ��SPI */
}


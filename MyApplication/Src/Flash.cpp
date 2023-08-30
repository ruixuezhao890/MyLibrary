/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : Flash.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/26 
*********************************************************************
*********
*/
//

#include "Flash.h"

#ifdef HAL_SPI_ERROR_NONE
Flash::Flash(SPI_HandleTypeDef *SPISelect, GPIO *CS) : MySPI(SPISelect, CS) {
    norflash_init();
//    this->SetSelectMode(CMD);
}
#endif
void Flash::norflash_init(void) {
    uint8_t temp;
    g_norflash_type=norflash_read_id();
    if (g_norflash_type==W25Q256){
        temp = norflash_read_sr(3);
        if ((temp & 0X01) == 0)             /* �������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ */
        {
            norflash_write_enable();        /* дʹ�� */
            temp |= 1 << 1;                 /* ADP=1, �ϵ�4λ��ַģʽ */
            norflash_write_sr(3, temp);     /* дSR3 */
            SetCS(0);
            print(FLASH_Enable4ByteAddr,CMD);
            SetCS(1);
//            NORFLASH_CS(0);
//            spi1_read_write_byte(FLASH_Enable4ByteAddr);    /* ʹ��4�ֽڵ�ַָ�� */
//            NORFLASH_CS(1);
        }
    }

}

uint16_t Flash::norflash_read_id(void) {
    uint16_t deviceId=0;
    SetCS(0);
    print(FLASH_ManufactDeviceID,CMD);
    for (int i = 0; i < 3; ++i) {
        print(0);
    }
    scan(deviceId,CMD);
    SetCS(1);
    return deviceId;

}

uint8_t Flash::norflash_read_sr(uint8_t regno) {
    uint8_t byte = 0, command = 0;

    switch (regno)
    {
        case 1:
            command = FLASH_ReadStatusReg1;  /* ��״̬�Ĵ���1ָ�� */
            break;

        case 2:
            command = FLASH_ReadStatusReg2;  /* ��״̬�Ĵ���2ָ�� */
            break;

        case 3:
            command = FLASH_ReadStatusReg3;  /* ��״̬�Ĵ���3ָ�� */
            break;

        default:
            command = FLASH_ReadStatusReg1;
            break;
    }

    SetCS(0);
    print(command,CMD);      /* ���Ͷ��Ĵ������� */
    scan(byte,CMD);         /* ��ȡһ���ֽ� */
    SetCS(1);

    return byte;
}

void Flash::norflash_write_enable(void) {
    SetCS(0);
    print(FLASH_WriteEnable,CMD);
    SetCS(1);
}

void Flash::norflash_write_sr(uint8_t regno, uint8_t sr) {
    uint8_t command = 0;

    switch (regno)
    {
        case 1:
            command = FLASH_WriteStatusReg1;  /* д״̬�Ĵ���1ָ�� */
            break;

        case 2:
            command = FLASH_WriteStatusReg2;  /* д״̬�Ĵ���2ָ�� */
            break;

        case 3:
            command = FLASH_WriteStatusReg3;  /* д״̬�Ĵ���3ָ�� */
            break;

        default:
            command = FLASH_WriteStatusReg1;
            break;
    }

    SetCS(0);
    print(command,CMD);  /* ���Ͷ��Ĵ������� */
    print(sr,CMD);       /* д��һ���ֽ� */
    SetCS(1);
}

void Flash::norflash_wait_busy(void) {
    while ((norflash_read_sr(1) & 0x01) == 0x01);
}

void Flash::norflash_send_address(uint32_t address) {
    if (g_norflash_type == W25Q256)                     /* ֻ��W25Q256֧��4�ֽڵ�ַģʽ */
    {
        print((uint8_t)((address)>>24),CMD); /* ���� bit31 ~ bit24 ��ַ */
    }
    print((uint8_t)((address)>>16),CMD);     /* ���� bit23 ~ bit16 ��ַ */
    print((uint8_t)((address)>>8),CMD);      /* ���� bit15 ~ bit8  ��ַ */
    print((uint8_t)address,CMD);             /* ���� bit7  ~ bit0  ��ַ */
}

Flash::Flash() {

}

Flash::Flash(GPIO *CS, GPIO *SCK, GPIO *MISO, GPIO *MOSI) : MySPI(CS, SCK, MISO, MOSI){

}

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
        if ((temp & 0X01) == 0)             /* 如果不是4字节地址模式,则进入4字节地址模式 */
        {
            norflash_write_enable();        /* 写使能 */
            temp |= 1 << 1;                 /* ADP=1, 上电4位地址模式 */
            norflash_write_sr(3, temp);     /* 写SR3 */
            SetCS(0);
            print(FLASH_Enable4ByteAddr,CMD);
            SetCS(1);
//            NORFLASH_CS(0);
//            spi1_read_write_byte(FLASH_Enable4ByteAddr);    /* 使能4字节地址指令 */
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
            command = FLASH_ReadStatusReg1;  /* 读状态寄存器1指令 */
            break;

        case 2:
            command = FLASH_ReadStatusReg2;  /* 读状态寄存器2指令 */
            break;

        case 3:
            command = FLASH_ReadStatusReg3;  /* 读状态寄存器3指令 */
            break;

        default:
            command = FLASH_ReadStatusReg1;
            break;
    }

    SetCS(0);
    print(command,CMD);      /* 发送读寄存器命令 */
    scan(byte,CMD);         /* 读取一个字节 */
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
            command = FLASH_WriteStatusReg1;  /* 写状态寄存器1指令 */
            break;

        case 2:
            command = FLASH_WriteStatusReg2;  /* 写状态寄存器2指令 */
            break;

        case 3:
            command = FLASH_WriteStatusReg3;  /* 写状态寄存器3指令 */
            break;

        default:
            command = FLASH_WriteStatusReg1;
            break;
    }

    SetCS(0);
    print(command,CMD);  /* 发送读寄存器命令 */
    print(sr,CMD);       /* 写入一个字节 */
    SetCS(1);
}

void Flash::norflash_wait_busy(void) {
    while ((norflash_read_sr(1) & 0x01) == 0x01);
}

void Flash::norflash_send_address(uint32_t address) {
    if (g_norflash_type == W25Q256)                     /* 只有W25Q256支持4字节地址模式 */
    {
        print((uint8_t)((address)>>24),CMD); /* 发送 bit31 ~ bit24 地址 */
    }
    print((uint8_t)((address)>>16),CMD);     /* 发送 bit23 ~ bit16 地址 */
    print((uint8_t)((address)>>8),CMD);      /* 发送 bit15 ~ bit8  地址 */
    print((uint8_t)address,CMD);             /* 发送 bit7  ~ bit0  地址 */
}

Flash::Flash() {

}

Flash::Flash(GPIO *CS, GPIO *SCK, GPIO *MISO, GPIO *MOSI) : MySPI(CS, SCK, MISO, MOSI){

}

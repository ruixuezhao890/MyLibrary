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
    this->SoftSPIModeSet(SPIMode3);
    g_norflash_type=norflash_read_id();
    if (g_norflash_type==W25Q256){
        temp = norflash_read_sr(3);
        if ((temp & 0X01) == 0)             /* �������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ */
        {
            norflash_write_enable();        /* дʹ�� */
            temp |= 1 << 1;                 /* ADP=1, �ϵ�4λ��ַģʽ */
            norflash_write_sr(3, temp);     /* дSR3 */

            print(FLASH_Enable4ByteAddr,CMD);

//            NORFLASH_CS(0);
//            spi1_read_write_byte(FLASH_Enable4ByteAddr);    /* ʹ��4�ֽڵ�ַָ�� */
//            NORFLASH_CS(1);
        }
    }

}

uint16_t Flash::norflash_read_id(void) {
    uint16_t deviceId=0;
    uint8_t temp;
    SetCS(0);
    print(FLASH_ManufactDeviceID,CMD);
    for (int i = 0; i < 3; ++i) {
        print(0);
    }
    scan(temp,CMD);deviceId=temp<<8;
    scan(temp,CMD);deviceId|=temp;
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
    norflash_init();
}

void Flash::norflash_write_page(char *pbuf, uint32_t addr, uint16_t datalen) {
    uint16_t i;

    norflash_write_enable();                    /* дʹ�� */
    SetCS(0);
    print(FLASH_PageProgram,CMD);    /* ����дҳ���� */
    norflash_send_address(addr);                /* ���͵�ַ */

    for (i = 0; i < datalen; i++)
    {
        print(pbuf[i]);          /* ѭ����ȡ */
    }

    SetCS(1);
    norflash_wait_busy();       /* �ȴ�д����� */
}

void Flash::norflash_write_nocheck(char *pbuf, uint32_t addr, uint16_t datalen) {
    uint16_t pageremain;
    pageremain = 256 - addr % 256;  /* ��ҳʣ����ֽ��� */

    if (datalen <= pageremain)      /* ������256���ֽ� */
    {
        pageremain = datalen;
    }

    while (1)
    {
        /* ��д���ֽڱ�ҳ��ʣ���ַ���ٵ�ʱ��, һ����д��
         * ��д��ֱ�ӱ�ҳ��ʣ���ַ�����ʱ��, ��д������ҳ��ʣ���ַ, Ȼ�����ʣ�೤�Ƚ��в�ͬ����
         */
        norflash_write_page(pbuf, addr, pageremain);

        if (datalen == pageremain)      /* д������� */
        {
            break;
        }
        else                            /* datalen > pageremain */
        {
            pbuf += pageremain;         /* pbufָ���ַƫ��,ǰ���Ѿ�д��pageremain�ֽ� */
            addr += pageremain;         /* д��ַƫ��,ǰ���Ѿ�д��pageremain�ֽ� */
            datalen -= pageremain;      /* д���ܳ��ȼ�ȥ�Ѿ�д���˵��ֽ��� */

            if (datalen > 256)          /* ʣ�����ݻ�����һҳ,����һ��дһҳ */
            {
                pageremain = 256;       /* һ�ο���д��256���ֽ� */
            }
            else                        /* ʣ������С��һҳ,����һ��д�� */
            {
                pageremain = datalen;   /* ����256���ֽ��� */
            }
        }
    }
}

void Flash::norflash_write(char *pbuf, uint32_t addr, uint16_t datalen) {
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    char *norflash_buf;

    norflash_buf = g_norflash_buf;
    secpos = addr / 4096;       /* ������ַ */
    secoff = addr % 4096;       /* �������ڵ�ƫ�� */
    secremain = 4096 - secoff;  /* ����ʣ��ռ��С */

    //printf("ad:%X,nb:%X\r\n", addr, datalen); /* ������ */
    if (datalen <= secremain)
    {
        secremain = datalen;    /* ������4096���ֽ� */
    }

    while (1)
    {
        norflash_read(norflash_buf, secpos * 4096, 4096);   /* ������������������ */

        for (i = 0; i < secremain; i++)     /* У������ */
        {
            if (norflash_buf[secoff + i] != 0XFF)
            {
                break;                      /* ��Ҫ����, ֱ���˳�forѭ�� */
            }
        }

        if (i < secremain)                  /* ��Ҫ���� */
        {
            norflash_erase_sector(secpos);  /* ����������� */

            for (i = 0; i < secremain; i++) /* ���� */
            {
                norflash_buf[i + secoff] = pbuf[i];
            }

            norflash_write_nocheck(norflash_buf, secpos * 4096, 4096);  /* д���������� */
        }
        else    /* д�Ѿ������˵�,ֱ��д������ʣ������. */
        {
            norflash_write_nocheck(pbuf, addr, secremain);              /* ֱ��д���� */
        }

        if (datalen == secremain)
        {
            break;  /* д������� */
        }
        else        /* д��δ���� */
        {
            secpos++;               /* ������ַ��1 */
            secoff = 0;             /* ƫ��λ��Ϊ0 */

            pbuf += secremain;      /* ָ��ƫ�� */
            addr += secremain;      /* д��ַƫ�� */
            datalen -= secremain;   /* �ֽ����ݼ� */

            if (datalen > 4096)
            {
                secremain = 4096;   /* ��һ����������д���� */
            }
            else
            {
                secremain = datalen;/* ��һ����������д���� */
            }
        }
    }
}

void Flash::norflash_erase_chip(void) {
    norflash_write_enable();    /* дʹ�� */
    norflash_wait_busy();       /* �ȴ����� */
    SetCS(0);
    print(FLASH_ChipErase,CMD);  /* ���Ͷ��Ĵ������� */
    SetCS(1);
    norflash_wait_busy();       /* �ȴ�оƬ�������� */
}

void Flash::norflash_erase_sector(uint32_t saddr) {
    saddr *= 4096;
    norflash_write_enable();        /* дʹ�� */
    norflash_wait_busy();           /* �ȴ����� */

   SetCS(0);
    print(FLASH_SectorErase,CMD);    /* ����дҳ���� */
    norflash_send_address(saddr);   /* ���͵�ַ */
    SetCS(1);
    norflash_wait_busy();           /* �ȴ������������ */
}

void Flash::norflash_read(char *pbuf, uint32_t addr, uint16_t datalen) {
    uint16_t i;

    SetCS(0);
    print(FLASH_ReadData,CMD);       /* ���Ͷ�ȡ���� */
    norflash_send_address(addr);                /* ���͵�ַ */
//    scan(pbuf);
    for (i = 0; i < datalen; i++)
    {
        scan(pbuf[i]);   /* ѭ����ȡ */
    }

    SetCS(1);
}

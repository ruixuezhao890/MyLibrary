/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MySPI.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/21 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_MYSPI_H
#define MY_LIBARARY_MYSPI_H
#include "main.h"
#include "MyGPIO.h"
#include "stm32f4xx_hal_spi.h"
# include "spi.h"
#define SPI_SPEED_2         0
#define SPI_SPEED_4         1
#define SPI_SPEED_8         2
#define SPI_SPEED_16        3
#define SPI_SPEED_32        4
#define SPI_SPEED_64        5
#define SPI_SPEED_128       6
#define SPI_SPEED_256       7
#define SPI1_SCK_GPIO_PORT              GPIOB
#define SPI1_SCK_GPIO_PIN               GPIO_PIN_3
#define SPI1_SCK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

#define SPI1_MISO_GPIO_PORT             GPIOB
#define SPI1_MISO_GPIO_PIN              GPIO_PIN_4
#define SPI1_MISO_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

#define SPI1_MOSI_GPIO_PORT             GPIOB
#define SPI1_MOSI_GPIO_PIN              GPIO_PIN_5
#define SPI1_MOSI_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */
//#define SPI2_SCK_GPIO_PORT              GPIOB
//#define SPI2_SCK_GPIO_PIN               GPIO_PIN_3
//#define SPI2_SCK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */
//
//#define SPI2_MISO_GPIO_PORT             GPIOB
//#define SPI2_MISO_GPIO_PIN              GPIO_PIN_4
//#define SPI2_MISO_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */
//
//#define SPI2_MOSI_GPIO_PORT             GPIOB
//#define SPI2_MOSI_GPIO_PIN              GPIO_PIN_5
//#define SPI2_MOSI_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */
enum spi{
    HardwareSpi1=1,
    HardwareSpi2,
    HardwareSpi3,
};
struct ALLSPIBase{
    SPI_TypeDef *SPI_1=SPI1;
    SPI_TypeDef *SPI_2=SPI2;
    SPI_TypeDef *SPI_3=SPI3;
};
typedef enum ModeSelect{
    HardwareSPI=0,
    SoftwareSPI,
}modeSelect;
using namespace stm32f407;
namespace stm32f407 {
    class MySPI {
    public:
        MySPI();
        MySPI(spi SPISelect,GPIO *CS);
        MySPI(GPIO* CS,GPIO* SCK,GPIO * MISO ,GPIO * MOSI);
        void SPIInit();
        void GPIOInit();
        void SPISpeedSet(uint8_t speed);
        uint8_t SPIWriteRead(uint8_t SendData);
        ~MySPI();

    private:
        GPIO *CS;GPIO* SCK;GPIO * MISO ;GPIO * MOSI;
        modeSelect Mode;
        ALLSPIBase GetBase;
        SPI_TypeDef * NowSPIBase;
        SPI_HandleTypeDef g_spi_handler;
        GPIO_InitTypeDef gpio_init_struct;



    };
}


#endif //MY_LIBARARY_MYSPI_H

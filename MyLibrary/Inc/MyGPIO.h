/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MyGPIO.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/11 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_MYGPIO_H
#define MY_LIBARARY_MYGPIO_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "stm32f4xx.h"
#include "main.h"
#include "gpio.h"
/*______________________类的声明在这下面________________:*/
namespace stm32f407{
    enum PORT_INDEX{
        PA=0,
        PB,PC,PD,PE,PF,PG
    };
   typedef struct tagGPIO{
       PORT_INDEX PinGroup;
       GPIO_TypeDef * port;
       uint16_t pin;
       uint32_t mode;
       uint32_t speed;
   }GPIO_PIN;

   typedef struct _periphPORT{
       GPIO_TypeDef * p_port;
   }periph_PORT;


   class GPIO{
   private:
       GPIO_PIN m_gpio;
       void RCCEnable(PORT_INDEX portIndex);
   public:
       ~GPIO(){

       }
       GPIO(){

       }
       GPIO(PORT_INDEX portIndex,uint16_t indexPin,uint32_t p_mode=GPIO_MODE_OUTPUT_PP,uint32_t p_speed=GPIO_SPEED_FREQ_MEDIUM);
       GPIO(PORT_INDEX p_periph,GPIO_TypeDef* p_port,uint16_t p_pins,uint32_t p_mode=GPIO_MODE_OUTPUT_PP,uint32_t p_speed=GPIO_SPEED_FREQ_MEDIUM);

       void Initialize(uint32_t p_mode=GPIO_MODE_OUTPUT_PP);
       void Initialize(PORT_INDEX p_periph,GPIO_TypeDef* p_port,uint16_t p_pins,uint32_t p_mode=GPIO_MODE_OUTPUT_PP,uint32_t p_speed=GPIO_SPEED_FREQ_MEDIUM);
       inline bool IsHigh(){
//           if((m_gpio.port->IDR & m_gpio.pin)!=(uint32_t)GPIO_PIN_RESET )
//           {
//               return true;
//           }
//           else
//           {
//               return false;
//           }
           if (HAL_GPIO_ReadPin(m_gpio.port,m_gpio.pin)==GPIO_PIN_SET){
               return true;
           } else{
               return false;
           }
       }
       inline bool IsLow(){
//           if((m_gpio.port->IDR & m_gpio.pin)!=(uint32_t)GPIO_PIN_RESET )
//           {
//               return false;
//           }
//           else
//           {
//               return true;
//           }
           if (HAL_GPIO_ReadPin(m_gpio.port,m_gpio.pin)==GPIO_PIN_SET){
               return false;
           } else{
               return true;
           }
       }
       inline void High(){
           m_gpio.port->BSRR=m_gpio.pin;
       }
       inline void Low(){
//           HAL_GPIO_WritePin(m_gpio.port,m_gpio.pin,GPIO_PIN_RESET);
           m_gpio.port->BSRR = (uint32_t)m_gpio.pin << 16U;
       }
       inline void invert(){
           if (IsHigh()){
               Low();
           }else{
               High();
           }
       }
       void TimingToggle(uint16_t times=0,uint32_t t=1000,bool Bloop= true){
           uint16_t counter=0;
           while (Bloop){
               High();
               HAL_Delay(t);
               Low();
               HAL_Delay(t);
               counter++;
               if (times!=0){
                   if(counter>=times)
                       break;
               }
           }
       }

   };
}
#ifdef __cplusplus
}
#endif
#endif //MY_LIBARARY_MYGPIO_H

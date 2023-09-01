/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MyKey.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/31 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_MYKEY_H
#define MY_LIBARARY_MYKEY_H
#include "main.h"
#include "MyGPIO.h"
#include "vector"
#include "map"
#define TICKS_INTERVAL    5	//ms
#define DEBOUNCE_TICKS    3	//MAX 7 (0 ~ 7)
#define SHORT_TICKS       (300 /TICKS_INTERVAL)
#define LONG_TICKS        (1000 /TICKS_INTERVAL)
using namespace stm32f407;
using namespace std;
namespace stm32f407 {

    typedef enum {
        PRESS_DOWN = 0,
        PRESS_UP,
        PRESS_REPEAT,
        SINGLE_CLICK,
        DOUBLE_CLICK,
        LONG_PRESS_START,
        LONG_PRESS_HOLD,
        number_of_event,
        NONE_PRESS
    }PressEvent;
    class MyButton;

    class ButtonManage {
    public:

        void button_handler();
        void buttonTick();
        uint8_t TaskSelect(uint8_t i,PressEvent Event);


    protected:
        uint16_t time=0;

    };

    typedef void (*BtnCallBack)(void);
    class MyButton:public ButtonManage{
    private:
        GPIO KeyPin;

    public:
        uint8_t Ready;
        uint16_t ticks;
        uint8_t  repeat : 4;
        uint8_t  event : 4;
        uint8_t  state : 3;
        uint8_t  debounce_cnt : 3;
        uint8_t  active_level : 1;
        uint8_t  button_id;
        bool  button_level : 1;
        // uint8_t  (*hal_button_Level)(uint8_t button_id_);
        BtnCallBack taskFunction[number_of_event];
        MyButton(PORT_INDEX portIndex,uint16_t indexPin,uint8_t ButtonId,uint8_t activeLeve=0);
        void MyButtonTaskRegister(PressEvent event,BtnCallBack taskFunction);
        void MyButtonEnable();
        void MyButtonDisEnable();
        PressEvent GetNowEvent();



        GPIO *GetKeyPin();
        ~MyButton();
    };
}
extern vector<MyButton *> SaveButtonId;
//extern  map<uint8_t,MyButton *> RunButton;
extern ButtonManage buttonManage;
#endif //MY_LIBARARY_MYKEY_H

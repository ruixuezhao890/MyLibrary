/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MyKey.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/31 
*********************************************************************
*********
*/
//

#include "MyKey.h"
#include "MyUsart.h"
MyUsart KeyU(&huart1);
vector<MyButton *> SaveButtonId;
ButtonManage buttonManage;
MyButton::MyButton(PORT_INDEX portIndex,uint16_t indexPin,uint8_t ButtonId,uint8_t activeLeve) {
    this->KeyPin= GPIO (portIndex,indexPin,GPIO_MODE_INPUT);
    this->event=NONE_PRESS;
    this->button_level=KeyPin.IsHigh();

//    if (activeLeve==0){
//        this->active_level= false;//´¥·¢µçÆ½
//    }
//    else{
//        this->active_level= true;
//    }
    this->active_level=activeLeve;
    this->button_id=ButtonId;
    this->state=0;
    for (int i = 0; i < number_of_event; ++i) {
        taskFunction[i]= nullptr;
    }

    //this->Ready=1;
}

MyButton::~MyButton() {
    delete &this->KeyPin;
}

void MyButton::MyButtonTaskRegister(PressEvent event, BtnCallBack taskFunction) {
    this->taskFunction[event]=taskFunction;
}

PressEvent MyButton::GetNowEvent() {
    return (PressEvent)this->event;
}

GPIO *MyButton::GetKeyPin() {
    return &this->KeyPin;
}

void MyButton::MyButtonEnable() {
    SaveButtonId.push_back(this);
}

void MyButton::MyButtonDisEnable() {
//    for (int i = 0; i < SaveButtonId.size();) {
//        if(SaveButtonId[i]->button_id==this->button_id){
//            SaveButtonId.erase(&SaveButtonId[i]);
//        }
//    }
    for (auto it=SaveButtonId.begin(); it<SaveButtonId.end() ; ) {
        if((*it)->button_id==this->button_id){
            SaveButtonId.erase(it);
        }else{
            it++;
        }
    }
    //this->Ready=0;
    //this->RunButton.erase(this->button_id);
}

void ButtonManage::button_handler() {
    for (int i = 0; i < SaveButtonId.size(); ++i) {

        bool GPIOLevel=SaveButtonId[i]->GetKeyPin()->IsHigh();
        if (SaveButtonId[i]->state>0)SaveButtonId[i]->ticks++;
//        KeyU<<"now"<<GPIOLevel<<endl;
//        KeyU<<"buton"<<SaveButtonId[i]->button_level<<endl;
        if (GPIOLevel!=SaveButtonId[i]->button_level){
            //KeyU<<"OK"<<endl;

            if (++(SaveButtonId[i]->debounce_cnt)>=DEBOUNCE_TICKS){
                SaveButtonId[i]->button_level=GPIOLevel;
                SaveButtonId[i]->debounce_cnt=0;
            }
        }else{
            SaveButtonId[i]->debounce_cnt=0;
        }
        //KeyU<<"i:"<<i<<"state "<<SaveButtonId[i]->state<<endl;
        switch (SaveButtonId[i]->state) {
            case 0:
                if (SaveButtonId[i]->button_level==SaveButtonId[i]->active_level){
                   // KeyU<<0<<endl;
                    SaveButtonId[i]->event=PRESS_DOWN;
                    TaskSelect(i,PRESS_DOWN);
                    SaveButtonId[i]->ticks=0;
                    SaveButtonId[i]->repeat=1;
                    SaveButtonId[i]->state=1;
                }else{
                    SaveButtonId[i]->event=NONE_PRESS;
                }
                break;
            case 1:
                if (SaveButtonId[i]->button_level!=SaveButtonId[i]->active_level){
                    SaveButtonId[i]->event=PRESS_UP;
                    TaskSelect(i,PRESS_UP);

                    SaveButtonId[i]->ticks=0;
                   // KeyU<<"1"<<endl;
                    SaveButtonId[i]->state=2;

                }
                else if (SaveButtonId[i]->ticks>LONG_TICKS){
                    SaveButtonId[i]->event=LONG_PRESS_START;
                    TaskSelect(i,LONG_PRESS_START);
                    SaveButtonId[i]->state=5;
                    KeyU<<"5"<<endl;
                }

                break;
            case 2:
//                KeyU<<2<<endl;
                if (SaveButtonId[i]->button_level==SaveButtonId[i]->active_level){
                    SaveButtonId[i]->event=PRESS_DOWN;
                    //TaskSelect(i,PRESS_DOWN);
                    SaveButtonId[i]->repeat++;
                    TaskSelect(i,PRESS_REPEAT);
                    SaveButtonId[i]->ticks=0;
                    SaveButtonId[i]->state=3;
                }
                else if (SaveButtonId[i]->ticks>SHORT_TICKS){
                    if (SaveButtonId[i]->repeat==1){
                        SaveButtonId[i]->event=SINGLE_CLICK;
                        TaskSelect(i,SINGLE_CLICK);
                    }else if (SaveButtonId[i]->repeat==2){
                        SaveButtonId[i]->event=DOUBLE_CLICK;
                        //SaveButtonId[i]->taskFunction[DOUBLE_CLICK];
                        TaskSelect(i,DOUBLE_CLICK);
                    }
                    SaveButtonId[i]->state=0;
                }
                break;
            case 3:
               // KeyU<<3<<endl;
                if (SaveButtonId[i]->button_level!=SaveButtonId[i]->active_level){
                    SaveButtonId[i]->event=PRESS_UP;
                    TaskSelect(i,PRESS_UP);
                    //SaveButtonId[i]->taskFunction[PRESS_UP];
                    if (SaveButtonId[i]->ticks<SHORT_TICKS){
                        SaveButtonId[i]->ticks=0;
                        SaveButtonId[i]->state=2;
                    }else{
                        SaveButtonId[i]->state=0;
                    }

                }
                else if (SaveButtonId[i]->ticks>SHORT_TICKS){
                    SaveButtonId[i]->state=0;
                }
                break;
            case 5:
              //  KeyU<<5<<endl;
                if (SaveButtonId[i]->button_level==SaveButtonId[i]->active_level){
                    SaveButtonId[i]->event=LONG_PRESS_HOLD;
                   // SaveButtonId[i]->taskFunction[LONG_PRESS_HOLD];
                 TaskSelect(i,LONG_PRESS_HOLD);
                } else{
                    SaveButtonId[i]->event=PRESS_UP;
                    //SaveButtonId[i]->taskFunction[PRESS_UP];
                    TaskSelect(i,PRESS_UP);
                    SaveButtonId[i]->state=0;
                }
                break;
            default:
                SaveButtonId[i]->state=0;
                break;
        }
    }

}

uint8_t ButtonManage::TaskSelect(uint8_t i, PressEvent Event) {
    if (SaveButtonId[i]->taskFunction[Event]!= nullptr){
        SaveButtonId[i]->taskFunction[Event]();
        return 1;
    }
    return 0;
}

void ButtonManage::buttonTick() {
    time++;
    if (time>=10){
        time=0;
        this->button_handler();
    }
}

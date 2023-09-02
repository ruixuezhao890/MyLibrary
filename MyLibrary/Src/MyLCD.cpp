/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MyLCD.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/18 
*********************************************************************
*********
*/
//

#include "MyLCD.h"
//#include "lcd.h"
//#include "touch.h"
MyLCD::MyLCD(uint8_t mode) {
    if (mode){

        lv_init();
        lv_port_disp_init();
        lv_port_indev_init();

    }else{
        lv_init();
        lv_port_disp_init();
    }

}

MyLCD::~MyLCD() {

}

void MyLCD::MyLCDGreatButton(uint16_t x, uint16_t y, String text, CallbackFun callbackFun, uint16_t wight, uint16_t high) {
    lv_obj_t * btn= lv_btn_create(lv_scr_act());
    if (btn!=NULL){
        lv_obj_set_size(btn,wight,high);
        lv_obj_set_pos(btn,x,y);
        lv_obj_add_event_cb(btn,callbackFun,LV_EVENT_CLICKED,NULL);
        lv_obj_t * label= lv_label_create(btn);
        if (label!=NULL){
            lv_label_set_text(label,text.c_str());
            lv_obj_center((label));
        }

    }
}

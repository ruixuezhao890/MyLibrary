/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : Startup.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/11 
*********************************************************************
*********
*/
#include "ConnectCplusplus.h"
#include "ALLfile.h"
#include "touch.h"
using namespace stm32f407;
using namespace lvgl;
using namespace lvgl::core;
using namespace lvgl::misc;
using namespace lvgl::widgets;
 char g_text_buf[] = {"STM32 SPI TEST"};
#define TEXT_SIZE   sizeof(g_text_buf)
MyUsart myUart(&huart1);
static void btn_event_callback(lv_event_t* event){
   // lv_obj_t *target = lv_event_get_target(event);
//    myUart<<"screen button pressDown"<<endl;
//    myUart<<"test OK"<<endl;
    if(event->code == LV_EVENT_CLICKED) {
        myUart<<("Clicked")<<endl;
    }
    else if(event->code == LV_EVENT_VALUE_CHANGED) {
        myUart<<("Toggled")<<endl;
    }
}
    void PressDownHandle(){
        myUart<<"button pressDown"<<endl;
        myUart<<"test OK"<<endl;
    }
    void Pressrepet(){


            myUart<<"repet"<<endl;

    }
    void Longpress(){
            myUart<<"Longpress"<<endl;
    }
void MyMain(){
    lvgl::init();
//    lv_init();
//    lv_port_disp_init();
//    lv_port_indev_init();
    Object root = scr_act();
    static auto button1 = Button(root);
    static auto label1 = Label(button1);
//
    auto event_handler = [](Event & e) {
        auto code = e.get_code();

        if(code == LV_EVENT_CLICKED) {
            myUart<<("Clicked")<<endl;
        }
        else if(code == LV_EVENT_VALUE_CHANGED) {
            myUart<<("Toggled")<<endl;
        }
    };
    button1.add_event_cb(event_handler, LV_EVENT_ALL);
    button1.align(LV_ALIGN_CENTER, 0, -40);

    label1.set_text("Button");
    label1.center();

    static auto button2 = Button(root);
    static auto label2 = Label(button2);
    button2.add_event_cb(event_handler, LV_EVENT_ALL);
    button2.align(LV_ALIGN_CENTER, 0, 40);
    button2.add_flag(LV_OBJ_FLAG_CHECKABLE);
    button2.set_height(LV_SIZE_CONTENT);

    label2.set_text("Toggle");
    label2.center();

//    lv_obj_t * btn_speed_down = lv_btn_create(lv_scr_act());
///* 设置按钮大小 */
//    lv_obj_set_size(btn_speed_down, 100, 200);
///* 设置按钮位置 */
//    lv_obj_align(btn_speed_down, LV_ALIGN_CENTER, 0, 0);
///* 设置按钮事件 */
//    lv_obj_add_event_cb(btn_speed_down, btn_event_callback, LV_EVENT_CLICKED, NULL);
//    lv_obj_t* label = lv_label_create(btn_speed_down); /* 创建减速按钮标签 */
//    //lv_obj_set_style_text_font(label, font, LV_PART_MAIN); /* 设置字体 */
//    lv_label_set_text(label, "Speed -"); /* 设置标签文本 */
//    lv_obj_set_align(label,LV_ALIGN_CENTER);
    for (;;){


    }


}
void Interrupt_management(DMA_HandleTypeDef * hdma_usartx_rx){
    myUart.ReceiveDataCallBack(hdma_usartx_rx);
}
void KeyTimeBenchmark(){
        buttonManage.buttonTick();
}
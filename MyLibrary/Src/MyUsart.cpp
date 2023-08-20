/**
*********************************************************************
*********
* @project_name :templateDevelopment
* @file : myUsart.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/4/27 
*********************************************************************
*********
*/
//

#include <cstdarg>
#include <cstring>
#include <cstdio>
#include "MyUsart.h"

MyUsart::MyUsart()
{

}
MyUsart::MyUsart(UART_HandleTypeDef *huart)
{
    this->huart=huart;
}
MyUsart::MyUsart(uint16_t Enable,UART_HandleTypeDef *huart)
{
    this->huart=huart;
    this->ReceiveInit();
}
/***************************************************************
  *  @brief     �������� ���ܿ����жϽ��ղ�����������
  *  @param     ����   ��
  *  @note      ��ע �� �ڽ���ǰ�����ȵ��øú���
  *  @Sample usage:     ������ʹ�÷��� ��ֱ�ӵ���
   * @author     wangzibo
 **************************************************************/

void MyUsart::ReceiveInit()
{
    buff.write_index=buff.read_index=buff.data_size=0;
    for (int i = 0; i < RELENTH; ++i)
        buff.buf[i]=0;
    memset(this->re_Buff,0,RELENTH);
    __HAL_UART_ENABLE_IT(this->huart, UART_IT_IDLE);//ʹ��idle�ж�
    HAL_UART_Receive_DMA(this->huart, re_Buff, RELENTH);
}
//����"stm32f4xx_it.c�ļ���USARTx_IRQHandler(void)��
void MyUsart::ReceiveDataCallBack(DMA_HandleTypeDef * hdma_usartx_rx)
{
    if (hdma_usartx_rx==this->huart->hdmarx) {
        uint32_t tmp_flag = 0;
        uint32_t temp;
        tmp_flag = __HAL_UART_GET_FLAG(this->huart, UART_FLAG_IDLE); //��ȡIDLE��־λ
        if ((tmp_flag != RESET))//idle��־����λ
        {
            __HAL_UART_CLEAR_IDLEFLAG(this->huart);//�����־λ
            temp = huart->Instance->SR;  //���״̬�Ĵ���SR,��ȡSR�Ĵ�������ʵ�����SR�Ĵ����Ĺ���
            temp = huart->Instance->DR; //��ȡ���ݼĴ����е�����
            HAL_UART_DMAStop(this->huart); //
            temp = __HAL_DMA_GET_COUNTER(hdma_usartx_rx); // ��ȡDMA��δ��������ݸ�����NDTR�Ĵ�������������
            re_len = RELENTH - temp; //�ܼ�����ȥδ��������ݸ������õ��Ѿ����յ����ݸ���
            recv_end_flag = 1;    // ������ɱ�־λ��1
            buff.data_size=re_len;
            for (int i = 0; i < re_len; i++) {
                buff.buf[buff.write_index++]=this->re_Buff[i];
            }
        }
    }
}
void MyUsart::SendReceive()
{
    if(recv_end_flag ==1)
    {
        HAL_UART_DMAStop(huart);

        //��ӡ��������
        HAL_UART_Transmit(huart, re_Buff, re_len, 50);

        for(uint8_t i=0; i < re_len; i++)
        {
            re_Buff[i]=0;//����ջ���
        }
        re_len=0;//�������
        recv_end_flag=0;//������ս�����־λ
    }

    HAL_UART_Receive_DMA(this->huart, re_Buff, RELENTH);
}
MyUsart::~MyUsart() {

}

void MyUsart::SendCmdOut(char * fmt, ...) {
    va_list ap;
    uint16_t len;
    va_start(ap, fmt);
    vsprintf((char *)tx_Buff, fmt, ap);
    va_end(ap);

    len = strlen((const char *)tx_Buff);
    HAL_UART_Transmit(this->huart, tx_Buff, len, HAL_MAX_DELAY);
}

uint8_t *MyUsart::Re_GetData() {
    if (this->recv_end_flag==1)
    {

        return this->re_Buff;
    }
    else
    {
        return NULL;
    }

}

uint8_t *MyUsart::ReceiveAgain() {
    for (int i = 0; i < RELENTH; ++i)
        buff.buf[i]=0;
    buff.write_index=buff.read_index=buff.data_size=0;
    memcpy(this->temp,this->re_Buff,RELENTH);
    memset(this->re_Buff,0,RELENTH);
    this->recv_end_flag=0;
    this->re_len=0;
    __HAL_UART_ENABLE_IT(this->huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(this->huart, re_Buff, RELENTH);
    return temp;

}

uint8_t MyUsart::GetRECV() {
    return this->recv_end_flag;
}

uint8_t MyUsart::GetRecvLenth() {
    return this->re_len;
}

void MyUsart::Myprintf(char *fmt, ...) {
    va_list ap;
    uint16_t len;
    va_start(ap, fmt);
    vsprintf((char *)tx_Buff, fmt, ap);
    va_end(ap);
    len = strlen((const char *)tx_Buff);
    HAL_UART_Transmit(this->huart, tx_Buff, len, HAL_MAX_DELAY);
}

void MyUsart::SendString(uint8_t *stringBuf) {
    this->Myprintf("%s\n",stringBuf);

}

void MyUsart::SendString(const char *stringBuf) {
    this->Myprintf("%s\n",stringBuf);
}

void MyUsart::SendOneInt(uint16_t IntBuf) {
    this->Myprintf("%d\n",IntBuf);
}

void MyUsart::SendInt(uint16_t *IntBuf, uint16_t times) {
    for (int i = 0; i < times; ++i) {
        this->Myprintf("%d\n",IntBuf[i]);
    }
}

void MyUsart::SendOneFloat(float FloatBuf) {
    this->Myprintf("%f\n",FloatBuf);
}

void MyUsart::SendFloat(float *FloatBuf, uint16_t times) {
    for (int i = 0; i < times; ++i) {
        this->Myprintf("%f\n",FloatBuf[i]);
    }
}

size_t MyUsart::write(uint8_t data) {
    size_t n= sizeof(data);
    HAL_UART_Transmit(this->huart,&data,n,HAL_MAX_DELAY);
    return n;
}

size_t MyUsart::write(const uint8_t *buffer, size_t size) {
    HAL_UART_Transmit(this->huart,buffer,size,HAL_MAX_DELAY);
    return size;
}

void MyUsart::begin(uint32_t baudRate) {
    this->huart->Init.BaudRate=baudRate;
    HAL_UART_Init(this->huart);
}


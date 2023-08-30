/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : inputStream.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/14 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_INPUTSTREAM_H
#define MY_LIBARARY_INPUTSTREAM_H
#include "main.h"
#include "WString.h"
#define CMD 0
#define BUFFMAX 256
typedef struct
{
     uint8_t buf[BUFFMAX];      //���ջ�����
     uint8_t write_index;   //дָ��
     uint8_t read_index;    //��ָ��
     uint16_t data_size;    //���������յ������ݳ���
    /* data */
}__rec_buf;
class inputStream{
public:
    uint8_t CMDFlag=0;//���ǵ���ȡָ������
    __rec_buf buff;
    virtual int available();
    virtual int read();
    virtual int peek();

    /***���Ͷ�ȡ**********************/
    long scanNumber();
    double scanFloat();

    int getc();

    int scan(char *); //ɨ���ַ���
    int scan(String &);
    int scan(char &);
    int scan(unsigned char &);      //ɨ���ַ�
    int scan(int &);                //ɨ������
    int scan(uint16_t &);                //ɨ������
    int scan(uint32_t &);                //ɨ������
    int scan(unsigned int &);       //ɨ������
    int scan(long long &);          //ɨ������
    int scan(unsigned long long &); //ɨ������
    int scan(double &);             //ɨ�踡����
    int scan(float &);              //ɨ�踡����


    int scan(int &,int base);                //ɨ����������ָ��
    int scan(uint8_t &,int base);                //ɨ����������ָ��
    int scan(uint16_t &,int base);           //ɨ����������ָ��
    int scan(uint32_t &,int base);           //ɨ����������ָ��
    int scan(unsigned int &,int base);       //ɨ����������ָ��
    int scan(long long &,int base);          //ɨ����������ָ��
    int scan(unsigned long long &,int base); //ɨ����������ָ��

    template <typename T>
    inputStream &operator>>(T &out)
    {
        scan(out);
        return *this;
    }
};
#endif //MY_LIBARARY_INPUTSTREAM_H

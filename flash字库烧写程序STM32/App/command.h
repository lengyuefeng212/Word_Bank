#ifndef __COMMAND_H
#define __COMMAND_H

#define  FONTSIZE  0x10914FL  //ÿ������Ĵ�С2017-08-13
#define  ASCII     0x1078D0L   //ASCII������ÿ�������е�ƫ����

#define  SONGTI    0x00L   //�������忪ʼ��ַ
#define  HEITI     FONTSIZE+1   //�������忪ʼ��ַ 0x109150
#define  HUAKAI    HEITI + FONTSIZE + 1   //�����п����忪ʼ��ַ
#define  FANGTI    HUAKAI + FONTSIZE + 1   //�������己�����忪ʼ��ַ
#define  KAITI     FANGTI + FONTSIZE + 1   //���ֿ�ʼ��ַ
#define  HUACAI    KAITI + FONTSIZE + 1   //���Ĳ��ƿ�ʼ��ַ
#define  LISHU     HUACAI + FONTSIZE + 1   //�������忪ʼ��ַ

void MoniorCM(void);

#endif
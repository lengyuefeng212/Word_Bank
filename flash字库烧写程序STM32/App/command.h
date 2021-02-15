#ifndef __COMMAND_H
#define __COMMAND_H

#define  FONTSIZE  0x10914FL  //每种字体的大小2017-08-13
#define  ASCII     0x1078D0L   //ASCII字体在每种字体中的偏移量

#define  SONGTI    0x00L   //宋体字体开始地址
#define  HEITI     FONTSIZE+1   //宋体字体开始地址 0x109150
#define  HUAKAI    HEITI + FONTSIZE + 1   //华文行楷字体开始地址
#define  FANGTI    HUAKAI + FONTSIZE + 1   //方正楷体繁体字体开始地址
#define  KAITI     FANGTI + FONTSIZE + 1   //楷字开始地址
#define  HUACAI    KAITI + FONTSIZE + 1   //华文彩云开始地址
#define  LISHU     HUACAI + FONTSIZE + 1   //隶书字体开始地址

void MoniorCM(void);

#endif
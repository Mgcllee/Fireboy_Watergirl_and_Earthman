#pragma once
#include "stdafx.h"
#include "ImageMgr.h"

extern RECTANGLE Rt;

extern BOOL playsound;
extern int stage;
extern BOOL clear;
extern int average;
extern BOOL time_over;
extern BOOL red_door_open, blue_door_open;
extern int count;
extern int red_total;
extern int blue_total;
extern BOOL stair;

extern OBJECT Jewelry[20];	// 0~9±îÁö ÆÄ¶ûº¸¼® 10~19±îÁö »¡°­º¸¼®
extern OBJECT Trap[90];		// 0~19±îÁö ÆÄ¶û °¡¿îµ¥ ¹° 20~29±îÁö ÆÄ¶û ¿Þ.¿À 30~49±îÁö »¡°­ °¡¿îµ¥ ¹° 50~59±îÁö »¡°­¹° ¿Þ.¿À 60~79 ÃÊ·Ï °¡¿îµ¥ 80~89 ÃÊ·Ï¿Þ.¿À
extern OBJECT Die;
extern OBJECT blue_door;
extern OBJECT red_door;
extern OBJECT button[5];
extern OBJECT block[5];
extern FootHold Ft[20]; 
extern RECTANGLE Rt;

void Stage_1(int order);
void Stage_2(int order);
void Stage_3(int order);

#pragma once
#include "MapMgr.h"

RECTANGLE Rt;

BOOL playsound;
int stage;
BOOL clear;
int average;
BOOL time_over;
BOOL red_door_open, blue_door_open;
int count;
int red_total;
int blue_total;
BOOL stair;

OBJECT Jewelry[20];	// 0~9±îÁö ÆÄ¶ûº¸¼® 10~19±îÁö »¡°­º¸¼®
OBJECT Trap[90];	// 0~19±îÁö ÆÄ¶û °¡¿îµ¥ ¹° 20~29±îÁö ÆÄ¶û ¿Þ.¿À 30~49±îÁö »¡°­ °¡¿îµ¥ ¹° 50~59±îÁö »¡°­¹° ¿Þ.¿À 60~79 ÃÊ·Ï °¡¿îµ¥ 80~89 ÃÊ·Ï¿Þ.¿À
OBJECT Die;
OBJECT blue_door;
OBJECT red_door;
OBJECT button[5];
OBJECT block[5];
FootHold Ft[20];

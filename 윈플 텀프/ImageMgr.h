#pragma once
#include "stdafx.h"

extern CImage robby, buttonimg, stage1, Jewelry_blue, Jewelry_red, red_water_center, red_water_left, red_water_right, blue_water_center, blue_water_left, blue_water_right;
extern CImage green_water_center, green_water_left, green_water_right, die, retryimg, endimg, backimg, door_red, door_blue, button_img, clear_img, red_stair, blue_stair, rect;
extern CImage zero, one, two, three, four, five, six, seven, eight, nine, clock, timeout;
extern CImage block_w, block_h, foot_block;

void LoadImages();
void DrawMap(HDC* memdc, short stage);
void DrawTimer(HDC* memdc, short time);
void DrawPlayer(HDC* memdc, short Frame, PLAYER* pl);

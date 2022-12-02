#pragma once
#include "stdafx.h"
#include"Stage.h"


class ImageMgr {
public:
	CImage loading,robby, buttonimg, stage1, Jewelry_blue, Jewelry_red, red_water_center, red_water_left, red_water_right, blue_water_center, blue_water_left, blue_water_right;
	CImage green_water_center, green_water_left, green_water_right, die, retryimg, endimg, backimg, door_red, door_blue, button_img, clear_img, red_stair, blue_stair, rect;
	CImage zero, one, two, three, four, five, six, seven, eight, nine, clock, timeout;
	CImage block_w, block_h, foot_block;

	CImage lobby;
	CImage fraction;

	CImage fireStopImage;
	
	CImage waterStopImage;	

	CImage earthStopImage;

	CImage leftArrow, rightArrow;
	CImage me, player1, player2;
	CImage selectBtn;

	CImage btnend;


public:
	void LoadImages();
	void DrawMap(HDC* memdc, short stageNum, Stage& stage);
	void DrawTimer(HDC* memdc, short time);
	void DrawPlayers(HDC* memdc, Stage& stage);
};

#pragma once
#include<wtypes.h>
#include"Player.h"
class OBJECT
{
	bool On = FALSE;			// 사용 여부

public:
	int x{}, y{};				// 윈도우 기준 좌상단 (x, y) 위치좌표
	int image_x{}, image_y{};	// 출력할 이미지 프레임 기준점 (스프라이트 이미지)
	int wid{}, hei{};			// 가로길이, 세로길이
	int MaxWid{};				// 전체 스프라이트 가로길이
	int imageMoveWid{};			// 스프라이트 이미지 이동

	OBJECT() {};
	OBJECT(int pos_x, int pos_y, int WID, int HEI, int MAXWID, int IMAGEMOVEWID, bool ON)
		: x(pos_x), y(pos_y), wid(WID), hei(HEI), MaxWid(MAXWID), imageMoveWid(IMAGEMOVEWID), On(ON) {}

	// 충돌 확인 함수, Player(fire boy or water girl) 입력을 받아 this Object로 충돌 비교
	bool Collision(PLAYER& pl) {
		if (
			(abs((x - wid / 2) - (pl.x - pl.wid / 2)) <= (wid + pl.wid) / 2)
			&&
			(abs((y - hei / 2) - (pl.y - pl.hei / 2)) <= (hei + pl.hei) / 2)
			) {
			return true;
		}
		else return false;
	}

	bool Ft_Collision(PLAYER& pl) {
		if (
			(abs((x - wid / 2) - (pl.x - pl.wid / 2)) < (wid + pl.wid) / 2)
			&&
			(abs((y - hei / 2) - (pl.y - pl.hei / 2)) < (hei + pl.hei) / 2)
			) {
			return true;
		}
		else return false;
	}

	void SetVisible(bool in) {
		On = in;
	}

	bool GetVisible() {
		return On;
	}

	bool ChangeFrame(int direction, bool replay) {
		if (MaxWid == image_x + imageMoveWid) {
			if (replay)
				image_x = 0;
			return true;
		}
		else {
			image_x += (imageMoveWid * direction);
			return false;
		}
	}
};


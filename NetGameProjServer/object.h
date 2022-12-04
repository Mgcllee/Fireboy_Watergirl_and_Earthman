#pragma once
//#include"ThreadInfo.h"
class ThreadInfo;
class OBJECT {
	bool On = false;			// 사용 여부

public:
	int x{}, y{};				// 윈도우 기준 좌상단 (x, y) 위치좌표
	int image_x{}, image_y{};	// 출력할 이미지 프레임 기준점 (스프라이트 이미지)
	int wid{}, hei{};			// 가로길이, 세로길이
	int MaxWid{};				// 전체 스프라이트 가로길이
	int imageMoveWid{};			// 스프라이트 이미지 이동

	OBJECT() {};
	OBJECT(int pos_x, int pos_y, int WID, int HEI, int MAXWID, int IMAGEMOVEWID, bool ON)
		: x(pos_x), y(pos_y), wid(WID), hei(HEI), MaxWid(MAXWID), imageMoveWid(IMAGEMOVEWID), On(ON) {}

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
	bool Collision(ThreadInfo& pl);
	bool FT_Collide_Fall(ThreadInfo& pl);
	bool Ft_Collision(ThreadInfo& pl);
	bool OBJECT_Collide(ThreadInfo& pl);
};

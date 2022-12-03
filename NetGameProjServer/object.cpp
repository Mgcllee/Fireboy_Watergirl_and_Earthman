#include"object.h"
#include"ThreadInfo.h"
// 충돌 확인 함수, Player(fire boy or water girl) 입력을 받아 this Object로 충돌 비교
bool OBJECT::Collision(ThreadInfo& pl) {
	/*if ((abs(x - pl.x) <= (wid + pl.wid) / 2) && (abs(y - pl.y) <= (hei + pl.hei) / 2)) {
		return true;
	}*/
	if (pl.x > x - wid / 2  && pl.x - 60 < x + wid / 2) { // 사이 안에 있고
		if (pl.y - 60 < y && pl.y > y) // 머리가 밑에 닿았을때
			return true;
	}
	return false;
}

bool OBJECT::Ft_Collision(ThreadInfo& pl) {
	/*if ((abs(x - pl.x) <= (wid + pl.wid) / 2) && ((y - pl.y) < ((hei + pl.hei) / 3.0f))) {
		return true;
	}*/
	if (pl.x > x - wid / 2 && pl.x - 60 < x + wid / 2) {// 사이 안에 있고
		if (pl.y - 60 < y - hei && pl.y > y - hei) // 다리가 위에 닿았을때
			return true;
	}
	return false;
}

bool OBJECT::FT_Collide_Fall(ThreadInfo& pl) {
	if (pl.x < x - wid / 2 || pl.x - 60 > x + wid / 2) {
		return true;
	}
	return false;
}


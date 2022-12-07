#include"object.h"
#include"ThreadInfo.h"
#include<iostream>
// 충돌 확인 함수, Player(fire boy or water girl) 입력을 받아 this Object로 충돌 비교
bool OBJECT::Collision(ThreadInfo& pl) {
	if (pl.x + 5 > x - wid / 2 && pl.x - 55 < x + wid / 2) { // 사이 안에 있고
		if (pl.y - 60 < y && pl.y > y) {// 머리가 밑에 닿았을때
			return true;
		}
	}
	return false;
}

bool OBJECT::Ft_Collision(ThreadInfo& pl) {
	if (pl.x - 5 > x - wid / 2 && pl.x - 55 < x + wid / 2) {// 사이 안에 있고
		if (pl.y - 60 < y - hei && pl.y > y - hei) // 다리가 위에 닿았을때
		{
			return true;
		}
	}
	return false;
}

bool OBJECT::FT_Collide_Fall(ThreadInfo& pl) {
	if (pl.x - 5 < x - wid / 2 || pl.x - 55 > x + wid / 2) {
		return true;
	}
	return false;
}

bool OBJECT::OBJECT_Collide(ThreadInfo& pl)
{
	if (pl.x - 55 > x - wid / 2 && pl.x - 55 < x + wid / 2) { // 사이 안에 있고
		if (pl.y < y && pl.y > y - hei) {
			return true;
		}
		if (pl.y - 60 < y && pl.y - 60 > y - hei) {
			return true;
		}
	}
	if (pl.x-5 > x - wid / 2 && pl.x-5 < x + wid / 2) {
		if (pl.y < y && pl.y > y - hei) {
			return true;
		}
		if (pl.y - 60 < y && pl.y - 60 > y - hei) {
			return true;
		}
	}
	if (x - wid / 2 > pl.x - 55 && x - wid / 2 < pl.x-5) {
		if (pl.y - 60 < y && pl.y > y) {
			return true;
		}
		if (pl.y - 60 < y - hei && pl.y > y - hei) {
			return true;
		}
	}
	if (x + wid / 2 > pl.x - 55 && x + wid / 2 < pl.x - 5) {
		if (pl.y - 60 < y && pl.y > y) {
			return true;
		}
		if (pl.y - 60 < y - hei && pl.y > y - hei) {
			return true;
		}
	}

	return false;
}
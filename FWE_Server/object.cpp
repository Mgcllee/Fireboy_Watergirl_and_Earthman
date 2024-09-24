#include"object.h"
#include"Client.h"
#include<iostream>

bool OBJECT::Collision(Client& pl) {
	if (pl.x + 5 > x - wid / 2 && pl.x - 55 < x + wid / 2) { 
		if (pl.y - 60 < y && pl.y > y) {
			return true;
		}
	}
	return false;
}

bool OBJECT::Ft_Collision(Client& pl) {
	if (pl.x - 5 > x - wid / 2 && pl.x - 55 < x + wid / 2) {
		if (pl.y - 60 < y - hei && pl.y > y - hei) 
		{
			return true;
		}
	}
	return false;
}

bool OBJECT::FT_Collide_Fall(Client& pl) {
	if (pl.x - 5 < x - wid / 2 || pl.x - 55 > x + wid / 2) {
		return true;
	}
	return false;
}

bool OBJECT::OBJECT_Collide(Client& pl)
{
	if (pl.x - 55 > x - wid / 2 && pl.x - 55 < x + wid / 2) { 
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
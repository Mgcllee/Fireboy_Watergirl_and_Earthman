#pragma once

class Client;
class OBJECT {
	bool On = false;			

public:
	int x{}, y{};				
	int image_x{}, image_y{};	
	int wid{}, hei{};			
	int MaxWid{};				
	int imageMoveWid{};			

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
	bool Collision(Client& pl);
	bool FT_Collide_Fall(Client& pl);
	bool Ft_Collision(Client& pl);
	bool OBJECT_Collide(Client& pl);
};

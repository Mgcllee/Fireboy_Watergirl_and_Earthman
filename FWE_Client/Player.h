#pragma once
#include <atlimage.h>
#include<wtypes.h>
enum DIRECTION {
	IDLE,
	JUMP,
	LEFT,
	RIGHT
};

class PLAYER {
public:
	int id;						
	short role;					
	int x, y;					
	int hei = 100, wid = 60;	

	int direction = DIRECTION::IDLE;			
	float wid_v{};
	float wid_a{};

	int ground = 730;		
	float g = 3.f;			
	float v = 0;			
	short Frame = 0;		

	CImage Anim[5];
	int C_img_Frame{};
	int C_img_x{}, C_img_y{};
	int C_img_X_Size_01{}, C_img_Y_Size_01{};
	int C_img_X_Size_02{}, C_img_Y_Size_02{};

	bool on = TRUE;
	bool is_Jumping = FALSE;
	bool is_Push = FALSE;
	bool Down = FALSE;
	bool isIntoDoor = FALSE;
	int score = 0;

	PLAYER() : id(-1), role('f') {}
	~PLAYER() {

	}
};

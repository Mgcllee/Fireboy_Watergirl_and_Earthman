#include"Main_head.h"

BOOL keybuffer[256]; //다중키 입력을 위한 버퍼를 담는 곳

void Loop()
{
	//다중키 입력을 위해서 전부 if로 작성했으며 [ ] 안에 원하는 키와 행동을 넣으면 작동한다.
	if (keybuffer[VK_LEFT])
	{
		fire.wid_a += 1;
		if (fire.wid_a > 5) {
			fire.wid_a = 10;
		}
		fire.dic = -1;
		fire.is_Move = TRUE;
	}
	if (keybuffer[VK_RIGHT])
	{
		fire.wid_a += 1;
		if (fire.wid_a > 5) {
			fire.wid_a = 10;
		}
		fire.dic = 1;
		fire.is_Move = TRUE;
	}
	if (keybuffer[VK_UP])
	{
		fire.is_Jumping = TRUE;
	}
	if (keybuffer[VK_DOWN])
	{
	//	button[0].On = FALSE;
	//	block[0].On = TRUE;
	}
	if (keybuffer['d'] || keybuffer['D'])
	{
		water.wid_a += 1;
		if (water.wid_a > 5) {
			water.wid_a = 10;
		}
		water.dic = 1;
		water.is_Move = TRUE;
	}
	if (keybuffer['a'] || keybuffer['A'])
	{
		water.wid_a += 1;
		if (water.wid_a > 5) {
			water.wid_a = 10;
		}
		water.dic = -1;
		water.is_Move = TRUE;
	}
	if (keybuffer['s'] || keybuffer['S'])
	{
	//	button[0].On = TRUE;
	}
	if (keybuffer['w'] || keybuffer['W'])
	{
		water.is_Jumping = TRUE;
	}
}
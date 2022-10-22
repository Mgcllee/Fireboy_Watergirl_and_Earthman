#pragma once
#include"MapMgr.h"

void Stage_1(int order) {
	if (order == 0) {
		water.x = 600;
		water.y = 730;
		fire.x = 700;
		fire.y = 730;

		fire.on = TRUE;
		water.on = TRUE;

		{
			Die.On = FALSE;
			for (int i = 0; i < 20; i++)
			{
				Jewelry[i].image_x = 0;
				Jewelry[i].image_y = 0;
				Jewelry[i].x = 0;
				Jewelry[i].y = 0;
				Jewelry[i].wid = 50;
				Jewelry[i].hei = 50;
			}
			for (int i = 0; i < 90; i++)
			{
				Trap[i].image_x = 0;
				Trap[i].image_y = 0;
				Trap[i].x = 0;
				Trap[i].y = 0;
				Trap[i].wid = 20;
				Trap[i].hei = 20;
			}

			Jewelry[0].On = TRUE;
			Jewelry[0].x = 300;
			Jewelry[0].y = 450;

			Jewelry[10].On = TRUE;
			Jewelry[10].x = 900;
			Jewelry[10].y = 450;

			blue_door.On = FALSE;
			blue_door.image_x = 0;
			blue_door.image_y = 0;
			blue_door.x = 480;
			blue_door.y = 300;

			red_door.On = FALSE;
			red_door.image_x = 0;
			red_door.image_y = 0;
			red_door.x = 630;
			red_door.y = 300;


			Ft[0].x = 100, Ft[0].y = 600;
			Ft[1].x = 250, Ft[1].y = 500;

			Ft[2].x = 410, Ft[2].y = 400, Ft[2].wid = 380;

			Ft[3].x = 850, Ft[3].y = 500;
			Ft[4].x = 1000, Ft[4].y = 600;
		}
	}
	else {
		blue_door.y = 300;
		red_door.y = 300;

		Ft[0].x = 100, Ft[0].y = 600;
		Ft[1].x = 250, Ft[1].y = 500;

		Ft[2].x = 410, Ft[2].y = 400, Ft[2].wid = 380;

		Ft[3].x = 850, Ft[3].y = 500;
		Ft[4].x = 1000, Ft[4].y = 600;
	}
	return;
}
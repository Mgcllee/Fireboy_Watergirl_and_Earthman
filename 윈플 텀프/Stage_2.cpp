#pragma once
#include"MapMgr.h"

void Stage_2(int order) {
	if (order == 0) {
		water.x = 100;
		water.y = 730;
		fire.x = 1100;
		fire.y = 730;

		fire.on = TRUE;
		water.on = TRUE;

		{
			Die.On = FALSE;
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

			for (int i = 0; i < 20; i++)
			{
				Ft[i].x = 0;
				Ft[i].y = 0;
			}
			Jewelry[0].On = TRUE;
			Jewelry[0].x = 450;
			Jewelry[0].y = 550;

			Jewelry[1].On = TRUE;
			Jewelry[1].x = 150;
			Jewelry[1].y = 450;

			Jewelry[2].On = TRUE;
			Jewelry[2].x = 750;
			Jewelry[2].y = 350;

			Jewelry[3].On = TRUE;
			Jewelry[3].x = 1000;
			Jewelry[3].y = 250;

			Jewelry[4].On = TRUE;
			Jewelry[4].x = 750;
			Jewelry[4].y = 200;

			Jewelry[10].On = TRUE;
			Jewelry[10].x = 750;
			Jewelry[10].y = 550;

			Jewelry[11].On = TRUE;
			Jewelry[11].x = 1050;
			Jewelry[11].y = 450;

			Jewelry[12].On = TRUE;
			Jewelry[12].x = 450;
			Jewelry[12].y = 350;

			Jewelry[13].On = TRUE;
			Jewelry[13].x = 200;
			Jewelry[13].y = 250;

			Jewelry[14].On = TRUE;
			Jewelry[14].x = 450;
			Jewelry[14].y = 200;
			// 0~19±îÁö ÆÄ¶û °¡¿îµ¥ 20~24 ÆÄ¶û¿Þ 25~29ÆÄ¶û¿À 30~49 »¡°­ °¡¿îµ¥ 50~54~»¡°­¿Þ 55~59»¡°­¿À 60~79ÃÊ·Ï °¡¿îµ¥ 80~84ÃÊ·Ï¿Þ 85~89ÃÊ·Ï¿À
			Trap[0].On = TRUE;
			Trap[0].x = 200;
			Trap[0].y = 730;

			Trap[20].On = TRUE;
			Trap[20].x = Trap[0].x - 20;
			Trap[20].y = 730;

			Trap[25].On = TRUE;
			Trap[25].x = Trap[0].x + 20;
			Trap[25].y = 730;

			Trap[30].On = TRUE;
			Trap[30].x = 900;
			Trap[30].y = 730;

			Trap[50].On = TRUE;
			Trap[50].x = Trap[30].x - 20;
			Trap[50].y = 730;

			Trap[55].On = TRUE;
			Trap[55].x = Trap[30].x + 20;
			Trap[55].y = 730;

			Trap[60].On = TRUE;
			Trap[60].x = 600;
			Trap[60].y = 730;

			Trap[80].On = TRUE;
			Trap[80].x = Trap[60].x - 20;
			Trap[80].y = 730;

			Trap[85].On = TRUE;
			Trap[85].x = Trap[60].x + 20;
			Trap[85].y = 730;

			Ft[0].x = 100, Ft[0].y = 500;
			Ft[1].x = 400, Ft[1].y = 600;

			Ft[2].x = 380, Ft[2].y = 400, Ft[2].wid = 450;

			Ft[3].x = 700, Ft[3].y = 600;
			Ft[4].x = 1000, Ft[4].y = 500;
			// 1Â÷ 
			Ft[5].x = 150, Ft[5].y = 300;
			Ft[6].x = 400, Ft[6].y = 250;

			Ft[7].x = 700, Ft[7].y = 250;
			Ft[8].x = 950, Ft[8].y = 300;
		}
	}
	else {
		blue_door.On = FALSE;
		blue_door.image_x = 0;
		blue_door.image_y = 0;
		blue_door.x = 1000;
		blue_door.y = 0;

		red_door.On = FALSE;
		red_door.image_x = 0;
		red_door.image_y = 0;
		red_door.x = 200;
		red_door.y = 0;
	}
	return;
}
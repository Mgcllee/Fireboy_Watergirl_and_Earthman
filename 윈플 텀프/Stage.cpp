#include "Stage.h"

void Stage::Loading()
{
}

void Stage::SelectCharacter()
{
}

// 스테이지 내부 오브젝트 위치 설정
void Stage::Stage_1() {
	players[0].x = 600;
	players[0].y = 730;
	players[1].x = 700;
	players[1].y = 730;

	if(Red_Jewel.empty() && Blue_Jewel.empty())
	{
		Die.SetVisible(false);
		Red_Jewel.clear();
		Blue_Jewel.clear();

		Trap.reserve(3);

		for (auto& t : Trap) {
			t = OBJECT{0, 0, 20, 20, 230, 23, FALSE};
		}
		
		if (Blue_Jewel.size() == 0 && Red_Jewel.size() == 0) {
			Blue_Jewel.push_back(OBJECT{ 300, 450, 28, 25, 1160, 29, TRUE });
			Red_Jewel.push_back(OBJECT{ 900, 450, 28, 25, 1160, 29, TRUE });
		}

		blue_door = OBJECT{ 480, 300, 0, 0, 0, 0, FALSE };
		red_door = OBJECT{ 630, 300, 0, 0, 0, 0, FALSE };

		Ft[0].x = 100, Ft[0].y = 600;
		Ft[1].x = 250, Ft[1].y = 500;

		Ft[2].x = 410, Ft[2].y = 400, Ft[2].wid = 380;

		Ft[3].x = 850, Ft[3].y = 500;
		Ft[4].x = 1000, Ft[4].y = 600;
	}
}
void Stage::Stage_2() {
	//if (order == 0) {
	//	water.x = 100;
	//	water.y = 730;
	//	fire.x = 1100;
	//	fire.y = 730;

	//	fire.on = TRUE;
	//	water.on = TRUE;

	//	{
	//		Die.On = FALSE;
	//		Die.On = FALSE;
	//		for (int i = 0; i < 20; i++)
	//		{
	//			Jewelry[i].image_x = 0;
	//			Jewelry[i].image_y = 0;
	//			Jewelry[i].x = 0;
	//			Jewelry[i].y = 0;
	//			Jewelry[i].wid = 50;
	//			Jewelry[i].hei = 50;
	//		}

	//		for (int i = 0; i < 90; i++)
	//		{
	//			Trap[i].image_x = 0;
	//			Trap[i].image_y = 0;
	//			Trap[i].x = 0;
	//			Trap[i].y = 0;
	//			Trap[i].wid = 20;
	//			Trap[i].hei = 20;
	//		}

	//		for (int i = 0; i < 20; i++)
	//		{
	//			Ft[i].x = 0;
	//			Ft[i].y = 0;
	//		}
	//		Jewelry[0].On = TRUE;
	//		Jewelry[0].x = 450;
	//		Jewelry[0].y = 550;

	//		Jewelry[1].On = TRUE;
	//		Jewelry[1].x = 150;
	//		Jewelry[1].y = 450;

	//		Jewelry[2].On = TRUE;
	//		Jewelry[2].x = 750;
	//		Jewelry[2].y = 350;

	//		Jewelry[3].On = TRUE;
	//		Jewelry[3].x = 1000;
	//		Jewelry[3].y = 250;

	//		Jewelry[4].On = TRUE;
	//		Jewelry[4].x = 750;
	//		Jewelry[4].y = 200;

	//		Jewelry[10].On = TRUE;
	//		Jewelry[10].x = 750;
	//		Jewelry[10].y = 550;

	//		Jewelry[11].On = TRUE;
	//		Jewelry[11].x = 1050;
	//		Jewelry[11].y = 450;

	//		Jewelry[12].On = TRUE;
	//		Jewelry[12].x = 450;
	//		Jewelry[12].y = 350;

	//		Jewelry[13].On = TRUE;
	//		Jewelry[13].x = 200;
	//		Jewelry[13].y = 250;

	//		Jewelry[14].On = TRUE;
	//		Jewelry[14].x = 450;
	//		Jewelry[14].y = 200;
	//		// 0~19까지 파랑 가운데 20~24 파랑왼 25~29파랑오 30~49 빨강 가운데 50~54~빨강왼 55~59빨강오 60~79초록 가운데 80~84초록왼 85~89초록오
	//		Trap[0].On = TRUE;
	//		Trap[0].x = 200;
	//		Trap[0].y = 730;

	//		Trap[20].On = TRUE;
	//		Trap[20].x = Trap[0].x - 20;
	//		Trap[20].y = 730;

	//		Trap[25].On = TRUE;
	//		Trap[25].x = Trap[0].x + 20;
	//		Trap[25].y = 730;

	//		Trap[30].On = TRUE;
	//		Trap[30].x = 900;
	//		Trap[30].y = 730;

	//		Trap[50].On = TRUE;
	//		Trap[50].x = Trap[30].x - 20;
	//		Trap[50].y = 730;

	//		Trap[55].On = TRUE;
	//		Trap[55].x = Trap[30].x + 20;
	//		Trap[55].y = 730;

	//		Trap[60].On = TRUE;
	//		Trap[60].x = 600;
	//		Trap[60].y = 730;

	//		Trap[80].On = TRUE;
	//		Trap[80].x = Trap[60].x - 20;
	//		Trap[80].y = 730;

	//		Trap[85].On = TRUE;
	//		Trap[85].x = Trap[60].x + 20;
	//		Trap[85].y = 730;

	//		Ft[0].x = 100, Ft[0].y = 500;
	//		Ft[1].x = 400, Ft[1].y = 600;

	//		Ft[2].x = 380, Ft[2].y = 400, Ft[2].wid = 450;

	//		Ft[3].x = 700, Ft[3].y = 600;
	//		Ft[4].x = 1000, Ft[4].y = 500;
	//		// 1차 
	//		Ft[5].x = 150, Ft[5].y = 300;
	//		Ft[6].x = 400, Ft[6].y = 250;

	//		Ft[7].x = 700, Ft[7].y = 250;
	//		Ft[8].x = 950, Ft[8].y = 300;
	//	}
}
void Stage::Stage_3() {
	{
		//	water.x = 100;
		//	water.y = 730;
		//	fire.x = 1100;
		//	fire.y = 730;

		//	fire.on = TRUE;
		//	water.on = TRUE;

		//	{
		//		Die.On = FALSE;
		//		Die.On = FALSE;
		//		for (int i = 0; i < 20; i++)
		//		{
		//			Jewelry[i].On = FALSE;
		//			Jewelry[i].image_x = 0;
		//			Jewelry[i].image_y = 0;
		//			Jewelry[i].x = 0;
		//			Jewelry[i].y = 0;
		//			Jewelry[i].wid = 50;
		//			Jewelry[i].hei = 50;
		//		}

		//		for (int i = 0; i < 90; i++)
		//		{
		//			Trap[i].On = FALSE;
		//			Trap[i].image_x = 0;
		//			Trap[i].image_y = 0;
		//			Trap[i].x = 0;
		//			Trap[i].y = 0;
		//			Trap[i].wid = 20;
		//			Trap[i].hei = 20;
		//		}

		//		for (int i = 0; i < 20; i++)
		//		{
		//			Ft[i].x = 0;
		//			Ft[i].y = 0;
		//		}

		//		blue_door.image_x = 0;
		//		blue_door.image_y = 0;
		//		blue_door.x = 0;
		//		blue_door.y = 0;

		//		red_door.image_x = 0;
		//		red_door.image_y = 0;
		//		red_door.x = 0;
		//		red_door.y = 0;

		//		Jewelry[0].On = TRUE;
		//		Jewelry[0].x = 450;
		//		Jewelry[0].y = 550;

		//		Jewelry[1].On = TRUE;
		//		Jewelry[1].x = 150;
		//		Jewelry[1].y = 450;

		//		Jewelry[2].On = TRUE;
		//		Jewelry[2].x = 750;
		//		Jewelry[2].y = 350;

		//		Jewelry[3].On = TRUE;
		//		Jewelry[3].x = 1000;
		//		Jewelry[3].y = 250;

		//		Jewelry[4].On = TRUE;
		//		Jewelry[4].x = 750;
		//		Jewelry[4].y = 200;

		//		Jewelry[10].On = TRUE;
		//		Jewelry[10].x = 750;
		//		Jewelry[10].y = 550;

		//		Jewelry[11].On = TRUE;
		//		Jewelry[11].x = 1050;
		//		Jewelry[11].y = 450;

		//		Jewelry[12].On = TRUE;
		//		Jewelry[12].x = 450;
		//		Jewelry[12].y = 350;

		//		Jewelry[13].On = TRUE;
		//		Jewelry[13].x = 200;
		//		Jewelry[13].y = 250;

		//		Jewelry[14].On = TRUE;
		//		Jewelry[14].x = 450;
		//		Jewelry[14].y = 200;
		//		// 0~19까지 파랑 가운데 20~24 파랑왼 25~29파랑오 30~49 빨강 가운데 50~54~빨강왼 55~59빨강오 60~79초록 가운데 80~84초록왼 85~89초록오
		//		Trap[31].On = TRUE;
		//		Trap[31].x = 200;
		//		Trap[31].y = 730;

		//		Trap[51].On = TRUE;
		//		Trap[51].x = Trap[31].x - 20;
		//		Trap[51].y = 730;

		//		Trap[56].On = TRUE;
		//		Trap[56].x = Trap[31].x + 20;
		//		Trap[56].y = 730;

		//		Trap[30].On = TRUE;
		//		Trap[30].x = 900;
		//		Trap[30].y = 730;

		//		Trap[50].On = TRUE;
		//		Trap[50].x = Trap[30].x - 20;
		//		Trap[50].y = 730;

		//		Trap[55].On = TRUE;
		//		Trap[55].x = Trap[30].x + 20;
		//		Trap[55].y = 730;

		//		Trap[60].On = TRUE;
		//		Trap[60].x = 600;
		//		Trap[60].y = 730;

		//		Trap[80].On = TRUE;
		//		Trap[80].x = Trap[60].x - 20;
		//		Trap[80].y = 730;

		//		Trap[85].On = TRUE;
		//		Trap[85].x = Trap[60].x + 20;
		//		Trap[85].y = 730;

		//		Ft[0].x = 150, Ft[0].y = 200;
		//		Ft[1].x = 100, Ft[1].y = 600;

		//		Ft[2].x = 380, Ft[2].y = 400, Ft[2].wid = 450;

		//		Ft[3].x = 700, Ft[3].y = 600;
		//		Ft[4].x = 1000, Ft[4].y = 500;
		//		// 1차 
		//		Ft[5].x = 150, Ft[5].y = 300;
		//		Ft[6].x = 400, Ft[6].y = 250;

		//		Ft[7].x = 700, Ft[7].y = 250;
		//		Ft[8].x = 950, Ft[8].y = 300;

		//		Ft[9].x = 900, Ft[9].y = 200;

		//		blue_door.On = FALSE;
		//		blue_door.image_x = 0;
		//		blue_door.image_y = 0;
		//		blue_door.x = 950;
		//		blue_door.y = 0;

		//		red_door.On = FALSE;
		//		red_door.image_x = 0;
		//		red_door.image_y = 0;
		//		red_door.x = 200;
		//		red_door.y = 0;

		//		block[0].On = FALSE;
		//		block[0].x = 0;
		//		block[0].y = 600;
		//		block[0].image_x = 0;
		//		block[0].image_y = 0;

		//		button[0].On = FALSE;
		//		button[0].x = 600;
		//		button[0].y = 730;
		//		button[0].image_x = 42;
		//		button[0].image_y = 15;

		//		Rt.x = 800;
		//		Rt.y = 730;
		//	}
		//}
	}
}

// 플레이어 점프 동작시 충돌 체크
void Stage::Jump() {
	//if (fire.Down) {
	//	if (fire.y + (fire.v + fire.g) > fire.ground) {
	//		fire.is_Jumping = FALSE;
	//		fire.Down = FALSE;
	//		fire.v = 50;
	//		fire.y = fire.ground;
	//	}
	//	if (fire.y < fire.ground) {
	//		fire.v = fire.v + fire.g;
	//		fire.y = fire.y + fire.v;
	//	}
	//}
	//if (water.Down) {
	//	if (water.y + (water.v + water.g) > water.ground) {
	//		water.is_Jumping = FALSE;
	//		water.Down = FALSE;
	//		water.v = 50;
	//		water.y = water.ground;
	//	}
	//	if (water.y < water.ground) {
	//		water.v = water.v + water.g;
	//		water.y = water.y + water.v;
	//	}
	//}
	//if (water.is_Jumping == TRUE) {
	//	if (!water.Down) {
	//		if (water.v >= 0) {
	//			water.v = water.v - water.g;
	//			water.y = water.y - (water.v / 2);
	//		}
	//		else {
	//			water.Down = TRUE;    // 상태 변화
	//		}
	//	}
	//}
	//if (fire.is_Jumping == TRUE) {
	//	if (!fire.Down) {
	//		if (fire.v > 0) {
	//			fire.v = fire.v - fire.g;
	//			fire.y = fire.y - (fire.v / 2);
	//		}
	//		else {
	//			fire.Down = TRUE;    // 상태 변화
	//		}
	//	}
	//}
}

// 플레이어 좌우 이동시 충돌 체크
//void Stage::Wid_Move() {
//	if (water.is_Move) {
//		if (water.dic == 1) {
//			if (water.wid_v <= 10) {
//				water.wid_v = water.wid_v + water.
//					;
//			}
//			water.x = water.x + water.wid_v;
//		}
//		else if (water.dic == -1) {
//			if (water.wid_v <= 10) {
//				water.wid_v = water.wid_v + water.wid_a;
//			}
//			water.x = water.x - water.wid_v;
//		}
//	}
//	if (water.is_Speed_Down) {
//		if (water.dic == 1) {
//			water.wid_a += 1;
//			water.wid_v = water.wid_v - water.wid_a;
//			water.x = water.x + water.wid_v;
//			if (water.wid_v <= 0) {
//				water.wid_v = 0;
//				water.wid_a = 0;
//				water.dic = 0;
//				water.is_Speed_Down = FALSE;
//			}
//		}
//		else if (water.dic == -1) {
//			water.wid_a += 1;
//			water.wid_v = water.wid_v - water.wid_a;
//			water.x = water.x - water.wid_v;
//			if (water.wid_v <= 0) {
//				water.wid_v = 0;
//				water.wid_a = 0;
//				water.dic = 0;
//				water.is_Speed_Down = FALSE;
//			}
//		}
//	}
//	if (fire.is_Move) {
//		if (fire.dic == 1) {
//			if (fire.wid_v <= 10) {
//				fire.wid_v = fire.wid_v + fire.wid_a;
//			}
//			fire.x = fire.x + fire.wid_v;
//		}
//		else if (fire.dic == -1) {
//			if (fire.wid_v <= 10) {
//				fire.wid_v = fire.wid_v + fire.wid_a;
//			}
//			fire.x = fire.x - fire.wid_v;
//		}
//	}
//	if (fire.is_Speed_Down) {
//		if (fire.dic == 1) {
//			fire.wid_a += 1;
//			fire.wid_v = fire.wid_v - fire.wid_a;
//			fire.x = fire.x + fire.wid_v;
//			if (fire.wid_v <= 0) {
//				fire.wid_v = 0;
//				fire.wid_a = 0;
//				fire.dic = 0;
//				fire.is_Speed_Down = FALSE;
//			}
//		}
//		else if (fire.dic == -1) {
//			fire.wid_a += 1;
//			fire.wid_v = fire.wid_v - fire.wid_a;
//			fire.x = fire.x - fire.wid_v;
//			if (fire.wid_v <= 0) {
//				fire.wid_v = 0;
//				fire.wid_a = 0;
//				fire.dic = 0;
//				fire.is_Speed_Down = FALSE;
//			}
//		}
//	}
//}

// Stage 03에 나오는 박스를 양쪽에서 밀었을 때 처리
void Stage::Push() {
	//if (water.is_Push == FALSE && fire.is_Push == FALSE) {
	//	if (water.y == Rt.y && abs(water.x - Rt.x) <= 60) {			// 불과 접촉
	//		water.is_Push = TRUE;
	//	}
	//	if (fire.y == Rt.y && abs(fire.x - Rt.x) <= 60) {		// 물과 접촉
	//		fire.is_Push = TRUE;
	//	}
	//}
	//// 충돌 체크
	//if (water.is_Push == TRUE && water.y == Rt.y && abs(water.x - Rt.x) <= 60) {
	//	if (Rt.dic == 0) {
	//		Rt.dic = water.dic;
	//	}

	//	if (Rt.dic == water.dic) {
	//		if (Rt.dic == 1) {
	//			Rt.x = water.x + 60;
	//		}
	//		else if (Rt.dic == -1) {
	//			Rt.x = water.x - 60;
	//		}
	//	}
	//}
	//else if (water.is_Push == TRUE && Rt.dic != water.dic) {
	//	Rt.dic = 0;
	//	water.is_Push = FALSE;
	//	return;
	//}

	//if (fire.is_Push == TRUE && fire.y == Rt.y && abs(fire.x - Rt.x) <= 60) {
	//	if (Rt.dic == 0) {
	//		Rt.dic = fire.dic;
	//	}

	//	if (Rt.dic == fire.dic) {
	//		if (Rt.dic == 1) {
	//			Rt.x = fire.x + 60;
	//		}
	//		else if (Rt.dic == -1) {
	//			Rt.x = fire.x - 60;
	//		}
	//	}
	//}
	//else if (fire.is_Push == TRUE && Rt.dic != fire.dic) {
	//	Rt.dic = 0;
	//	fire.is_Push = FALSE;
	//	return;
	//}

	//for (int i = 0; i < 20 && Ft[i].x != NULL; ++i) {
	//	if ((Rt.y == Ft[i].y && Ft[i].x > Rt.x - 60) || (Rt.y == Ft[i].y && Ft[i].x + Ft[i].wid < Rt.x)) {
	//		Rt.Down = TRUE;
	//	}
	//}
	//if (Rt.Down) {
	//	if (Rt.y + (Rt.v + Rt.g) >= 730) {
	//		Rt.v = 0;
	//		Rt.y = 730;
	//		Rt.Down = FALSE;
	//		return;
	//	}
	//	if (Rt.y <= 730) {
	//		Rt.v = Rt.v + Rt.g;
	//		Rt.y = Rt.y + Rt.v;
	//		return;
	//	}
	//}
}

// 스테이지에 나오는 발판들을 플레이어와 충돌 검사
void Stage::Foot() {
	//for (int i = 0; i < 20; ++i) {	// 발판의 최대수가 20개이므로 최대치 20
	//	if (Ft[i].W_On) {
	//		if (Ft[i].x > water.x || (water.x - water.wid) > (Ft[i].x + Ft[i].wid)) {    // 밖으로 나갔을 경우
	//			bool signal = FALSE;
	//			for (int beam = water.y; beam < 730; ++beam) {            // 빔 쏘기
	//				for (int j = 0; j < 20; ++j) {    // 블럭 하나씩 검사
	//					if (((Ft[j].y + average) - beam) < 10 && ((Ft[j].y + average) - beam) > -10) {
	//						if (Ft[j].x < water.x - 30 && water.x - 30 < Ft[j].x + Ft[j].wid) {            // if (Ft[i].x <= water.x && water.x - 60 <= Ft[i].x + Ft[i].wid)
	//							water.ground = beam;
	//							Ft[j].W_On = FALSE;
	//							signal = TRUE;
	//						}
	//					}
	//				}
	//			}
	//			if (!signal) {
	//				water.ground = 730;
	//			}
	//			if (water.is_Jumping == FALSE) {
	//				water.v = 0;
	//				water.Down = TRUE;
	//			}
	//			Ft[i].W_On = FALSE;
	//		}
	//	}
	//	if (Ft[i].F_On) {
	//		if (Ft[i].x > fire.x || fire.x - 60 > Ft[i].x + Ft[i].wid) {    // 밖으로 나갔을 경우
	//			bool signal = FALSE;
	//			for (int beam = fire.y; beam < 730; ++beam) {            // 빔 쏘기
	//				for (int j = 0; j < 20; ++j) {    // 블럭 하나씩 검사
	//					if ((Ft[j].y + average) - beam < 10 && (Ft[j].y + average) - beam > -10) {
	//						if (Ft[j].x < fire.x && fire.x - 60 < Ft[j].x + Ft[j].wid) {
	//							fire.ground = beam;
	//							Ft[j].W_On = FALSE;
	//							signal = TRUE;
	//						}
	//					}
	//				}
	//			}
	//			if (!signal) {
	//				fire.ground = 730;
	//			}
	//			if (fire.is_Jumping == FALSE) {
	//				fire.v = 0;
	//				fire.Down = TRUE;
	//			}
	//			Ft[i].F_On = FALSE;
	//		}
	//	}
	//	if (water.is_Jumping == TRUE) {
	//		if (Ft[i].x <= water.x && water.x - 60 <= Ft[i].x + Ft[i].wid) {
	//			if ((water.y - 70) - (Ft[i].y + Ft[i].hei + average) <= 5 && (water.y - 70) - (Ft[i].y + Ft[i].hei + average) > Ft[i].hei * -1) {    // 머리 부닥치기
	//				water.v = 0;
	//				water.Down = TRUE;
	//			}
	//			if (Ft[i].y + average - water.y <= 5 && Ft[i].y + average - water.y > -20) {        // 안착
	//				water.is_Jumping = FALSE;
	//				water.Down = FALSE;
	//				water.v = 50;
	//				water.y = Ft[i].y + average;
	//				water.ground = Ft[i].y + average;
	//				Ft[i].W_On = TRUE;
	//			}
	//		}
	//		if (Ft[i].y + average > water.y - 60 && Ft[i].y + Ft[i].hei + average < water.y) {
	//			if (Ft[i].x - 5 - water.x < 20 && Ft[i].x - 5 - water.x > 0) {
	//				water.wid_a = 0;
	//				water.wid_v = 0;
	//			}
	//			if ((water.x - 60) - (Ft[i].x + Ft[i].wid + 5) < 20 && (water.x - 60) - (Ft[i].x + Ft[i].wid + 5) > 0) {
	//				water.wid_a = 0;
	//				water.wid_v = 0;
	//			}
	//		}
	//	}
	//	if (fire.is_Jumping == TRUE) {
	//		if (Ft[i].x < fire.x && fire.x - 60 < Ft[i].x + Ft[i].wid) {
	//			if ((fire.y - 70) - (Ft[i].y + Ft[i].hei + average) <= 5 && (fire.y - 70) - (Ft[i].y + Ft[i].hei + average) > Ft[i].hei * -1) {
	//				fire.v = 0;
	//				fire.Down = TRUE;
	//			}
	//			if (Ft[i].y + average - fire.y <= 5 && Ft[i].y + average - fire.y > -20) {
	//				fire.is_Jumping = FALSE;
	//				fire.Down = FALSE;
	//				fire.v = 50;
	//				fire.y = Ft[i].y + average;
	//				fire.ground = Ft[i].y + average;
	//				Ft[i].F_On = TRUE;
	//			}
	//		}
	//		if (Ft[i].y + average > fire.y - 70 && Ft[i].y + Ft[i].hei + average < fire.y) {
	//			if (Ft[i].x - 5 - fire.x < 20 && Ft[i].x - 5 - fire.x > 0) {
	//				fire.wid_a = 0;
	//				fire.wid_v = 0;
	//			}
	//			if ((fire.x - 60) - (Ft[i].x + Ft[i].wid + 5) < 20 && (fire.x - 60) - (Ft[i].x + Ft[i].wid + 5) > 0) {
	//				fire.wid_a = 0;
	//				fire.wid_v = 0;
	//			}
	//		}
	//	}
	//}
}

#pragma once
#include "stdafx.h"
#include "Stage.h"

void Stage::Stage_1() {
	threadHandles[0].x = 600;
	threadHandles[0].y = 730;
	threadHandles[0].v = 0.f;
	threadHandles[0].wid_v = 0.f;

	threadHandles[1].x = 700;
	threadHandles[1].y = 730;
	threadHandles[1].v = 0.f;
	threadHandles[1].wid_v = 0.f;

	threadHandles[2].x = 500;
	threadHandles[2].y = 730;
	threadHandles[2].v = 0.f;
	threadHandles[2].wid_v = 0.f;

	Die.SetVisible(false);

	//명철 인지
	//jewely queeue Clear
	for (int i = 0; i < jewely.size(); i++)
		jewely.pop();

	Trap.reserve(3);

	for (auto& t : Trap) {
		t = OBJECT{ 0, 0, 20, 20, 230, 23, FALSE };
	}

	//명철 인지
	//queue니까 일단 첫번째 보석은 current 한테 주고
	currentVisibleJewely = OBJECT{ 300, 450, 28, 25, 1160, 29, TRUE };
	//jewely.emplace(OBJECT{ 300, 450, 28, 25, 1160, 29, TRUE });
	//아직 안보여줄 보석은 큐에 저장
	jewely.emplace(OBJECT{ 900, 450, 28, 25, 1160, 29, FALSE });

	//보석 최대 갯수 정해주고
	maxJewelyNum = 2;

	/*jewely.emplace_back();
	jewely.emplace_back();*/

	door = OBJECT{ 700, 740, 60, 100, 0, 0, TRUE };
	//red_door = OBJECT{ 630, 400, 60, 100, 1260, 60, TRUE };

	Ft.push_back(Ground);
	Ft.emplace_back(OBJECT{ 100, 630, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 300, 550, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 600, 450, 224, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 900, 550, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 1100, 630, 112, 24, 0, 0, TRUE });

}
void Stage::Stage_2() {
	for (int i = 0; i < jewely.size(); i++)
		jewely.pop();
	Ft.clear();
	//door = OBJECT{ 700, 630, 60, 100, 0, 0, TRUE };
	maxJewelyNum = 8;

	threadHandles[0].x = 600;
	threadHandles[0].y = 730;
	threadHandles[0].v = 0.f;
	threadHandles[0].wid_v = 0.f;

	threadHandles[1].x = 700;
	threadHandles[1].y = 730;
	threadHandles[1].v = 0.f;
	threadHandles[1].wid_v = 0.f;

	threadHandles[2].x = 500;
	threadHandles[2].y = 730;
	threadHandles[2].v = 0.f;
	threadHandles[2].wid_v = 0.f;
	door = OBJECT{ 700, 250 - 24, 60, 100, 0, 0, TRUE };

	Ft.push_back(Ground);
	Ft.emplace_back(OBJECT{ 100, 500, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 400, 600, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 380, 400, 224, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 700, 600, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 1000, 500, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 150, 300, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 400, 250, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 700, 250, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 950, 300, 112, 24, 0, 0, TRUE });

	currentVisibleJewely = OBJECT{ 450, 550, 28, 25, 1160, 29, TRUE };
	//아직 안보여줄 보석은 큐에 저장
	jewely.emplace(OBJECT{ 150, 450, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 750, 350, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 1000, 250, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 750, 200, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 750, 550, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 1050, 450, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 450, 350, 28, 25, 1160, 29, FALSE });
}

void Stage::Stage_3() {
	for (int i = 0; i < jewely.size(); i++)
		jewely.pop();
	Ft.clear();

	threadHandles[0].x = 100;
	threadHandles[0].y = 730;
	threadHandles[0].v = 0.f;
	threadHandles[0].wid_v = 0.f;

	threadHandles[1].x = 1100;
	threadHandles[1].y = 730;
	threadHandles[1].v = 0.f;
	threadHandles[1].wid_v = 0.f;

	threadHandles[2].x = 500;
	threadHandles[2].y = 730;
	threadHandles[2].v = 0.f;
	threadHandles[2].wid_v = 0.f;
	
	door = OBJECT{ 700, 800, 60, 100, 0, 0, TRUE };

	Ft.push_back(Ground);
	Ft.emplace_back(OBJECT{ 150, 200, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 100, 600, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 380, 400, 450, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 700, 600, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 1000, 500, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 150, 300, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 400, 250, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 700, 250, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 950, 300, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 900, 200, 112, 24, 0, 0, TRUE });

	maxJewelyNum = 8;
	currentVisibleJewely = OBJECT{ 450, 550, 28, 25, 1160, 29, TRUE };
	jewely.emplace(OBJECT{ 150, 450, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 750, 350, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 1000, 250, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 750, 200, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 750, 550, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 1050, 450, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 450, 350, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 200, 250, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 450, 200, 28, 25, 1160, 29, FALSE });

	block = OBJECT{ 0, 600, 28, 25, 0, 0, TRUE };
	button = OBJECT{ 600, 717, 30, 30, 0, 0, TRUE };
}

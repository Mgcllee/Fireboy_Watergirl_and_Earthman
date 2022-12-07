#include"stdafx.h"
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

	players[2].x = 500;
	players[2].y = 730;


	Die.SetVisible(false);
	currentVisibleJewely = OBJECT{ 300, 450, 28, 25, 1160, 29, TRUE };
	jewely.emplace(OBJECT{ 900, 450, 28, 25, 1160, 29, TRUE });

	maxJewelyNum = 2;
	Trap.reserve(3);

	for (auto& t : Trap) {
		t = OBJECT{ 0, 0, 20, 20, 230, 23, FALSE };
	}

	door = OBJECT{ 700, 740, 60, 100, 0, 0, TRUE };

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

	door = OBJECT{ 700, 250 - 24, 60, 100, 0, 0, TRUE };

	maxJewelyNum = 8;
	players[0].x = 600;
	players[0].y = 730;

	players[1].x = 700;
	players[1].y = 730;

	players[2].x = 500;
	players[2].y = 730;

	Ft.emplace_back(OBJECT{ 100, 500, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 400, 600, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 380, 400, 224, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 700, 600, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 1000, 500, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 150, 300, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 400, 250, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 700, 250, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 950, 300, 112, 24, 0, 0, TRUE });

	maxJewelyNum = 8;
	currentVisibleJewely = OBJECT{ 450, 550, 28, 25, 1160, 29, TRUE };
	jewely.emplace(OBJECT{ 150, 450, 28, 25, 1160, 29, TRUE });
	jewely.emplace(OBJECT{ 750, 350, 28, 25, 1160, 29, TRUE });
	jewely.emplace(OBJECT{ 1000, 250, 28, 25, 1160, 29, TRUE });
	jewely.emplace(OBJECT{ 750, 200, 28, 25, 1160, 29, TRUE });
	jewely.emplace(OBJECT{ 750, 550, 28, 25, 1160, 29, TRUE });
	jewely.emplace(OBJECT{ 1050, 450, 28, 25, 1160, 29, TRUE });
	jewely.emplace(OBJECT{ 450, 350, 28, 25, 1160, 29, TRUE });
}

void Stage::Stage_3() {
	for (int i = 0; i < jewely.size(); i++)
		jewely.pop();
	Ft.clear();

	players[0].x = 100;
	players[0].y = 730;

	players[1].x = 1100;
	players[1].y = 730;

	players[2].x = 500;
	players[2].y = 730;

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
	
	door = OBJECT{ 700, 800, 60, 100, 0, 0, TRUE };

	block = OBJECT{ 0, 600, 28, 25, 0, 0, TRUE };
	button = OBJECT{ 600, 717, 30, 30, 0, 0, TRUE };
}

#pragma once

#include "Stage.h"

void Stage::Stage_1(array<StagePosition, 3>& positions) {
	for (StagePosition& pl : positions) {
		pl.isJump = false;
	}

	positions[0].x = 600;
	positions[0].y = 730;
	positions[0].v = 0.f;
	positions[0].wid_v = 0.f;
	positions[0].onBoard = Ground;

	positions[1].x = 700;
	positions[1].y = 730;
	positions[1].v = 0.f;
	positions[1].wid_v = 0.f;
	positions[1].onBoard = Ground;

	positions[2].x = 500;
	positions[2].y = 730;
	positions[2].v = 0.f;
	positions[2].wid_v = 0.f;
	positions[2].onBoard = Ground;

	Die.SetVisible(false);

	for (int i = 0; i < jewely.size(); i++)
		jewely.pop();

	Trap.reserve(3);

	for (auto& t : Trap) {
		t = OBJECT{ 0, 0, 20, 20, 230, 23, FALSE };
	}

	currentVisibleJewely = OBJECT{ 300, 450, 28, 25, 1160, 29, TRUE };
	jewely.emplace(OBJECT{ 900, 450, 28, 25, 1160, 29, FALSE });

	maxJewelyNum = 2;


	door = OBJECT{ 700, 740, 60, 100, 0, 0, TRUE };

	Ft.push_back(Ground);
	Ft.emplace_back(OBJECT{ 100, 630, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 300, 550, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 600, 450, 224, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 900, 550, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 1100, 630, 112, 24, 0, 0, TRUE });

}

void Stage::Stage_2(array<StagePosition, 3>& positions) {
	for (int i = 0; i < jewely.size(); i++)
		jewely.pop();
	Ft.clear();

	for (ThreadInfo& pl : positions)
		pl.isJump = false;

	positions[0].x = 600;
	positions[0].y = 730;
	positions[0].v = 0.f;
	positions[0].wid_v = 0.f;
	positions[0].ground = 730;

	positions[1].x = 700;
	positions[1].y = 730;
	positions[1].v = 0.f;
	positions[1].wid_v = 0.f;
	positions[1].ground = 730;

	positions[2].x = 500;
	positions[2].y = 730;
	positions[2].v = 0.f;
	positions[2].wid_v = 0.f;
	positions[2].ground = 730;

	door = OBJECT{ 700, 400 - 4, 60, 100, 0, 0, TRUE };

	Ft.push_back(Ground);
	Ft.emplace_back(OBJECT{ 100, 520, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 300, 620, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 360, 420, 224, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 700, 620, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 840, 520, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 350, 270, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 680, 420, 112, 24, 0, 0, TRUE });

	maxJewelyNum = 8;
	currentVisibleJewely = OBJECT{ 450, 550, 28, 25, 1160, 29, TRUE };
	jewely.emplace(OBJECT{ 150, 470, 28, 25, 1160, 29, TRUE });
	jewely.emplace(OBJECT{ 750, 370, 28, 25, 1160, 29, TRUE });
	jewely.emplace(OBJECT{ 840, 570, 28, 25, 1160, 29, TRUE });
	jewely.emplace(OBJECT{ 750, 220, 28, 25, 1160, 29, TRUE });
	jewely.emplace(OBJECT{ 750, 570, 28, 25, 1160, 29, TRUE });
	jewely.emplace(OBJECT{ 840, 570, 28, 25, 1160, 29, TRUE });
	jewely.emplace(OBJECT{ 450, 370, 28, 25, 1160, 29, TRUE });
}

void Stage::Stage_3(array<StagePosition, 3>& positions) {
	for (int i = 0; i < jewely.size(); i++)
		jewely.pop();
	Ft.clear();

	for (ThreadInfo& pl : positions) {
		pl.isJump = false;
	}

	positions[0].x = 550;
	positions[0].y = 730;
	positions[0].v = 0.f;
	positions[0].wid_v = 0.f;

	positions[1].x = 600;
	positions[1].y = 730;
	positions[1].v = 0.f;
	positions[1].wid_v = 0.f;

	positions[2].x = 500;
	positions[2].y = 730;
	positions[2].v = 0.f;
	positions[2].wid_v = 0.f;

	door = OBJECT{ 700, 740, 60, 100, 0, 0, TRUE };

	Ft.push_back(Ground);
	Ft.emplace_back(OBJECT{ 280, 700, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 700, 650, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 1100, 620, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 330, 650, 450, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 150, 550, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 400, 510, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 700, 510, 112, 24, 0, 0, TRUE });
	Ft.emplace_back(OBJECT{ 900, 580, 112, 24, 0, 0, TRUE });

	maxJewelyNum = 8;
	currentVisibleJewely = OBJECT{ 450, 550, 28, 25, 1160, 29, TRUE };
	jewely.emplace(OBJECT{ 150, 450, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 750, 350, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 1000, 350, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 750, 450, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 750, 550, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 1050, 450, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 450, 350, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 200, 450, 28, 25, 1160, 29, FALSE });
	jewely.emplace(OBJECT{ 450, 350, 28, 25, 1160, 29, FALSE });
}

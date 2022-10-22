#pragma once
#include "MapMgr.h"

void Jump() {
	if (fire.Down) {
		if (fire.y + (fire.v + fire.g) > fire.ground) {
			fire.is_Jumping = FALSE;
			fire.Down = FALSE;
			fire.v = 50;
			fire.y = fire.ground;
		}
		if (fire.y < fire.ground) {
			fire.v = fire.v + fire.g;
			fire.y = fire.y + fire.v;
		}
	}
	if (water.Down) {
		if (water.y + (water.v + water.g) > water.ground) {
			water.is_Jumping = FALSE;
			water.Down = FALSE;
			water.v = 50;
			water.y = water.ground;
		}
		if (water.y < water.ground) {
			water.v = water.v + water.g;
			water.y = water.y + water.v;
		}
	}
	if (water.is_Jumping == TRUE) {
		if (!water.Down) {
			if (water.v >= 0) {
				water.v = water.v - water.g;
				water.y = water.y - (water.v / 2);
			}
			else {
				water.Down = TRUE;    // 상태 변화
			}
		}
	}
	if (fire.is_Jumping == TRUE) {
		if (!fire.Down) {
			if (fire.v > 0) {
				fire.v = fire.v - fire.g;
				fire.y = fire.y - (fire.v / 2);
			}
			else {
				fire.Down = TRUE;    // 상태 변화
			}
		}
	}
}

void Wid_Move() {
	if (water.is_Move) {
		if (water.dic == 1) {
			if (water.wid_v <= 10) {
				water.wid_v = water.wid_v + water.wid_a;
			}
			water.x = water.x + water.wid_v;
		}
		else if (water.dic == -1) {
			if (water.wid_v <= 10) {
				water.wid_v = water.wid_v + water.wid_a;
			}
			water.x = water.x - water.wid_v;
		}
	}
	if (water.is_Speed_Down) {
		if (water.dic == 1) {
			water.wid_a += 1;
			water.wid_v = water.wid_v - water.wid_a;
			water.x = water.x + water.wid_v;
			if (water.wid_v <= 0) {
				water.wid_v = 0;
				water.wid_a = 0;
				water.dic = 0;
				water.is_Speed_Down = FALSE;
			}
		}
		else if (water.dic == -1) {
			water.wid_a += 1;
			water.wid_v = water.wid_v - water.wid_a;
			water.x = water.x - water.wid_v;
			if (water.wid_v <= 0) {
				water.wid_v = 0;
				water.wid_a = 0;
				water.dic = 0;
				water.is_Speed_Down = FALSE;
			}
		}
	}
	if (fire.is_Move) {
		if (fire.dic == 1) {
			if (fire.wid_v <= 10) {
				fire.wid_v = fire.wid_v + fire.wid_a;
			}
			fire.x = fire.x + fire.wid_v;
		}
		else if (fire.dic == -1) {
			if (fire.wid_v <= 10) {
				fire.wid_v = fire.wid_v + fire.wid_a;
			}
			fire.x = fire.x - fire.wid_v;
		}
	}
	if (fire.is_Speed_Down) {
		if (fire.dic == 1) {
			fire.wid_a += 1;
			fire.wid_v = fire.wid_v - fire.wid_a;
			fire.x = fire.x + fire.wid_v;
			if (fire.wid_v <= 0) {
				fire.wid_v = 0;
				fire.wid_a = 0;
				fire.dic = 0;
				fire.is_Speed_Down = FALSE;
			}
		}
		else if (fire.dic == -1) {
			fire.wid_a += 1;
			fire.wid_v = fire.wid_v - fire.wid_a;
			fire.x = fire.x - fire.wid_v;
			if (fire.wid_v <= 0) {
				fire.wid_v = 0;
				fire.wid_a = 0;
				fire.dic = 0;
				fire.is_Speed_Down = FALSE;
			}
		}
	}
}

void Push() {
	if (water.is_Push == FALSE && fire.is_Push == FALSE) {
		if (water.y == Rt.y && abs(water.x - Rt.x) <= 60) {			// 불과 접촉
			water.is_Push = TRUE;
		}
		if (fire.y == Rt.y && abs(fire.x - Rt.x) <= 60) {		// 물과 접촉
			fire.is_Push = TRUE;
		}
	}
	// 충돌 체크
	if (water.is_Push == TRUE && water.y == Rt.y && abs(water.x - Rt.x) <= 60) {
		if (Rt.dic == 0) {
			Rt.dic = water.dic;
		}

		if (Rt.dic == water.dic) {
			if (Rt.dic == 1) {
				Rt.x = water.x + 60;
			}
			else if (Rt.dic == -1) {
				Rt.x = water.x - 60;
			}
		}
	}
	else if (water.is_Push == TRUE && Rt.dic != water.dic) {
		Rt.dic = 0;
		water.is_Push = FALSE;
		return;
	}

	if (fire.is_Push == TRUE && fire.y == Rt.y && abs(fire.x - Rt.x) <= 60) {
		if (Rt.dic == 0) {
			Rt.dic = fire.dic;
		}

		if (Rt.dic == fire.dic) {
			if (Rt.dic == 1) {
				Rt.x = fire.x + 60;
			}
			else if (Rt.dic == -1) {
				Rt.x = fire.x - 60;
			}
		}
	}
	else if (fire.is_Push == TRUE && Rt.dic != fire.dic) {
		Rt.dic = 0;
		fire.is_Push = FALSE;
		return;
	}

	for (int i = 0; i < 20 && Ft[i].x != NULL; ++i) {
		if ((Rt.y == Ft[i].y && Ft[i].x > Rt.x - 60) || (Rt.y == Ft[i].y && Ft[i].x + Ft[i].wid < Rt.x)) {
			Rt.Down = TRUE;
		}
	}
	if (Rt.Down) {
		if (Rt.y + (Rt.v + Rt.g) >= 730) {
			Rt.v = 0;
			Rt.y = 730;
			Rt.Down = FALSE;
			return;
		}
		if (Rt.y <= 730) {
			Rt.v = Rt.v + Rt.g;
			Rt.y = Rt.y + Rt.v;
			return;
		}
	}
}

void Foot() {
	for (int i = 0; i < 20; ++i) {
		if (Ft[i].W_On) {
			if (Ft[i].x > water.x || (water.x - 60) > (Ft[i].x + Ft[i].wid)) {    // 밖으로 나갔을 경우
				bool signal = FALSE;
				for (int beam = water.y; beam < 730; ++beam) {            // 빔 쏘기
					for (int j = 0; j < 20; ++j) {    // 블럭 하나씩 검사
						if (((Ft[j].y + average) - beam) < 10 && ((Ft[j].y + average) - beam) > -10) {
							if (Ft[j].x < water.x - 30 && water.x - 30 < Ft[j].x + Ft[j].wid) {            // if (Ft[i].x <= water.x && water.x - 60 <= Ft[i].x + Ft[i].wid)
								water.ground = beam;
								Ft[j].W_On = FALSE;
								signal = TRUE;
							}
						}
					}
				}
				if (!signal) {
					water.ground = 730;
				}
				if (water.is_Jumping == FALSE) {
					water.v = 0;
					water.Down = TRUE;
				}
				Ft[i].W_On = FALSE;
			}
		}
		if (Ft[i].F_On) {
			if (Ft[i].x > fire.x || fire.x - 60 > Ft[i].x + Ft[i].wid) {    // 밖으로 나갔을 경우
				bool signal = FALSE;
				for (int beam = fire.y; beam < 730; ++beam) {            // 빔 쏘기
					for (int j = 0; j < 20; ++j) {    // 블럭 하나씩 검사
						if ((Ft[j].y + average) - beam < 10 && (Ft[j].y + average) - beam > -10) {
							if (Ft[j].x < fire.x && fire.x - 60 < Ft[j].x + Ft[j].wid) {
								fire.ground = beam;
								Ft[j].W_On = FALSE;
								signal = TRUE;
							}
						}
					}
				}
				if (!signal) {
					fire.ground = 730;
				}
				if (fire.is_Jumping == FALSE) {
					fire.v = 0;
					fire.Down = TRUE;
				}
				Ft[i].F_On = FALSE;
			}
		}
		if (water.is_Jumping == TRUE) {
			if (Ft[i].x <= water.x && water.x - 60 <= Ft[i].x + Ft[i].wid) {
				if ((water.y - 70) - (Ft[i].y + Ft[i].hei + average) <= 5 && (water.y - 70) - (Ft[i].y + Ft[i].hei + average) > Ft[i].hei * -1) {    // 머리 부닥치기
					water.v = 0;
					water.Down = TRUE;
				}
				if (Ft[i].y + average - water.y <= 5 && Ft[i].y + average - water.y > -20) {        // 안착
					water.is_Jumping = FALSE;
					water.Down = FALSE;
					water.v = 50;
					water.y = Ft[i].y + average;
					water.ground = Ft[i].y + average;
					Ft[i].W_On = TRUE;
				}
			}
			if (Ft[i].y + average > water.y - 60 && Ft[i].y + Ft[i].hei + average < water.y) {
				if (Ft[i].x - 5 - water.x < 20 && Ft[i].x - 5 - water.x > 0) {
					water.wid_a = 0;
					water.wid_v = 0;
				}
				if ((water.x - 60) - (Ft[i].x + Ft[i].wid + 5) < 20 && (water.x - 60) - (Ft[i].x + Ft[i].wid + 5) > 0) {
					water.wid_a = 0;
					water.wid_v = 0;
				}
			}
		}
		if (fire.is_Jumping == TRUE) {
			if (Ft[i].x < fire.x && fire.x - 60 < Ft[i].x + Ft[i].wid) {
				if ((fire.y - 70) - (Ft[i].y + Ft[i].hei + average) <= 5 && (fire.y - 70) - (Ft[i].y + Ft[i].hei + average) > Ft[i].hei * -1) {
					fire.v = 0;
					fire.Down = TRUE;
				}
				if (Ft[i].y + average - fire.y <= 5 && Ft[i].y + average - fire.y > -20) {
					fire.is_Jumping = FALSE;
					fire.Down = FALSE;
					fire.v = 50;
					fire.y = Ft[i].y + average;
					fire.ground = Ft[i].y + average;
					Ft[i].F_On = TRUE;
				}
			}
			if (Ft[i].y + average > fire.y - 70 && Ft[i].y + Ft[i].hei + average < fire.y) {
				if (Ft[i].x - 5 - fire.x < 20 && Ft[i].x - 5 - fire.x > 0) {
					fire.wid_a = 0;
					fire.wid_v = 0;
				}
				if ((fire.x - 60) - (Ft[i].x + Ft[i].wid + 5) < 20 && (fire.x - 60) - (Ft[i].x + Ft[i].wid + 5) > 0) {
					fire.wid_a = 0;
					fire.wid_v = 0;
				}
			}
		}
	}
}

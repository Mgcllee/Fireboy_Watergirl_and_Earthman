#pragma once
#include<wtypes.h>

#define S2CLoading				0
#define S2CChangeStage			1
#define S2CJewelryVisibility	2

//RolePacket 구분
#define C2SSelectRole			3	// 케릭터 선택시 클라이언트 => 서버 로 역할 전송
#define S2CSelectRole			4	// 서버 => 클라이언트 한테 역할을 전송
#define C2SChangRole			5	// 다른 클라이언트가 어떤 역할을 보고 있는지 알려주기 위한 패킷
#define S2CChangeRole			6	// 선택이 아닌 케릭터 뭘 보고 있는지 다른 클라이언트도 알아야 하니까 있는 패킷

//MovePacket 구분
#define C2SMove					7

//type 구분
#define S2CDoorVisible			12	// 문열리는걸 알리는 패킷 => 문 보이는걸 알려줌
#define S2CAddPlayer			13	// 다른 플레이어 정보 받기

#define S2CStageTimePass		14	// 경과 시간 패킷
#define S2CStageTimeout			15	//타임 아웃

// 캐릭터 이동방향 구분
#define S2CMove_IDLE			16
#define S2CMove_JUMP			17
#define S2CMove_LEFT			18
#define S2CMove_RIGHT			19
#define S2CBTN_DOWN				20	// 버튼 눌림을 알려주는 패킷타입
#define S2CBTN_UP				21	// 버튼 눌림을 알려주는 패킷타입

#define S2CEatJewely			22	// 보석을 누군가 먹었으니 다음 보석을 띄우라고 알려주기 위한 패킷
#define S2CIntoDoor				23	// 문으로 들어가는 애니메이션을 위한 패킷 타입

#define C2SEndout				24
#define S2CEndout				25
#define S2CPlayerOut			26

#define PORT_NUM				9000

#pragma pack(push, 1)

struct S2CPlayerPacket { // 다른 클라이언트 기다리기위한 패킷  -> 보석 먹은것도 이걸로 쓰자, Door 들어가는 것도
	char type;
	char id; // 클라이언트 고유 id 전송
};

struct S2CChangeStagePacket { // 스테이지를 변경하기 위한 패킷
	char type;
	short stageNum; // 스테이지 구분을 위한 정보
};

struct C2SRolePacket {
	char type;
	char role; //역할 정보
};

struct S2CRolePacket {
	char type;
	char id;		// 어떤 클라이언트인지 구분을 위한 정보
	char role;		// 역할 정보
};

struct MovePacket { // 움직임 동작 패킷
	char type;
	char id;		// 클라이언트 구분 패킷
	short x, y;		// 현재 위치
};

//스테이지 타임 아웃

struct typePacket {
	char type;
};


//스테이지 경과 시간
struct S2CStageTimePassPacket {
	char type = S2CStageTimePass;
	int timePassed;
};



#pragma pack(pop)


//TestCommit
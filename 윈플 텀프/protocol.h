#pragma once
#include<wtypes.h>

#define S2CLoading 0
#define S2CAddPlayer 13// 다른 플레이어 정보 받기

#define S2CChangeStage 1
#define S2CJewelryVisibility 2
//RolePacket 구분
#define C2SSelectRole 3// 케릭터 선택시 클라이언트 => 서버 로 역할 전송
#define S2CSelectRole 4// 서버 => 클라이언트 한테 역할을 전송
#define C2SChangeRole 5 // 다른 클라이언트가 어떤 역할을 보고 있는지 알려주기 위한 패킷
#define S2CChangeRole 6// 선택이 아닌 케릭터 뭘 보고 있는지 다른 클라이언트도 알아야 하니까 있는 패킷
//MovePacket 구분
#define C2SMove 7
#define S2CMove 8
//type 구분
#define C2SExitGame 9// 클라이언트가 게임 종료를 원할 때, 전송
#define S2CExitGame 10// 서버에서 다른 클라이언트에게 정보 알림
#define C2SRetry 11// 클라이언트가 서버에게 이번 스테이지 재시도 요청 패킷
#define S2CDoorOpen 12// 문열리는걸 알리는 패킷
#define S2CStageTimePass 14// 경과 시간 패킷
#define S2CStageTimeout 15//타임 아웃


#define PORT_NUM 9000

#pragma pack(push, 1)

struct S2CPlayerPacket { // 다른 클라이언트 기다리기위한 패킷
	//unsigned char size;
	char type;
	char id; // 클라이언트 고유 id 전송
};

struct S2CChangeStagePacket { // 스테이지를 변경하기 위한 패킷
	//unsigned char size;
	char type;
	short stageNum; // 스테이지 구분을 위한 정보
};

struct S2CJewelryVisibilityPacket { // 서버에서 보석을 먹었다고 판단 후, 클라이언트에게 정보를 전달
	//unsigned char size;
	char type;
	char jewelryType; // 어떤 종류의 보석인지
	int index; // 몇번째 인덱스에 저장된 보석인지
};

struct C2SRolePacket {
	//unsigned char size;
	char type;
	char role; //역할 정보
};

struct S2CRolePacket {
	//unsigned char size;
	char type;
	char id; // 어떤 클라이언트인지 구분을 위한 정보
	char role; // 역할 정보
};

struct MovePacket { // 움직임 동작 패킷
	//unsigned char size;
	char type;
	char id; // 클라이언트 구분 패킷
	short x, y; // 현재 위치
};

struct typePacket {
	//unsigned char size;
	char type;
};

//흐른 시간 패킷
struct S2CStageTimePassPacket {
	char type;
	int timePassed;
};
//스테이지 타임 아웃
struct S2CStageTimeoutPacket {
	char type;
};
#pragma pack(pop)

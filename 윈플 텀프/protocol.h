#pragma once
#include<wtypes.h>

#define S2CLoading 0
#define S2CChangeStage 1
#define C2SSelectRole 2
#define S2CSelectRole 3
#define S2CChangeRole 4
#define C2SMove 5
#define S2CMove 6
#define C2SExitGame 7
#define S2CExitGame 8
#define C2SRetry 9
#define S2CResetStage 10
#define S2CJewelryVisibility 11
#define S2CDoorOpen 12

#pragma pack(push, 1)

struct S2CLoadingPacket { // 다른 클라이언트 기다리기위한 패킷
	unsigned char size;
	char type;
	char id; // 클라이언트 고유 id 전송
};

struct S2CChangeStagePacket { // 스테이지를 변경하기 위한 패킷
	unsigned char size;
	char type;
	short stageNum; // 스테이지 구분을 위한 정보
};

struct C2SSelectRolePacket { // 케릭터 선택시 클라이언트 => 서버 로 역할 전송
	unsigned char size;
	char type;
	//char id; // 어떤 클라이언트인지 구분 => 서버에서는 socket만 보고도 알 수 있나??
	char role; // 역할 정보
};

struct S2CSelectRolePacket { // 서버 => 클라이언트 한테 역할을 전송
	unsigned char size;
	char type;
	char id; // 어떤 클라이언트인지 구분을 위한 정보
	char role; // 역할 정보
};

struct C2SChangeRolePacket { // 선택이 아닌 케릭터 뭘 보고 있는지 다른 클라이언트도 알아야 하니까 있는 패킷
	unsigned char size;
	char type;
	char role; //역할 정보
};

struct S2CChangeRolePacket { // 다른 클라이언트가 어떤 역할을 보고 있는지 알려주기 위한 패킷
	unsigned char size;
	char type;
	char id; // 클라이언트 구분을 위한 패킷
	char role; // 역할 정보
};

struct C2SMovePacket { // 움직임 동작 패킷
	unsigned char size;
	char type;
	char id; // 클라이언트 구분 패킷
	float x, y; // 현재 위치
};

struct S2CMovePacket { //움직임 동작 패킷
	unsigned char size;
	char type;
	char id; // 클라이언트 구분 패킷
	float x, y; // 현재 위치
};

struct C2SExitGamepacket { // 클라이언트가 게임 종료를 원할 때, 전송
	unsigned char size;
	char type;	
};

struct S2CExitGamepacket { // 서버에서 다른 클라이언트에게 정보 알림
	unsigned char size;
	char type;
};

struct C2SRetryPacket { // 클라이언트가 서버에게 이번 스테이지 재시도 요청 패킷
	unsigned char size;
	char type;
};

struct S2CResetStagePacket { // 클라이언트에게 스테이지 새로 세팅하라는 패킷
	unsigned char size;
	char type;
	short stageNum;
};

struct S2CJewelryVisibilityPacket { // 서버에서 보석을 먹었다고 판단 후, 클라이언트에게 정보를 전달
	unsigned char size;
	char type;
	char jewelryType; // 어떤 종류의 보석인지
	int index; // 몇번째 인덱스에 저장된 보석인지
};

struct S2CDoorOpenPacket { // 문열리는걸 알리는 패킷
	unsigned char size;
	char type;
	char RoleType;// 어떤 타입의 문이 열릴지	
};

#pragma pack(pop)
 
#pragma once
#include<wtypes.h>

#define S2CLoading				0
#define S2CChangeStage			1

#define C2SSelectRole			3	
#define S2CSelectRole			4	
#define C2SChangRole			5	
#define S2CChangeRole			6	

#define C2SMove					7

#define S2CDoorVisible			12	
#define S2CAddPlayer			13	

#define S2CStageTimePass		14	
#define S2CStageTimeout			15	

#define S2CMove_IDLE			16
#define S2CMove_JUMP			17
#define S2CMove_LEFT			18
#define S2CMove_RIGHT			19

#define S2CEatJewely			20	
#define S2CIntoDoor				21	

#define C2SEndout				22
#define S2CEndout				23
#define S2CPlayerOut			24

#define PORT_NUM				9000

#pragma pack(push, 1)

struct S2CPlayerPacket {
	char type;
	char id;
};

struct S2CChangeStagePacket { 
	char type;
	short stageNum;
};

struct C2SRolePacket {
	char type;
	char role;
};

struct S2CRolePacket {
	char type;
	char id;
	char role;
};

struct MovePacket {
	char type;
	char id;
	short x, y;
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
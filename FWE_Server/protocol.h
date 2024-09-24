// FBWG Packet information

#pragma once

#define PORT_NUM		9000
#define MAX_BUF_SIZE	1024

enum class PACKET_TYPE_S2C {
	Loading = 0,
	ChangeStage,
	SelectRole,
	ChangeRole,
	DoorVisible,
	AddPlayer,
	StageTimePass,
	StageTimeout,
	Move_IDLE,
	Move_JUMP,
	Move_LEFT,
	Move_RIGHT,
	EatJewely,
	IntoDoor,
	Endout,
	PlayerOut
};

enum class PACKET_TYPE_C2S {
	ChangRole = 0,
	SelectRole,
	Move,
	Endout
};


#pragma pack(push, 1)

struct S2CPlayerPacket {
	char type;
	char size;
	char id;
};

struct S2CChangeStagePacket { 
	char type;
	char size;
	int stageNum;
};

struct C2SRolePacket {
	char type;
	char size;
	char id;
	char role;
};

struct S2CRolePacket {
	char type;
	char size;
	char id;
	char role;
};

struct MovePacket {
	char type;
	char size;
	char id;
	int x, y;
};

struct typePacket {
	char type;
	char size;
};

struct S2CStageTimePassPacket {
	char type;
	char size;
	int timePassed;
};

#pragma pack(pop)

// FBWG Packet information

#define PORT_NUM				9000

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

struct typePacket {
	char type;
};

struct S2CStageTimePassPacket {
	char type;
	int timePassed;
};

#pragma pack(pop)

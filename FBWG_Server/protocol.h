// FBWG Packet information

#define PORT_NUM				9000

enum PACKET_TYPE_S2C {
	S2CLoading = 0,
	S2CChangeStage,
	S2CSelectRole,
	S2CChangeRole,
	S2CDoorVisible,
	S2CAddPlayer,
	S2CStageTimePass,
	S2CStageTimeout,
	S2CMove_IDLE,
	S2CMove_JUMP,
	S2CMove_LEFT,
	S2CMove_RIGHT,
	S2CEatJewely,
	S2CIntoDoor,
	S2CEndout,
	S2CPlayerOut
};

enum PACKET_TYPE_C2S {
	C2SChangRole = 0,
	C2SSelectRole,
	C2SMove,
	C2SEndout
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
	char type = S2CStageTimePass;
	int timePassed;
};

#pragma pack(pop)

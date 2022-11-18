#pragma once

// CS : 클라 > 서버
// SC : 서버 > 클라

// 클라이언트가 계정 생성/로그인 요청으로 서버에 보내는 패킷
// 계정 생성과 로그인은 PACKET_TYPE으로 구분

// 기본적으로 부모 구조체 속에 packet type 존재
typedef struct _tagPacketBase
{
	PACKET_TYPE type; // 패킷 타입
	short size; // 패킷 사이즈

}PACKET_BASE, *PPACKET_BASE;

typedef struct _tagPacketLogin : public _tagPacketBase
{
	char id[MAX_CHAR_LEN]; // 아이디
	char pw[MAX_CHAR_LEN]; // 비밀번호

	_tagPacketLogin()
	{
		type = PT_LOGIN;
	}
}CS_LoginPacket, CS_CreateAccountPacket, *PCS_LoginPacket, *PCS_CreateAccountPacket;

// 클라이언트가 캐릭터 접속시 클라이언트에게 보내주는 캐릭터 정보 패킷
typedef struct _tagPacketCharacterInfo : public _tagPacketBase
{
	char name[MAX_CHAR_LEN]; // 캐릭터 이름
	short level; // 캐릭터 레벨
	int exp; // 캐릭터 경험치
	int hp;
	int mp;
	short _str;
	short _dex;
	short _int;
	short _luk;
	short abilitypoint; // 투자하지 않은 ap
	short skillpoint; // 투자하지 않은 sp
	int meso;

	_tagPacketCharacterInfo()
	{
		type = PT_STATUS;
	}
}SC_StatusPacket, *PSC_StatusPacket;

// 스킬 정보 패킷
typedef struct _tagPacketCharacterSkillInfo : public _tagPacketBase
{
	char skillName[MAX_CHAR_LEN];
	short skillStepLevel; // 스킬 차수
	short skillLevel; // 스킬 레벨

	_tagPacketCharacterSkillInfo()
	{
		type = PT_SKILLINFO;
	}
}SC_SkillInfoPacket, *PSC_SkillInfoPacket;

// 아이템 정보 패킷
typedef struct _tagPacketCharacterItemInfo : public _tagPacketBase
{
	char itemName[MAX_CHAR_LEN]; // 아이템 이름
	ITEM_TYPE eItemType; // 아이템 종류
	short itemCount; // 아이템 개수
	short x; // 아이템 창 x좌표
	short y; // 아이템 창 y좌표

	_tagPacketCharacterItemInfo()
	{
		type = PT_ITEMINFO;
	}
}SC_ItemInfoPacket, *PSC_ItemInfoPacket;

// 장비 정보 패킷
typedef struct _tagPacketCharacterEquipInfo : public _tagPacketBase
{
	char name[MAX_CHAR_LEN]; // 캐릭터 이름
	char equipName[MAX_CHAR_LEN];
	EQUIP_TYPE eEquipType;

	_tagPacketCharacterEquipInfo()
	{
		type = PT_EQUIPINFO;
	}

}SC_EquipInfoPacket, *PSC_EquipInfoPacket, CS_AddEquip, *PCS_AddEquip, SC_AddEquip, *PSC_AddEquip;

typedef struct _tagPacketEraseEquip : public _tagPacketBase
{
	char name[MAX_CHAR_LEN]; // 캐릭터 이름
	EQUIP_TYPE eEquipType;

	_tagPacketEraseEquip()
	{
		type = PT_ERASEEQUIP;
	}

}CS_EraseEquip, *PCS_EraseEquip, SC_EraseEquip, *PSC_EraseEquip;

// 캐릭터의 움직임 처리 패킷
typedef struct _tagPacketMoveInfo : public _tagPacketBase
{
	POSITION tPos;
	MOVE_DIR eDir;
	OBJECT_STATE objState;

	_tagPacketMoveInfo()
	{
		type = PT_MOVE;
	}
}CS_MovePacket, *PCS_MovePacket;

// 오브젝트 패킷
typedef struct _tagPacketObject : public _tagPacketBase
{
	int sceneNum; // scene 고유번호
				  //char layerName[MAX_CHAR_LEN]; // object가 속한 layer이름
				  // 무조건 object layer의 정보만 전송할 것이므로 필요없다
	OBJECT_TYPE objType; // 오브젝트의 타입
	OBJECT_STATE objState; // 오브젝트 상태
	char name[MAX_CHAR_LEN]; // 오브젝트 이름
	POSITION tPos; // 위치
	MOVE_DIR eDir; // 오브젝트 방향
	POSITION tOrigin; // 오브젝트의 origin값 = 몬스터 소환 origin
					  // 다른 플레이어인 경우 (0,0) = default, 몬스터인 경우에 한하여 입력
	int hp;
	int mp;
	bool enable;
	bool life;

	// 아이템용 데이터
	int iNum; // 개수

	// 플레이어용 데이터
	int exp;
	char equipName[ET_END][MAX_CHAR_LEN];
	bool isSelf; // 자기 자신의 데이터인지 확인하는 플래그

	_tagPacketObject()
	{
		type = PT_OBJECT;
		life = true;
		isSelf = false;

		for (int i = 0; i < (int)ET_END; i++)
		{
			memset(equipName[i], 0, sizeof(equipName[i]));
		}
	}
}SC_ObjectPacket, *PSC_ObjectPacket;

typedef struct _tagPacketMessage : public _tagPacketBase
{
	char name[MAX_CHAR_LEN]; // 보낸 유저 이름
	char message[MAX_MESSAGE_LEN];

	_tagPacketMessage()
	{
		type = PT_MESSAGE;
	}
}PACKETMESSAGE, *PPACKETMESSAGE;

typedef struct _tagPacketLoginAccept : public _tagPacketBase
{
	bool accept;
	// 로그인 실패 시 아이디가 없으면 false 비밀번호 틀리면 true
	// createaccount 에서는 불필요, 사용 안해도됨
	bool idExist;

	_tagPacketLoginAccept()
	{
		type = PT_LOGIN;
	}
}SC_LoginPacket, SC_CreateAccountPacket, *PSC_LoginPacket, *PSC_CreateAccountPacket;

// 서버가 캐릭터 초기화시 또는
// 클라이언트가 scene 이동 시 보내주는 패킷
typedef struct _tagPacketSceneChange : public _tagPacketBase
{
	int sceneNum; // scene 번호
	POSITION tPos; // 위치

	_tagPacketSceneChange()
	{
		type = PT_POSITION;
	}
}SC_PositionPacket, *PSC_PositionPacket, CS_PositionPacekt, *PCS_PositionPacket;

// 새 캐릭터 입장시 클라에게 보내는 패킷
typedef struct _tagPacketEnterScene : public _tagPacketBase
{
	OBJECT_STATE objState;
	char name[MAX_CHAR_LEN];
	POSITION tPos;

	// 캐릭터가 착용한 장비
	char equipName[ET_END][MAX_CHAR_LEN];

	_tagPacketEnterScene()
	{
		type = PT_ENTER;

		for (int i = 0; i < (int)ET_END; i++)
		{
			memset(equipName[i], 0, sizeof(equipName[i]));
		}
	}
}SC_CharacterEnter, *PSC_CharacterEnter;

// 캐릭터 퇴장시 클라에게 보내는 패킷
typedef struct _tagPacketLeaveScene : public _tagPacketBase
{
	char name[MAX_CHAR_LEN];

	_tagPacketLeaveScene()
	{
		type = PT_LEAVE;
	}
}SC_CharacterLeave, *PSC_CharacterLeave;

typedef struct _tagPacketPortal : public _tagPacketBase
{
	int sceneNumFrom; // 포탈 위치한 scene(이전 scene)
	int sceneNumTo; // 포탈이 향하는 scene(다음 scene)

	_tagPacketPortal()
	{
		type = PT_PORTAL;
	}
}CS_Portal, *PCS_Portal;

typedef struct _tagPacketUseSkill : public _tagPacketBase
{
	int sceneNum; // scene 번호
	char name[MAX_CHAR_LEN]; // 유저 이름
	int skillstep; // 스킬 차수
	char skillname[MAX_CHAR_LEN]; // 사용 스킬 이름

	_tagPacketUseSkill()
	{
		type = PT_SKILL;
	}
}CS_UseSkill, SC_UseSkill, *PCS_UseSkill, *PSC_UseSkill;

typedef struct _tagPacketSkillLevel : public _tagPacketBase
{
	char name[MAX_CHAR_LEN]; // 스킬 이름
	int skillLevel; // 스킬 레벨

	_tagPacketSkillLevel()
	{
		type = PT_CHANGESKILL;
	}
}CS_ChangeSkill, *PCS_ChangeSkill;

typedef struct _tagPacketChangeItem : public _tagPacketBase
{
	char name[MAX_CHAR_LEN];
	ITEM_TYPE eType; // 아이템 종류
	int iNum; // 아이템 개수
	POSITION tPos; // 아이템 좌표
	bool erased; // 아이템이 사라졌다면 true, 기본값 false

	_tagPacketChangeItem()
	{
		type = PT_CHANGEITEM;
		erased = false;
	}
}CS_ChangeItem, *PCS_ChangeItem;

typedef struct _tagPacketThrowItem : public _tagPacketBase
{
	char name[MAX_CHAR_LEN]; // 아이템 이름
	int iNum; // 아이템 개수
	POSITION tPos; // 버린 위치(플레이어 위치)
	ITEM_TYPE eType; // 아이템 타입

					 // 패킷 전송한 플레이어에게서 이름으로 검색한 아이템을 개수만큼 scene에 전달
					 // 플레이어에게서는 해당 개수만큼 제거

	_tagPacketThrowItem()
	{
		type = PT_THROWITEM;
	}
}CS_ThrowItem, *PCS_ThrowItem;

typedef struct _tagPacketGetItem : public _tagPacketBase
{
	char name[MAX_CHAR_LEN];
	ITEM_TYPE eType; // 아이템 타입
	POSITION tPos; // 얻으려는 아이템 위치
	POSITION tCoord; // 아이템 창에 들어갈 좌표

	_tagPacketGetItem()
	{
		type = PT_GETITEM;
	}
}CS_GetItem, *PCS_GetItem, SC_GetItem, *PSC_GetItem;

// 몬스터에게 데미지를 주었을때 전달하는 패킷
typedef struct _tagPacketDamage : public _tagPacketBase
{
	int sceneNum; // 몬스터가 속한 scene
	POSITION tOrigin; // 몬스터 origin
	int iDamage; // 데미지 수치

	_tagPacketDamage()
	{
		type = PT_DAMAGE;
	}
}CS_Damage, *PCS_Damage;

typedef struct _tagPacketLevelUp : public _tagPacketBase
{
	bool levelup; // 레벨업 패킷, true

	_tagPacketLevelUp()
	{
		type = PT_LEVELUP;
		levelup = true;
	}
}SC_LevelUp, *PSC_LevelUp;

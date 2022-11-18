#pragma once

// CS : Ŭ�� > ����
// SC : ���� > Ŭ��

// Ŭ���̾�Ʈ�� ���� ����/�α��� ��û���� ������ ������ ��Ŷ
// ���� ������ �α����� PACKET_TYPE���� ����

// �⺻������ �θ� ����ü �ӿ� packet type ����
typedef struct _tagPacketBase
{
	PACKET_TYPE type; // ��Ŷ Ÿ��
	short size; // ��Ŷ ������

}PACKET_BASE, *PPACKET_BASE;

typedef struct _tagPacketLogin : public _tagPacketBase
{
	char id[MAX_CHAR_LEN]; // ���̵�
	char pw[MAX_CHAR_LEN]; // ��й�ȣ

	_tagPacketLogin()
	{
		type = PT_LOGIN;
	}
}CS_LoginPacket, CS_CreateAccountPacket, *PCS_LoginPacket, *PCS_CreateAccountPacket;

// Ŭ���̾�Ʈ�� ĳ���� ���ӽ� Ŭ���̾�Ʈ���� �����ִ� ĳ���� ���� ��Ŷ
typedef struct _tagPacketCharacterInfo : public _tagPacketBase
{
	char name[MAX_CHAR_LEN]; // ĳ���� �̸�
	short level; // ĳ���� ����
	int exp; // ĳ���� ����ġ
	int hp;
	int mp;
	short _str;
	short _dex;
	short _int;
	short _luk;
	short abilitypoint; // �������� ���� ap
	short skillpoint; // �������� ���� sp
	int meso;

	_tagPacketCharacterInfo()
	{
		type = PT_STATUS;
	}
}SC_StatusPacket, *PSC_StatusPacket;

// ��ų ���� ��Ŷ
typedef struct _tagPacketCharacterSkillInfo : public _tagPacketBase
{
	char skillName[MAX_CHAR_LEN];
	short skillStepLevel; // ��ų ����
	short skillLevel; // ��ų ����

	_tagPacketCharacterSkillInfo()
	{
		type = PT_SKILLINFO;
	}
}SC_SkillInfoPacket, *PSC_SkillInfoPacket;

// ������ ���� ��Ŷ
typedef struct _tagPacketCharacterItemInfo : public _tagPacketBase
{
	char itemName[MAX_CHAR_LEN]; // ������ �̸�
	ITEM_TYPE eItemType; // ������ ����
	short itemCount; // ������ ����
	short x; // ������ â x��ǥ
	short y; // ������ â y��ǥ

	_tagPacketCharacterItemInfo()
	{
		type = PT_ITEMINFO;
	}
}SC_ItemInfoPacket, *PSC_ItemInfoPacket;

// ��� ���� ��Ŷ
typedef struct _tagPacketCharacterEquipInfo : public _tagPacketBase
{
	char name[MAX_CHAR_LEN]; // ĳ���� �̸�
	char equipName[MAX_CHAR_LEN];
	EQUIP_TYPE eEquipType;

	_tagPacketCharacterEquipInfo()
	{
		type = PT_EQUIPINFO;
	}

}SC_EquipInfoPacket, *PSC_EquipInfoPacket, CS_AddEquip, *PCS_AddEquip, SC_AddEquip, *PSC_AddEquip;

typedef struct _tagPacketEraseEquip : public _tagPacketBase
{
	char name[MAX_CHAR_LEN]; // ĳ���� �̸�
	EQUIP_TYPE eEquipType;

	_tagPacketEraseEquip()
	{
		type = PT_ERASEEQUIP;
	}

}CS_EraseEquip, *PCS_EraseEquip, SC_EraseEquip, *PSC_EraseEquip;

// ĳ������ ������ ó�� ��Ŷ
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

// ������Ʈ ��Ŷ
typedef struct _tagPacketObject : public _tagPacketBase
{
	int sceneNum; // scene ������ȣ
				  //char layerName[MAX_CHAR_LEN]; // object�� ���� layer�̸�
				  // ������ object layer�� ������ ������ ���̹Ƿ� �ʿ����
	OBJECT_TYPE objType; // ������Ʈ�� Ÿ��
	OBJECT_STATE objState; // ������Ʈ ����
	char name[MAX_CHAR_LEN]; // ������Ʈ �̸�
	POSITION tPos; // ��ġ
	MOVE_DIR eDir; // ������Ʈ ����
	POSITION tOrigin; // ������Ʈ�� origin�� = ���� ��ȯ origin
					  // �ٸ� �÷��̾��� ��� (0,0) = default, ������ ��쿡 ���Ͽ� �Է�
	int hp;
	int mp;
	bool enable;
	bool life;

	// �����ۿ� ������
	int iNum; // ����

	// �÷��̾�� ������
	int exp;
	char equipName[ET_END][MAX_CHAR_LEN];
	bool isSelf; // �ڱ� �ڽ��� ���������� Ȯ���ϴ� �÷���

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
	char name[MAX_CHAR_LEN]; // ���� ���� �̸�
	char message[MAX_MESSAGE_LEN];

	_tagPacketMessage()
	{
		type = PT_MESSAGE;
	}
}PACKETMESSAGE, *PPACKETMESSAGE;

typedef struct _tagPacketLoginAccept : public _tagPacketBase
{
	bool accept;
	// �α��� ���� �� ���̵� ������ false ��й�ȣ Ʋ���� true
	// createaccount ������ ���ʿ�, ��� ���ص���
	bool idExist;

	_tagPacketLoginAccept()
	{
		type = PT_LOGIN;
	}
}SC_LoginPacket, SC_CreateAccountPacket, *PSC_LoginPacket, *PSC_CreateAccountPacket;

// ������ ĳ���� �ʱ�ȭ�� �Ǵ�
// Ŭ���̾�Ʈ�� scene �̵� �� �����ִ� ��Ŷ
typedef struct _tagPacketSceneChange : public _tagPacketBase
{
	int sceneNum; // scene ��ȣ
	POSITION tPos; // ��ġ

	_tagPacketSceneChange()
	{
		type = PT_POSITION;
	}
}SC_PositionPacket, *PSC_PositionPacket, CS_PositionPacekt, *PCS_PositionPacket;

// �� ĳ���� ����� Ŭ�󿡰� ������ ��Ŷ
typedef struct _tagPacketEnterScene : public _tagPacketBase
{
	OBJECT_STATE objState;
	char name[MAX_CHAR_LEN];
	POSITION tPos;

	// ĳ���Ͱ� ������ ���
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

// ĳ���� ����� Ŭ�󿡰� ������ ��Ŷ
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
	int sceneNumFrom; // ��Ż ��ġ�� scene(���� scene)
	int sceneNumTo; // ��Ż�� ���ϴ� scene(���� scene)

	_tagPacketPortal()
	{
		type = PT_PORTAL;
	}
}CS_Portal, *PCS_Portal;

typedef struct _tagPacketUseSkill : public _tagPacketBase
{
	int sceneNum; // scene ��ȣ
	char name[MAX_CHAR_LEN]; // ���� �̸�
	int skillstep; // ��ų ����
	char skillname[MAX_CHAR_LEN]; // ��� ��ų �̸�

	_tagPacketUseSkill()
	{
		type = PT_SKILL;
	}
}CS_UseSkill, SC_UseSkill, *PCS_UseSkill, *PSC_UseSkill;

typedef struct _tagPacketSkillLevel : public _tagPacketBase
{
	char name[MAX_CHAR_LEN]; // ��ų �̸�
	int skillLevel; // ��ų ����

	_tagPacketSkillLevel()
	{
		type = PT_CHANGESKILL;
	}
}CS_ChangeSkill, *PCS_ChangeSkill;

typedef struct _tagPacketChangeItem : public _tagPacketBase
{
	char name[MAX_CHAR_LEN];
	ITEM_TYPE eType; // ������ ����
	int iNum; // ������ ����
	POSITION tPos; // ������ ��ǥ
	bool erased; // �������� ������ٸ� true, �⺻�� false

	_tagPacketChangeItem()
	{
		type = PT_CHANGEITEM;
		erased = false;
	}
}CS_ChangeItem, *PCS_ChangeItem;

typedef struct _tagPacketThrowItem : public _tagPacketBase
{
	char name[MAX_CHAR_LEN]; // ������ �̸�
	int iNum; // ������ ����
	POSITION tPos; // ���� ��ġ(�÷��̾� ��ġ)
	ITEM_TYPE eType; // ������ Ÿ��

					 // ��Ŷ ������ �÷��̾�Լ� �̸����� �˻��� �������� ������ŭ scene�� ����
					 // �÷��̾�Լ��� �ش� ������ŭ ����

	_tagPacketThrowItem()
	{
		type = PT_THROWITEM;
	}
}CS_ThrowItem, *PCS_ThrowItem;

typedef struct _tagPacketGetItem : public _tagPacketBase
{
	char name[MAX_CHAR_LEN];
	ITEM_TYPE eType; // ������ Ÿ��
	POSITION tPos; // �������� ������ ��ġ
	POSITION tCoord; // ������ â�� �� ��ǥ

	_tagPacketGetItem()
	{
		type = PT_GETITEM;
	}
}CS_GetItem, *PCS_GetItem, SC_GetItem, *PSC_GetItem;

// ���Ϳ��� �������� �־����� �����ϴ� ��Ŷ
typedef struct _tagPacketDamage : public _tagPacketBase
{
	int sceneNum; // ���Ͱ� ���� scene
	POSITION tOrigin; // ���� origin
	int iDamage; // ������ ��ġ

	_tagPacketDamage()
	{
		type = PT_DAMAGE;
	}
}CS_Damage, *PCS_Damage;

typedef struct _tagPacketLevelUp : public _tagPacketBase
{
	bool levelup; // ������ ��Ŷ, true

	_tagPacketLevelUp()
	{
		type = PT_LEVELUP;
		levelup = true;
	}
}SC_LevelUp, *PSC_LevelUp;

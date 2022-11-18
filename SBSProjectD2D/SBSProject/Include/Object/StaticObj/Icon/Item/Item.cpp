#include "Item.h"
#include "../../../../Resources/ResourcesManager.h"
#include "../../../../Collider/ColliderRect.h"
#include "../../../../ItemManager/ItemManager.h"
#include "EquipParts.h"

CItem::CItem() :
	m_iNum(1),
	m_eType(IT_END),
	m_iPrice(1),
	m_pObj(NULL),
	m_bIsGot(false),
	m_eEquipType(ET_DEFAULT)
{
}

CItem::CItem(const CItem& item) :
	CIcon(item)
{
	m_iNum = item.m_iNum;
	m_eType = item.m_eType;
	m_iPrice = item.m_iPrice;

	//m_iRef = 1;
	m_bIsGot = false;

	m_pObj = NULL; // 이건 필요하면 복사하는 곳에서 set해주기

	m_eEquipType = item.m_eEquipType;

	list<CEquipParts*>::const_iterator iter;
	list<CEquipParts*>::const_iterator iterEnd = item.m_PartsList.end();

	for (iter = item.m_PartsList.begin(); iter != iterEnd; iter++)
	{
		m_PartsList.push_back((*iter)->Clone());
	}
}

CItem::~CItem()
{
	Safe_Release_VecList(m_PartsList);
}

CEquipParts* CItem::CreateParts(const string & strTag, EQUIP_TYPE eType)
{
	CEquipParts* pParts = new CEquipParts;

	pParts->SetTag(strTag);

	if (!pParts->Init())
		return false;

	SetEquipType(eType);

	m_PartsList.push_back(pParts);

	pParts->AddRef();
	return pParts;
}

bool CItem::Init()
{
	// 아이템의 포지션은 인벤토리 칸의 좌상단
	// 수정했음 (0, 0)으로
	SetIconPivot(0.f, 0.f);
	SetIconPos(0.f, 0.f);
	SetIconSize(32.f, 32.f);
	SetIconType(IC_ITEM);

	m_iNum = 1;

	SetSize(32.f, 32.f);
	SetPivot(0.5f, 1.f);
	SetObjType(OT_ITEM);

	SetItemType(GET_SINGLE(CItemManager)->FindItemType(m_strTag));

	CColliderRect* pRC = AddCollider<CColliderRect>("ItemBody");
	pRC->SetRect(-16.f, -32.f, 16.f, 0.f);
	SAFE_RELEASE(pRC);

	return true;
}

void CItem::Input(float fDeltaTime)
{
	CIcon::Input(fDeltaTime);
}

int CItem::Update(float fDeltaTime)
{
	CIcon::Update(fDeltaTime);

	if (m_eType == IT_EQUIP)
	{
		list<CEquipParts*>::iterator iter;
		list<CEquipParts*>::iterator iterEnd = m_PartsList.end();

		for (iter = m_PartsList.begin(); iter != iterEnd; iter++)
		{
			(*iter)->Update(fDeltaTime);
			if (m_pObj)
			{
				(*iter)->SetPos(m_pObj->GetPos());
				(*iter)->SetDir(m_pObj->GetDir());
				(*iter)->SetObjState(m_pObj->GetObjState());
			}
		}
	}

	return 0;
}

int CItem::LateUpdate(float fDeltaTime)
{
	CIcon::LateUpdate(fDeltaTime);
	return 0;
}

void CItem::Collision(float fDeltaTime)
{
	CIcon::Collision(fDeltaTime);
}

void CItem::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CIcon::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	if (m_eType == IT_EQUIP)
	{
		list<CEquipParts*>::iterator iter;
		list<CEquipParts*>::iterator iterEnd = m_PartsList.end();

		for (iter = m_PartsList.begin(); iter != iterEnd; iter++)
		{
			(*iter)->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
		}
	}
}

CItem* CItem::Clone()
{
	return new CItem(*this);
}

void CItem::UseItem()
{
	if (m_eType == IT_USABLE && m_iNum == 1)
		Die();
}

void CItem::ItemInit()
{
	m_eType = GET_SINGLE(CItemManager)->FindItemType(m_strTag);
	LoadItemIcon(m_strTag);

	if (m_eType == IT_EQUIP)
	{
		if (m_strTag == "BrownGot")
		{
			// part init
			CEquipParts* pParts = CreateParts("Head", ET_HEAD);

			CAnimation* pAni = pParts->CreateAnimation("BrownGotHeadAnimation");
			POSITION arrOrigin1[1] = { POSITION(32.f, 69.f) };
			POSITION arrLT1[1] = { POSITION(-32.f, -69.f) };
			POSITION arrRB1[1] = { POSITION(32.f, -35.f) };
			float arrDelay1[1] = { 1.f };
			pAni->AddClip("Idle", AO_LOOP, 1, arrOrigin1, arrLT1, arrRB1, arrDelay1, "BrownGotStand", L"Equip/Head/Stand/");

			POSITION arrOrigin2[2] = { POSITION(32.f, 69.f), POSITION(32.f, 68.f) };
			POSITION arrLT2[2] = { POSITION(-32.f, -69.f), POSITION(-32.f, -68.f) };
			POSITION arrRB2[2] = { POSITION(32.f, -35.f), POSITION(32.f, -34.f) };
			float arrDelay2[2] = { 0.18f, 0.18f };
			pAni->AddClip("Run", AO_LOOP, 2, arrOrigin2, arrLT2, arrRB2, arrDelay2, "BrownGotRun", L"Equip/Head/Stand/");

			POSITION arrOrigin3[1] = { POSITION(34.f, 68.f) };
			POSITION arrLT3[1] = { POSITION(-34.f, -68.f) };
			POSITION arrRB3[1] = { POSITION(30.f, -34.f) };
			float arrDelay3[1] = { 1.f };
			pAni->AddClip("Jump", AO_LOOP, 1, arrOrigin3, arrLT3, arrRB3, arrDelay3, "BrownGotJump", L"Equip/Head/Stand/");

			POSITION arrOrigin4[3] = { POSITION(53.f, 64.f), POSITION(55.f, 68.f), POSITION(82.f, 64.f) };
			POSITION arrLT4[3] = { POSITION(-53.f, -64.f), POSITION(-55.f, -68.f), POSITION(-82.f, -64.f) };
			POSITION arrRB4[3] = { POSITION(11.f, -30.f), POSITION(9.f, -34.f), POSITION(-18.f, -30.f) };
			float arrDelay4[3] = { 0.3f, 0.15f, 0.35f };
			pAni->AddClip("Attack", AO_ONCE_RETURN, 3, arrOrigin4, arrLT4, arrRB4, arrDelay4, "BrownGotAttack", L"Equip/Head/Stand/");

			POSITION arrOrigin5[1] = { POSITION(37.f, 42.f) };
			POSITION arrLT5[1] = { POSITION(-37.f, -42.f) };
			POSITION arrRB5[1] = { POSITION(27.f, -8.f) };
			float arrDelay5[1] = { 1.f };
			pAni->AddClip("Prone", AO_LOOP, 1, arrOrigin5, arrLT5, arrRB5, arrDelay5, "BrownGotProne", L"Equip/Head/Stand/");

			// TODO
			POSITION arrOrigin6[1] = { POSITION(31.f, 65.f) };
			POSITION arrLT6[1] = { POSITION(-31.f, -65.f) };
			POSITION arrRB6[1] = { POSITION(31.f, -41.f) };
			float arrDelay6[1] = { 1.f };
			pAni->AddClip("Rope1", AO_LOOP, 1, arrOrigin6, arrLT6, arrRB6, arrDelay6, "BrownGotRope1", L"Equip/Head/Rope/");
			pAni->AddClip("Rope2", AO_LOOP, 1, arrOrigin6, arrLT6, arrRB6, arrDelay6, "BrownGotRope2", L"Equip/Head/Rope/");
			pAni->AddClip("Ladder1", AO_LOOP, 1, arrOrigin6, arrLT6, arrRB6, arrDelay6, "BrownGotLadder1", L"Equip/Head/Rope/");
			pAni->AddClip("Ladder2", AO_LOOP, 1, arrOrigin6, arrLT6, arrRB6, arrDelay6, "BrownGotLadder2", L"Equip/Head/Rope/");

			SAFE_RELEASE(pAni);
			SAFE_RELEASE(pParts);
		}
		else if (m_strTag == "BlueGown")
		{
			// body
			CEquipParts* pParts = CreateParts("Body", ET_CLOTH);
			CAnimation* pAni = pParts->CreateAnimation("BlueGownBodyAnimation");

			POSITION arrOrigin[4] = { POSITION(7.f, 20.f), POSITION(8.f, 20.f), POSITION(8.f, 20.f), POSITION(8.f, 20.f) };
			POSITION arrLT[4] = { POSITION(-7.f, -20.f), POSITION(-8.f, -20.f), POSITION(-8.f, -20.f), POSITION(-8.f, -20.f) };
			POSITION arrRB[4] = { POSITION(7.f, 0.f), POSITION(8.f, 0.f), POSITION(9.f, 0.f), POSITION(8.f, 0.f) };
			float arrDelay[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
			pAni->AddClip("Idle", AO_LOOP, 4, arrOrigin, arrLT, arrRB, arrDelay, "BlueGownBodyStand", L"Equip/Cloth/Stand/Body/");

			POSITION arrOrigin2[4] = { POSITION(8.f, 20.f), POSITION(8.f, 19.f), POSITION(8.f, 20.f), POSITION(11.f, 19.f) };
			POSITION arrLT2[4] = { POSITION(-8.f, -20.f), POSITION(-8.f, -19.f), POSITION(-8.f, -20.f), POSITION(-11.f, -19.f) };
			POSITION arrRB2[4] = { POSITION(9.f, 0.f), POSITION(8.f, 0.f), POSITION(9.f, 0.f), POSITION(11.f, 0.f) };
			float arrDelay2[4] = { 0.18f, 0.18f, 0.18f, 0.18f };
			pAni->AddClip("Run", AO_LOOP, 4, arrOrigin2, arrLT2, arrRB2, arrDelay2, "BlueGownBodyRun", L"Equip/Cloth/Run/Body/");

			POSITION arrOrigin3[1] = { POSITION(9.f, 19.f) };
			POSITION arrLT3[1] = { POSITION(-9.f, -19.f) };
			POSITION arrRB3[1] = { POSITION(9.f, 0.f) };
			float arrDelay3[1] = { 0.1f };
			pAni->AddClip("Jump", AO_LOOP, 1, arrOrigin3, arrLT3, arrRB3, arrDelay3, "BlueGownBodyJump", L"Equip/Cloth/Jump/Body/");

			// 엎드리기
			POSITION arrOrigin4[1] = { POSITION(22.f, 12.f) };
			POSITION arrLT4[1] = { POSITION(-22.f, -12.f) };
			POSITION arrRB4[1] = { POSITION(22.f, 0.f) };
			float arrDelay4[1] = { 0.1f };
			pAni->AddClip("Prone", AO_LOOP, 1, arrOrigin4, arrLT4, arrRB4, arrDelay4, "BlueGownBodyProne", L"Equip/Cloth/Prone/Body/");

			POSITION arrOrigin5[1] = { POSITION(10.f, 19.f) };
			POSITION arrLT5[1] = { POSITION(-10.f, -19.f) };
			POSITION arrRB5[1] = { POSITION(10.f, 0.f) };
			float arrDelay5[1] = { 0.1f };
			pAni->AddClip("Rope1", AO_LOOP, 1, arrOrigin5, arrLT5, arrRB5, arrDelay5, "BlueGownBodyRope1", L"Equip/Cloth/Rope/Body/Rope1/");

			POSITION arrOrigin6[1] = { POSITION(9.f, 19.f) };
			POSITION arrLT6[1] = { POSITION(-9.f, -19.f) };
			POSITION arrRB6[1] = { POSITION(10.f, 0.f) };
			float arrDelay6[1] = { 0.1f };
			pAni->AddClip("Rope2", AO_LOOP, 1, arrOrigin6, arrLT6, arrRB6, arrDelay6, "BlueGownBodyRope2", L"Equip/Cloth/Rope/Body/Rope2/");

			POSITION arrOrigin7[1] = { POSITION(11.f, 19.f) };
			POSITION arrLT7[1] = { POSITION(-11.f, -19.f) };
			POSITION arrRB7[1] = { POSITION(11.f, 0.f) };
			float arrDelay7[1] = { 0.1f };
			pAni->AddClip("Ladder1", AO_LOOP, 1, arrOrigin7, arrLT7, arrRB7, arrDelay7, "BlueGownBodyLadder1", L"Equip/Cloth/Ladder/Body/Ladder1/");
			pAni->AddClip("Ladder2", AO_LOOP, 1, arrOrigin7, arrLT7, arrRB7, arrDelay7, "BlueGownBodyLadder2", L"Equip/Cloth/Ladder/Body/Ladder2/");

			// swing attack
			POSITION arrOrigin8[3] = { POSITION(9.f, 19.f), POSITION(9.f, 20.f), POSITION(11.f, 17.f) };
			POSITION arrLT8[3] = { POSITION(-9.f, -19.f), POSITION(-9.f, -20.f), POSITION(-11.f, -17.f) };
			POSITION arrRB8[3] = { POSITION(9.f, 0.f), POSITION(9.f, 0.f), POSITION(12.f, 0.f) };
			float arrDelay8[3] = { 0.3f, 0.15f, 0.35f };
			pAni->AddClip("Attack", AO_ONCE_RETURN, 3, arrOrigin8, arrLT8, arrRB8, arrDelay8, "BlueGownBodyAttack", L"Equip/Cloth/Attack/Body/");

			SAFE_RELEASE(pAni);
			SAFE_RELEASE(pParts);

			// arm
			pParts = CreateParts("Arm", ET_CLOTH);
			pAni = pParts->CreateAnimation("BlueGownArmAnimation");

			POSITION arrOrigin9[4] = { POSITION(4.f, 11.f), POSITION(4.f, 12.f), POSITION(5.f, 11.f), POSITION(4.f, 12.f) };
			POSITION arrLT9[4] = { POSITION(-4.f, -11.f), POSITION(-4.f, -12.f), POSITION(-5.f, -11.f), POSITION(-4.f, -12.f) };
			POSITION arrRB9[4] = { POSITION(4.f, 0.f), POSITION(4.f, 0.f), POSITION(5.f, 0.f), POSITION(4.f, 0.f) };
			float arrDelay9[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
			pAni->AddClip("Idle", AO_LOOP, 4, arrOrigin9, arrLT9, arrRB9, arrDelay9, "BlueGownArmStand", L"Equip/Cloth/Stand/Arm/");

			POSITION arrOrigin10[4] = { POSITION(5.f, 9.f), POSITION(3.f, 15.f), POSITION(5.f, 9.f), POSITION(6.f, 8.f) };
			POSITION arrLT10[4] = { POSITION(-5.f, -9.f), POSITION(-3.f, -15.f), POSITION(-8.f, -20.f), POSITION(-6.f, -8.f) };
			POSITION arrRB10[4] = { POSITION(6.f, 0.f), POSITION(4.f, 0.f), POSITION(6.f, 0.f), POSITION(7.f, 0.f) };
			float arrDelay10[4] = { 0.18f, 0.18f, 0.18f, 0.18f };
			pAni->AddClip("Run", AO_LOOP, 4, arrOrigin10, arrLT10, arrRB10, arrDelay10, "BlueGownBodyRun", L"Equip/Cloth/Run/Arm/");

			POSITION arrOrigin11[1] = { POSITION(3.f, 8.f) };
			POSITION arrLT11[1] = { POSITION(-3.f, -8.f) };
			POSITION arrRB11[1] = { POSITION(3.f, 0.f) };
			float arrDelay11[1] = { 0.1f };
			pAni->AddClip("Jump", AO_LOOP, 1, arrOrigin11, arrLT11, arrRB11, arrDelay11, "BlueGownBodyJump", L"Equip/Cloth/Jump/Arm/");

			// 엎드리기
			POSITION arrOrigin12[1] = { POSITION(5.f, 6.f) };
			POSITION arrLT12[1] = { POSITION(-5.f, -6.f) };
			POSITION arrRB12[1] = { POSITION(5.f, 0.f) };
			float arrDelay12[1] = { 0.1f };
			pAni->AddClip("Prone", AO_LOOP, 1, arrOrigin12, arrLT12, arrRB12, arrDelay12, "BlueGownBodyProne", L"Equip/Cloth/Prone/Arm/");

			POSITION arrOrigin13[1] = { POSITION(0.f, 0.f) };
			POSITION arrLT13[1] = { POSITION(-0.f, -0.f) };
			POSITION arrRB13[1] = { POSITION(1.f, 1.f) };
			float arrDelay13[1] = { 0.1f };
			pAni->AddClip("Rope1", AO_LOOP, 1, arrOrigin13, arrLT13, arrRB13, arrDelay13, "BlueGownBodyRope1", L"Equip/Cloth/Rope/Arm/");
			pAni->AddClip("Rope2", AO_LOOP, 1, arrOrigin13, arrLT13, arrRB13, arrDelay13, "BlueGownBodyRope2", L"Equip/Cloth/Rope/Arm/");
			pAni->AddClip("Ladder1", AO_LOOP, 1, arrOrigin13, arrLT13, arrRB13, arrDelay13, "BlueGownBodyLadder1", L"Equip/Cloth/Ladder/Arm/");
			pAni->AddClip("Ladder2", AO_LOOP, 1, arrOrigin13, arrLT13, arrRB13, arrDelay13, "BlueGownBodyLadder2", L"Equip/Cloth/Ladder/Arm/");

			// swing attack
			POSITION arrOrigin14[3] = { POSITION(4.f, 8.f), POSITION(5.f, 8.f), POSITION(7.f, 14.f) };
			POSITION arrLT14[3] = { POSITION(-4.f, -8.f), POSITION(-5.f, -8.f), POSITION(-7.f, -14.f) };
			POSITION arrRB14[3] = { POSITION(4.f, 0.f), POSITION(6.f, 0.f), POSITION(7.f, 0.f) };
			float arrDelay14[3] = { 0.3f, 0.15f, 0.35f };
			pAni->AddClip("Attack", AO_ONCE_RETURN, 3, arrOrigin14, arrLT14, arrRB14, arrDelay14, "BlueGownBodyAttack", L"Equip/Cloth/Attack/Arm/");

			SAFE_RELEASE(pAni);
			SAFE_RELEASE(pParts);
		}
		//else
			//printf("ItemInit error(Wrong item tag)\n");
	}
}
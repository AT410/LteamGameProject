#include"stdafx.h"
#include"Project.h"

namespace basecross {
	UpDownBox::UpDownBox(const shared_ptr<Stage>& Stageptr):
		ObjectBase(Stageptr),
		m_Speed(1.0f),
		m_OldPos(0.0f)
	{}

	void UpDownBox::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(7.0f, 0.0f, -5.0f);
		ptrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTransform->SetScale(1.0f, 1.0f, 1.0f);
		auto ptrPos = ptrTransform->GetPosition();
		auto ptrColl = AddComponent<CollisionObb>();
		auto ptrDraw = AddComponent<PCTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_OldPos = ptrTransform->GetPosition();
		m_CurrentPos = ptrTransform->GetPosition();
	}

	void UpDownBox::BoxJudgment() {
		auto Elapsedtime = App::GetApp()->GetElapsedTime();
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto ptrPlayerScale = ptrPlayer->GetComponent<Transform>()->GetScale();
		auto ptrPlayerPos = ptrPlayer->GetComponent<Transform>()->GetPosition();
		auto ptrTransform = GetComponent<Transform>();
		auto ptrScale = ptrTransform->GetScale();
		auto ptrPos = ptrTransform->GetPosition();
		m_PlayerAABBX = ptrPlayerScale.x / 2;
		m_PlayerAABBY = ptrPlayerScale.y / 2;
		m_PlayerAABBZ = ptrPlayerScale.z / 2;
		m_BoxAABBX = ptrScale.x / 2;
		m_BoxAABBY = ptrScale.y / 2;
		m_BoxAABBZ = ptrScale.z / 2;
		m_BoxAABB = AABB(Vec3(ptrPos.x - m_BoxAABBX, ptrPos.y, ptrPos.z - m_BoxAABBZ),
			Vec3(ptrPos.x + m_BoxAABBX, ptrPos.y + m_BoxAABBY, ptrPos.z + m_BoxAABBZ));

		//auto PlayerAABB = AABB(Vec3(ptrPlayerPos.x, ptrPlayerPos.y - m_BoxAABBY, ptrPlayerPos.z),
		//					   Vec3(ptrPlayerPos.x, ptrPlayerPos.y, ptrPlayerPos.z));
		auto PlayerCapsure = CAPSULE(ptrPlayerScale.x, Vec3(ptrPlayerPos.x, ptrPlayerPos.y - m_PlayerAABBY, ptrPlayerPos.z), ptrPlayerPos);
		Vec3 ReturnVec = Vec3(0.0f);
		if (HitTest::CAPSULE_AABB(PlayerCapsure, m_BoxAABB,ReturnVec)) {
			//m_CurrentPos.y += -m_Speed * Elapsedtime;
			ReturnVec.x += m_PlayerAABBX;
			ReturnVec.y += m_PlayerAABBY;
			ReturnVec.z += m_PlayerAABBZ;
			ptrPlayer->GetComponent<Transform>()->SetPosition(ReturnVec);
		}
		else {
			if (m_CurrentPos.y < m_OldPos.y) {
				m_CurrentPos.y += m_Speed * Elapsedtime;
			}
			else if (m_CurrentPos.y > m_OldPos.y) {
				m_CurrentPos = m_OldPos;
			}
		}
		ptrTransform->SetPosition(m_CurrentPos);

	}

	void UpDownBox::OnUpdate() {
		BoxJudgment();
	}
}
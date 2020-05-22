#include"stdafx.h"
#include"Project.h"

namespace basecross {
	WaterDrop::WaterDrop(const shared_ptr<Stage>& Stageptr) :
		ObjectBase(Stageptr),
		m_Speed(0.0f),
		m_FallSpeed(0.0f),
		m_time(3.0f),
		m_SpeedAdd(0.4f)
	{}

	void WaterDrop::OnCreate() {
		//AddComponent<Gravity>();
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(6.0f, 3.0f, 0.0f);
		ptrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTransform->SetScale(0.5f, 0.5f, 0.5f);
		auto ptrScale = ptrTransform->GetScale();
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		m_OldPos = ptrTransform->GetPosition();
		m_CurrentPos = ptrTransform->GetPosition();
		m_WaterAABBX = ptrScale.x / 2;
		m_WaterAABBY = ptrScale.y / 2;
		m_WaterAABBZ = ptrScale.z / 2;
	}
	//＠ドロップ関数
	//＠松崎　洸樹
	//＠水滴を落とす関数
	void WaterDrop::Drop() {
		auto ptrTransform = GetComponent<Transform>();
		auto ptrPos = ptrTransform->GetPosition();
		auto Elapsedtime = App::GetApp()->GetElapsedTime();
		m_Speed += m_SpeedAdd;
		m_CurrentPos.y += -m_Speed * Elapsedtime;
		m_time += -Elapsedtime;
		if (m_time <= 0.0f) {
			m_CurrentPos = m_OldPos;
			m_time = 3.0f;
			m_Speed = 0.0f;
		}
		ptrTransform->SetPosition(m_CurrentPos);
	}
	//＠水滴判定関数
	//＠松崎　洸樹
	//＠水滴とプレイヤーの衝突判定用の関数
	void WaterDrop::WaterDropJudgement() {
		auto ptrTransform = GetComponent<Transform>();
		auto ptrPos = ptrTransform->GetPosition();
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = ptrPlayer->GetComponent<Transform>()->GetPosition();
		auto PlayerScale = ptrPlayer->GetComponent<Transform>()->GetScale();
		m_WaterDropAABB = AABB(Vec3(ptrPos.x - m_WaterAABBX, ptrPos.y - m_WaterAABBY, ptrPos.z - m_WaterAABBZ),
							   Vec3(ptrPos.x + m_WaterAABBX, ptrPos.y, ptrPos.z + m_WaterAABBZ));
		auto PlayerAABB = AABB(Vec3(PlayerPos.x - m_PlayerAABBX, PlayerPos.y, PlayerPos.z - m_PlayerAABBZ),
							   Vec3(PlayerPos.x + m_PlayerAABBX, PlayerPos.y + m_PlayerAABBY, PlayerPos.z + m_PlayerAABBZ));
		if (HitTest::AABB_AABB(m_WaterDropAABB, PlayerAABB)) {
			ptrPlayer->GetComponent<Transform>()->SetPosition(0.0, 3.0f, 0.0f);
		}

	}
	void WaterDrop::OnUpdate() {
		Drop();
		WaterDropJudgement();
	}
}
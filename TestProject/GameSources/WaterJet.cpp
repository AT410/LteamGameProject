#include"stdafx.h"
#include"Project.h"

namespace basecross{
	WaterJet::WaterJet(const shared_ptr<Stage>& Stageptr) :
		ObjectBase(Stageptr),
		m_WaterJetmode(false),
		m_WaterJetDirToUp(false),
		m_SizeAABBX(2.0f),
		m_SizeAABBY(3.0f),
		m_SizeAABBZ(2.0f)
	{}
	//＠AABBX用のセッター関数
	//＠松崎　洸樹
	//＠m_SizeAABBXのセッター用の関数
	void WaterJet::SetSizeAABBX(const float sizeX) {
		m_SizeAABBX = sizeX;
	}
	//＠AABBY用のセッター関数
	//＠松崎　洸樹
	//＠m_SizeAABBYのセッター用の関数
	void WaterJet::SetSizeAABBY(const float sizeY) {
		m_SizeAABBY = sizeY;
	}
	//＠AABBZ用のセッター関数
	//＠松崎　洸樹
	//＠m_SizeAABBZのセッター用の関数
	void WaterJet::SetSizeAABBZ(const float sizeZ) {
		m_SizeAABBZ = sizeZ;
	}

	void WaterJet::OnCreate() {
		auto ptrDraw = AddComponent<BcPNStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		auto ptrTransform = AddComponent<Transform>();
		ptrTransform->SetPosition(-4.0f, 1.0f, -1.0f);
		ptrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTransform->SetScale(1.0f, 1.0f, 1.0f);
		m_StartPos = ptrTransform->GetPosition();
		if (m_WaterJetDirToUp) {
			m_WaterJetAABB = AABB(Vec3(m_StartPos.x - m_SizeAABBX, m_StartPos.y, m_StartPos.z - m_SizeAABBZ), 
								  Vec3(m_StartPos.x + m_SizeAABBX, m_StartPos.y + m_SizeAABBY, m_StartPos.z + m_SizeAABBZ));
		}
		else {
			m_WaterJetAABB = AABB(Vec3(m_StartPos.x - m_SizeAABBX, m_StartPos.y - m_SizeAABBY, m_StartPos.z - m_SizeAABBZ),
								  Vec3(m_StartPos.x + m_SizeAABBX, m_StartPos.y, m_StartPos.z + m_SizeAABBZ));
		}

		m_efk = ObjectFactory::Create<EfkPlay>(L"TEST_EFK", m_StartPos);
	}
	//＠作動判定関数
	//＠松崎　洸樹
	//＠水噴射の機能を作動させるかどうかの判断をさせる関数
	void WaterJet::StartJudgment() {
		auto Elapsedtime = App::GetApp()->GetElapsedTime();
		m_JudmentTime += Elapsedtime;
		if (m_JudmentTime > 4.0f) {
			m_JudmentTime = 0.0f;
			if (m_WaterJetmode) {
				m_WaterJetmode = false;
			}
			else {
				m_WaterJetmode = true;
				m_efk = ObjectFactory::Create<EfkPlay>(L"TEST_EFK", m_StartPos);
			}
		}
	}

	//＠水噴射接触判定関数
	//＠松崎　洸樹
	//＠作動している水噴射とプレイヤーに接触した際にプレイヤーで起こることを記述した関数
	void WaterJet::WaterJetJudgment() {
		auto ptrTransform = AddComponent<Transform>();
		m_Pos = ptrTransform->GetPosition();
		auto GetPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = GetPlayer->GetComponent<Transform>()->GetPosition();
		AABB PlayerAABB = AABB(PlayerPos, 1, 1, 1);
		auto ptrDraw = AddComponent<BcPNStaticDraw>();
		if (m_WaterJetmode) {
				ptrDraw->SetColorAndAlpha(Col4(0.0f, 1.0f, 0.0f, 1.0f));
			if (HitTest::AABB_AABB(m_WaterJetAABB, PlayerAABB)) {
				GetPlayer->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 0.0f);
				GetPlayer->GetComponent<Transform>()->SetRotation(0.0f, 0.0f, 0.0f);
			}
		}
		else {
			ptrDraw->SetColorAndAlpha(Col4(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	void WaterJet::OnUpdate() {
		StartJudgment();
		WaterJetJudgment();
	}

	
}
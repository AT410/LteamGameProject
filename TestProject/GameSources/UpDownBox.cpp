#include"stdafx.h"
#include"Project.h"

namespace basecross {
	UpDownBox::UpDownBox(const shared_ptr<Stage>& Stageptr):
		ObjectBase(Stageptr),
		m_Speed(1.0f),
		m_OldPos(0.0f),
		m_parenttime(2.0f)
	{}

	void UpDownBox::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(0.0f, 0.0f, -6.0f);
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
		
		if (!m_ParentJudge) {
			m_parenttime += Elapsedtime;
			if (m_parenttime <= 0.0f) {
				ptrPlayer->GetComponent<Transform>()->ClearParent();
			}
		}
		else {
			m_parenttime = 2.0f;
		}
	}

	void UpDownBox::OnCollisionExcute(shared_ptr<GameObject>& Obj) {
		m_ParentJudge = true;
		auto Elapsedtime = App::GetApp()->GetElapsedTime();
		auto obj = GetComponent<Transform>()->GetGameObject();
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		if (Obj->FindTag(L"Player")) {
			auto ptrTransform = GetComponent<Transform>();
			m_CurrentPos.y +=  -m_Speed * Elapsedtime;
			ptrTransform->SetPosition(m_CurrentPos);
			ptrPlayer->GetComponent<Transform>()->SetParent(obj);
		}
	}
	void UpDownBox::OnCollisionExit(shared_ptr<GameObject>& Obj) {
		auto m_ParentJudge = false;
	}

	void UpDownBox::OnUpdate() {
		BoxJudgment();
		//ïÇÇ≠èàóù
		m_CurrentPos = GetComponent<Transform>()->GetPosition();
		FloatMove();
	}

	bool UpDownBox::FloatMove() {
		//BoxJudgment();
		//ïÇÇ≠èàóù
		m_totaltime += App::GetApp()->GetElapsedTime();
		if (m_totaltime > 5.0f)
		{
			m_totaltime = 0;
			return true;
		}

		Easing<Vec3> easing;

		auto ep = easing.Linear(m_OldPos, m_CurrentPos, m_totaltime, 5.0);

		GetComponent<Transform>()->SetPosition(ep);
	}
}
#pragma once
#include"stdafx.h"
#include"Project.h"

namespace basecross {
	class UpDownBox : public ObjectBase {
		float m_Speed;
		float m_totaltime;
		float m_parenttime;
		AABB m_BoxAABB;
		Vec3 m_OldPos;
		Vec3 m_CurrentPos;
		bool m_ParentJudge;
		bool FloatMove();
	public:
		UpDownBox(const shared_ptr<Stage>& Stageptr);
		virtual ~UpDownBox() {}
		void BoxJudgment();
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Obj) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Obj) override;
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};

}
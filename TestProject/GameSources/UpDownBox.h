#pragma once
#include"stdafx.h"
#include"Project.h"

namespace basecross {
	class UpDownBox : public ObjectBase {
		float m_BoxAABBX;
		float m_BoxAABBY;
		float m_BoxAABBZ;
		float m_PlayerAABBX;
		float m_PlayerAABBY;
		float m_PlayerAABBZ;
		float m_Speed;
		AABB m_BoxAABB;
		Vec3 m_OldPos;
		Vec3 m_CurrentPos;
	public:
		UpDownBox(const shared_ptr<Stage>& Stageptr);
		virtual ~UpDownBox() {}
		void BoxJudgment();
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Obj) override;
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};

}
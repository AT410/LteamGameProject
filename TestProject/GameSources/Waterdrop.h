//@breif 水滴ギミック用
//@name 松崎　洸樹
#pragma once
#include"stdafx.h"

namespace basecross {
	class WaterDrop : public ObjectBase {
		float m_Speed;
		float m_FallSpeed;
		float m_time;
		float m_SpeedAdd;
		float m_WaterAABBX;
		float m_WaterAABBY;
		float m_WaterAABBZ;
		float m_PlayerAABBX;
		float m_PlayerAABBY;
		float m_PlayerAABBZ;
		Vec3 m_OldPos;
		Vec3 m_CurrentPos;
		AABB m_WaterDropAABB;
	public:
		WaterDrop(const shared_ptr<Stage>& Stageptr);
		virtual ~WaterDrop() {}
		void Drop();
		void WaterDropJudgement();
		virtual void OnCreate();
		virtual void OnUpdate();
	};
}
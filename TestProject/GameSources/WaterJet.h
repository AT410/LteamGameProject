//@breif ¬ËM~bNp
//@name ¼è@©÷
#pragma once
#include"stdafx.h"

namespace basecross {
	//EH[^[WFbgNX
	//»ìÒ@¼è©÷
	class WaterJet : public ObjectBase {
		Vec3 m_Pos;
		Vec3 m_StartPos;
		bool m_WaterJetmode; //¬ËÌON/OFFXCb`
		bool m_WaterJetDirToUp; //¬ËÌü«@trueªãü« falseªºü«
		AABB m_WaterJetAABB;
		float m_JudmentTime;
		float m_SizeAABBX;
		float m_SizeAABBY;
		float m_SizeAABBZ;

		shared_ptr<EfkPlay> m_efk;

	public:
		WaterJet(const shared_ptr<Stage>& Stageptr);
		virtual ~WaterJet() {}
		void SetSizeAABBX(const float sizeX);
		void SetSizeAABBY(const float sizeY);
		void SetSizeAABBZ(const float sizeZ);
		void WaterJetJudgment();
		void StartJudgment();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};



}
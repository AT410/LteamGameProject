//@breif 水噴射ギミック用
//@name 松崎　洸樹
#pragma once
#include"stdafx.h"

namespace basecross {
	//ウォータージェットクラス
	//＠製作者　松崎洸樹
	class WaterJet : public ObjectBase {
		Vec3 m_Pos;
		Vec3 m_StartPos;
		bool m_WaterJetmode; //水噴射のON/OFFスイッチ
		bool m_WaterJetDirToUp; //水噴射の向き　trueが上向き falseが下向き
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
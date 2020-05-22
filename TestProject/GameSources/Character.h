/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class FixBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		FixBox(const shared_ptr<Stage>& Stageptr,Vec3 Scale, Vec3 Rotation, Vec3 Position);
		virtual ~FixBox() {}
		virtual void OnCreate() override;
		
	};
	class ReturnCube : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public :
		ReturnCube(const shared_ptr<Stage>& Stageptr, Vec3 Scale, Vec3 Rotation, Vec3 Position);
		virtual ~ReturnCube() {}
		virtual void OnCreate() override;
	};
	//プッシュプルオブジェクトクラス
	//＠制作者　松崎洸樹
	class PushPullObj : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		PushPullObj(const shared_ptr<Stage>& Stageptr, Vec3 Scale, Vec3 Rotation, Vec3 Position);
		virtual ~PushPullObj() {}
		virtual void OnCreate() override;
	};

	//ウォーターレベルクラス
	//＠制作者　松崎洸樹
	class WaterLV : public ObjectBase {
		float m_Waterheight;
		float m_WaterTime;
		float m_AABBX;
		float m_AABBY;
		float m_AABBZ;
		float m_PlayerAABBX;
		float m_PlayerAABBY;
		float m_PlayerAABBZ;
		bool m_WaterLVMode;
		Vec3 m_WaterOldPos;
		Vec3 m_WaterCurrentPos;
		AABB m_WaterLVAABB;
		AABB m_PlayerAABB;
	public:
		WaterLV(const shared_ptr<Stage>& Stageptr);
		virtual ~WaterLV() {}
		void SetWaterLVMode(const bool LVMode);
		void WaterLVChange();
		void WaterLVJudgement();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}
//end basecross

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
	class PushPullObj : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		PushPullObj(const shared_ptr<Stage>& Stageptr, Vec3 Scale, Vec3 Rotation, Vec3 Position);
		virtual ~PushPullObj() {}
		virtual void OnCreate() override;
	};
}
//end basecross

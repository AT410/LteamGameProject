#pragma once
#include "stdafx.h"

namespace basecross
{
	class WaterWheel :public GameObject
	{
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float Time;
		float m_TotalTime;
		float m_Swap;

	public:
		WaterWheel(const shared_ptr<Stage>&Stageptr,
			const Vec3& scale,
			const Vec3& Rotation,
			const Vec3& Position
		);

		virtual ~WaterWheel();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};


	//バルブ
	class Valve : public GameObject
	{
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float Time;
		float m_TotalTime;
		float m_Swap;
		float m_valveTime;

	public:
	Valve (const shared_ptr<Stage>&Stageptr,
			const Vec3& scale,
			const Vec3& Rotation,
			const Vec3& Position
		);

		virtual ~Valve();
		//初期化
		virtual void OnCreate() override;
		virtual void  OnUpdate() override;
		//virtual void OnUpdate() override;
	};
}
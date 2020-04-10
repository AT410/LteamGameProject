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
		//èâä˙âª
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}
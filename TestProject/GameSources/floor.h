#pragma once
#include "stdafx.h"

namespace basecross
{
	class Floor : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		Floor(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~Floor();
		//èâä˙âª
		virtual void OnCreate() override;
	};

}
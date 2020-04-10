#pragma once
#include "stdafx.h"

namespace basecross
{
	class Background: public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		Background(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~Background();
		//èâä˙âª
		virtual void OnCreate() override;
	};

}

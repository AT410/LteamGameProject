
#pragma once
#include "stdafx.h"

namespace basecross
{
	class Gate2 : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		bool gateopen2 = false;
		float OpenSpeed = 2;
	public:
		Gate2(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~Gate2();
		void setopenflg2(bool active)
		{
			gateopen2 = active;
		}
		//èâä˙âª
		void GateOpen2();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}
#pragma once
#include "stdafx.h"
namespace basecross
{

	class Gate : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		bool gateopen = false;
		float OpenSpeed = 2;
	public:
		Gate(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~Gate();
		//gate�������邽�߂̃t���O
		void setopenflg(bool active)
		{
			gateopen = active;
		}
		//������
		void GateOpen();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};


}
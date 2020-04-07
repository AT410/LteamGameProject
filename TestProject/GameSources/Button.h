/*!
@file Player.h
@brief ギミックのボタンなど
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class Button :public GameObject
	{
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;

	public:
		Button(const shared_ptr<Stage>&Stageptr,
			const Vec3& scale,
			const Vec3& Rotation,
			const Vec3& Position
		);

		Vec3 GetPosition() const
		{
			return GetComponent<Transform>()->GetPosition();
		}
		void OnCollisionEnter(shared_ptr<GameObject>&other)override;
		virtual ~Button();
		virtual void OnCreate() override;
		virtual void OnUpdate()override;
	};

}

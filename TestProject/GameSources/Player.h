/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class Player:public GameObject
	{
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		int PlayerSpeed;
		Vec3 forward;

	public :
		Player(const shared_ptr<Stage>&Stageptr,
			const Vec3& scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		Vec3 GetPosition() const
		{
			return GetComponent<Transform>()->GetPosition();
		}
		virtual ~Player();
		virtual void OnCreate() override;
		virtual void OnUpdate()override;
	};

}
//end basecross


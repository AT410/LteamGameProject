/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class Player : public ObjectBase, public PawnBase<Player> {
		const int m_cntlNum;

		Vec3 m_Position;
		Vec3 m_Rotation;
		Vec3 m_Scale;
		Vec3 m_PlayerAngle;
		Vec3 GetMoveVector() const;
		Controller m_cntl;
		float m_Speed;
		float m_Jumpforce;
		float m_Dethtime;
		bool m_Jumpjudge;
		bool m_Dethtimejudge;


		//入力ハンドラー
		InputHandler<Player> m_inputHandler;
	public:
		Player(const shared_ptr<Stage>& Stageptr);
		virtual ~Player() {}
		void Move();
		virtual void OnPushA();
		//void Squat();
		//void Torch();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Obj) override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Obj) override;
	};

}
//end basecross


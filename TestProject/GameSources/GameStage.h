/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
		void GamePlayer();
		void Object();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
	};

	class GameStageShogo : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
		void GamePlayer();
		void Object();
	public:
		//�\�z�Ɣj��
		GameStageShogo() :Stage() {}
		virtual ~GameStageShogo() {}
		//������
		virtual void OnCreate()override;
	};

}
//end basecross


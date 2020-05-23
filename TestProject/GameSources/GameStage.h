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
		shared_ptr<SingleView> m_MyCameraView;
		void CreateViewLight();
		void GamePlayer();
		void Object();

	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;

		void OnUpdate()override
		{
			App::GetApp()->GetScene<Scene>()->GetEfkInterface()->OnUpdate();
		}

		void OnDraw()override
		{
			auto& camera = GetView()->GetTargetCamera();
			App::GetApp()->GetScene<Scene>()->GetEfkInterface()->SetViewProj(camera->GetViewMatrix(), camera->GetProjMatrix());
			App::GetApp()->GetScene<Scene>()->GetEfkInterface()->OnDraw();
		}
	};

	class GameStageHurukawa : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
		bool gateopen = false;
	public:
		//�\�z�Ɣj��
		GameStageHurukawa() :Stage() {}
		virtual ~GameStageHurukawa() {}
		//������
		virtual void OnCreate()override;
	};

	class GameStageShogo : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
	public:
		//�\�z�Ɣj��
		GameStageShogo() :Stage() {}
		virtual ~GameStageShogo() {}
		//������
		virtual void OnCreate()override;
	};
	//�e�N�X�`���o�^�̏���������

	class Stagechoice :public Stage
	{
	public:
		void CreateTraceSprite();
		void CreateTraceSprite2();
		void CreateTraceSprite3();
		void CreateTraceSprite4();
		void CreateViewLight();
		void UIctr();
		Stagechoice() :Stage() {}
		virtual ~Stagechoice() {}
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};

	class Translucent :public Stage
	{
	public:
		void CreateViewLight();
		Translucent() :Stage() {}
		virtual ~Translucent() {}
		virtual void OnCreate()override;
	};

	class SelectStage :public Stage
	{
	public:
		void CreateTraceSprite2();
		void CreateViewLight();
		SelectStage() :Stage() {}
		virtual ~SelectStage() {}
		virtual void OnCreate()override;
	};


	class TitleStage : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
		void CreateTitleSprite();
		bool gateopen = false;
	public:
		//�\�z�Ɣj��
		TitleStage() :Stage() {}
		virtual ~TitleStage() {}
		//������
		virtual void OnCreate()override;
	};
}
//end basecross


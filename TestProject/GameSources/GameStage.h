/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------

	class GameStage : public Stage {
		//ビューの作成
		shared_ptr<SingleView> m_MyCameraView;
		void CreateViewLight();
		void GamePlayer();
		void Object();

	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
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
		//ビューの作成
		void CreateViewLight();
		bool gateopen = false;
	public:
		//構築と破棄
		GameStageHurukawa() :Stage() {}
		virtual ~GameStageHurukawa() {}
		//初期化
		virtual void OnCreate()override;
	};

	class GameStageShogo : public Stage {
		//ビューの作成
		void CreateViewLight();
	public:
		//構築と破棄
		GameStageShogo() :Stage() {}
		virtual ~GameStageShogo() {}
		//初期化
		virtual void OnCreate()override;
	};
	//テクスチャ登録の処理を書く

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
		//ビューの作成
		void CreateViewLight();
		void CreateTitleSprite();
		bool gateopen = false;
	public:
		//構築と破棄
		TitleStage() :Stage() {}
		virtual ~TitleStage() {}
		//初期化
		virtual void OnCreate()override;
	};
}
//end basecross


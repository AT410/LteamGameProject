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
		void CreateViewLight();
		void GamePlayer();
		void Object();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;

	};

	class GameStageHurukawa : public Stage {
		//ビューの作成
		void CreateViewLight();
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

}
//end basecross


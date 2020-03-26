/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ステージベースクラス
	//--------------------------------------------------------------------------------------
	class StageBase : public Stage 
	{
	public:
		//構築と破棄
		StageBase() :Stage() {}
		virtual ~StageBase() {}
		//初期化
		virtual void OnCreate()override = 0;
		virtual void OnUpdate()override = 0;
		virtual void OnDestroy()override;
	protected:
		//ビューの作成
		virtual void CreateViewLight();

		void SetBGM(const wstring& BGMKey);

		shared_ptr<SoundItem> m_BGMPtr;

	};

	//--------------------------------------------------------------------------------------
	//	タイトルステージクラス
	//--------------------------------------------------------------------------------------
	class TitleStage :public StageBase
	{
	public:
		//構築と破棄
		TitleStage():StageBase(){}
		virtual ~TitleStage(){}
		//初期化
		void OnCreate()override;
		void OnUpdate()override;
	private:


	};
	//--------------------------------------------------------------------------------------
	//	セーブデータセレクトステージクラス
	//--------------------------------------------------------------------------------------
	class DataSelectStage :public StageBase
	{
	public:
		DataSelectStage():StageBase(){}
		virtual ~DataSelectStage(){}
		//初期化
		void OnCreate()override;
		void OnUpdate()override {};

	};
	//--------------------------------------------------------------------------------------
	//	エリアセレクトステージクラス
	//--------------------------------------------------------------------------------------
	class AreaSelectStage :public StageBase
	{
	public:
		AreaSelectStage() :StageBase() {}
		virtual ~AreaSelectStage() {}
		//初期化
		void OnCreate()override;
		void OnUpdate()override {};
	};

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage :public StageBase
	{
	public:
		GameStage() :StageBase() {}
		virtual ~GameStage() {}
		//初期化
		void OnCreate()override;
		void OnUpdate()override {};
	};

	//--------------------------------------------------------------------------------------
	//	エンディングステージクラス
	//--------------------------------------------------------------------------------------
	class EndingStage :public StageBase
	{
	public:
		EndingStage() :StageBase() {}
		virtual ~EndingStage() {}
		//初期化
		void OnCreate()override;
		void OnUpdate()override {};
	};

}
//end basecross


/*!
@file GameStage.h
@brief ゲームステージ
@name 作成者：阿部達哉
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	enum class StageType
	{
		TitleStage,
		LoadStage,
		DataSelectStage,
		AreaSelectStage,
		StageSelectStage,
		GameStage,
		EndingStage
	};

	//--------------------------------------------------------------------------------------
	//	ステージベースクラス
	//--------------------------------------------------------------------------------------
	class StageBase : public Stage 
	{
	public:
		//構築と破棄
		StageBase(const StageType Type) 
			:Stage(),m_StageType(Type) {}
		virtual ~StageBase() {}
		//初期化
		virtual void OnCreate()override = 0;
		virtual void OnUpdate()override {};
		virtual void OnDestroy()override;
		virtual void OnDraw()override {};

		StageType GetStageType()const { return m_StageType; }

		wstring GetStageTypeStr()const;

		shared_ptr<SingleView> GetOpeningView() { return m_OpeningView; }
		shared_ptr<SingleView> GetMainView() { return m_MainView; }
	protected:
		//ビューの作成
		virtual void CreateViewLight();

		void SetBGM(const wstring& BGMKey,const bool LoopActive = true);

		shared_ptr<SoundItem> m_BGMPtr = nullptr;

		StageType m_StageType;

		//カメラ
		shared_ptr<SingleView> m_MainView;
		shared_ptr<SingleView> m_OpeningView;
	};

	//--------------------------------------------------------------------------------------
	//	タイトルステージクラス
	//--------------------------------------------------------------------------------------
	class TitleStage :public StageBase
	{
	public:
		//構築と破棄
		TitleStage():StageBase(StageType::TitleStage){}
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
		DataSelectStage() :StageBase(StageType::DataSelectStage) {}
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
		AreaSelectStage() :StageBase(StageType::AreaSelectStage) {}
		virtual ~AreaSelectStage() {}
		//初期化
		void OnCreate()override;
		void OnUpdate()override {};
	};

	//--------------------------------------------------------------------------------------
	// ステージセレクトステージクラス
	//--------------------------------------------------------------------------------------
	class StageSelectStage :public StageBase
	{
	public:
		StageSelectStage() :StageBase(StageType::StageSelectStage) {}
		virtual ~StageSelectStage() {}
		//初期化
		void OnCreate()override;
		void OnUpdate()override {};
	};

	//--------------------------------------------------------------------------------------
	//ロード待機ステージ(ロード終了時自動で遷移）
	//--------------------------------------------------------------------------------------
	class LoadStage :public StageBase
	{
	public:
		LoadStage() :StageBase(StageType::LoadStage){}
		virtual ~LoadStage(){}

		//初期化
		void OnCreate()override;
		//更新
		void OnUpdate()override;
	};
	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage :public StageBase
	{
		shared_ptr<EfkPlay> m_EfkPlay[10];
		int m_EfkCount = 0;

	public:
		GameStage() :StageBase(StageType::GameStage) {}
		virtual ~GameStage() {}
		//初期化
		void OnCreate()override;
		void OnUpdate()override;

		void OnDraw()override;

		void Effectplay(wstring Key, Vec3 hitpoint);

		void ToReStart();

		void ToMyCamera();

		void ToEventCamera();
	private:
	};

	//--------------------------------------------------------------------------------------
	//	エンディングステージクラス
	//--------------------------------------------------------------------------------------
	class EndingStage :public StageBase
	{
	public:
		EndingStage() :StageBase(StageType::EndingStage) {}
		virtual ~EndingStage() {}
		//初期化
		void OnCreate()override;
		void OnUpdate()override {};
	};

}
//end basecross


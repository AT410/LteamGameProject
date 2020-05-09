/*!
@breif ゲーム全体の管理定義
@name　作成者：阿部達哉
*/
#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@berif ゲームマネージャークラス
	@info ゲーム全体の管理・ステージオブジェクトの生成
	*/
	class GameManager
	{
	private:
		struct GMDeleter
		{
			void operator()(GameManager *ptr) { delete ptr; }
		};

		static unique_ptr<GameManager, GMDeleter> m_ins;	///<-マネージャーのインスタンス

		pair<int, int > m_SelectStage;						///<-ステージ選択した値

		wstring m_MapFile;									///<-マップデータファイル

		wstring m_ResFile;									///<-リソースリストファイル

		std::mutex mutex;

		bool m_Loaded;

		void ResorceLoadFunc();								///<-リソースの読込を行う

		GameManager();
		~GameManager();
	public:
		static void CreateManager();
		
		static unique_ptr<GameManager, GMDeleter>& GetManager();

		static bool CheckManager();

		static void DeleteManager();

		void LoadResources();

		//ステージの生成
		//ステージから呼ばれる
		void CreateStage(const shared_ptr<StageBase>&StagePtr,const wstring& FileName, const bool MenuActive);

		//ゲッター
		pair<int, int> GetStagePair()const {return m_SelectStage; }
		bool GetLoadFlag()const { return m_Loaded; }
		//セッター
		void SetStagePair(int AreaNum, int StageNum) { m_SelectStage = make_pair(AreaNum, StageNum); }
		void SetStagePair(const pair<int, int> StagePair) { m_SelectStage = StagePair; }
	private:
		//コピー禁止
		GameManager(const GameManager&) = delete;
		GameManager& operator=(const GameManager&) = delete;
		//ムーブ禁止
		GameManager(const GameManager&&) = delete;
		GameManager& operator=(const GameManager&&) = delete;

	};
}
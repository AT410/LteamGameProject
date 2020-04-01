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

		GameManager();
		~GameManager();
	public:
		static void CreateManager();
		
		static unique_ptr<GameManager, GMDeleter>& GetManager();

		static bool CheckManager();

		static void DeleteManager();
	};
}
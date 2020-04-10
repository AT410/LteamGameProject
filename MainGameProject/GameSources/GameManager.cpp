/*!
@breif ゲーム全体の管理実体
@name　作成者:阿部達哉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//static変数の実体
	unique_ptr<GameManager,GameManager::GMDeleter> GameManager::m_ins;

	GameManager::GameManager()
		:m_SelectStage(0,0)
	{

	}

	GameManager::~GameManager()
	{

	}

	//static関数
	void GameManager::CreateManager()
	{
		try
		{
			if (m_ins.get() == 0)
			{
				m_ins.reset(new GameManager());
			}
		}
		catch (...)
		{
			throw;
		}
	}

	//Get
	unique_ptr<GameManager, GameManager::GMDeleter>& GameManager::GetManager()
	{
		try
		{
			if (m_ins.get() == 0)
			{
				throw(BaseException(
					L"GameManagerが生成されていません",
					L"if(m_ins.get()==0)",
					L"GameManager::GetManager"
				));
			}
			else
				return m_ins;
		}
		catch (...)
		{
			throw;
		}
	}


	bool GameManager::CheckManager()
	{
		if (m_ins == 0)
			return false;
		else
			return true;
	}

	void GameManager::DeleteManager()
	{
		if (m_ins.get() == 0)
		{
			//廃棄処理
			m_ins.reset();
		}
	}

	//ステージ生成
	void GameManager::CreateStage(const shared_ptr<StageBase>&StagePtr,const wstring& FileName)
	{
		StageBulider Builder;
		Builder.Register<StageTest>(L"Test");

		Builder.StageBuild(StagePtr, FileName);
	}
}
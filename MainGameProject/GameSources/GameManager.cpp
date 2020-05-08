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
	void GameManager::CreateStage(const shared_ptr<StageBase>&StagePtr, const wstring& FileName, const bool MenuActive)
	{
		StageBulider Builder;

		//セレクト画面にするか
		if (!MenuActive) 
		{
			StagePtr->CreateSharedObjectGroup(L"Rock");
			Builder.Register<StageTest>(L"Test");
			Builder.Register<StageTest>(L"Floor");
			Builder.Register<Player>(L"Player");
			Builder.Register<Omori>(L"Omori");
			Builder.Register<HeatStick>(L"HeatStick");
			Builder.Register<MoveFloor>(L"MoveFloor");
			Builder.Register<FixedObj>(L"Himo");
			Builder.Register<Fountain>(L"Fountain");
			Builder.Register<GoalTest>(L"Goal");
			Builder.Register<SwitchObj>(L"Switch");

			Builder.Register<RockTest>(L"Rock");
			Builder.Register<FixedObj>(L"Wall");

			Builder.StageBuild(StagePtr, FileName);

			//開始イベントをスタート
			auto Ptr = StagePtr->AddGameObject<OpeningCameraman>();
			auto OPCam = dynamic_pointer_cast<OpeningCamera>(StagePtr->GetOpeningView()->GetCamera());
			if (OPCam)
			{
				OPCam->SetCameraObject(Ptr);
			}
		}
		else
		{

		}
	}
}

/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate(){
		try {
			//クリアする色を設定
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);
			SetDebugResoruce();
			GameManager::CreateManager();
			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToAreaSelectStage");
		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToTileStage") {
			//最初のアクティブステージの設定
			ResetActiveStage<TitleStage>();
		}
		else if (event->m_MsgStr == L"ToDataSelectStage")
		{
			ResetActiveStage<DataSelectStage>();
		}
		else if (event->m_MsgStr == L"ToAreaSelectStage")
		{
			ResetActiveStage<AreaSelectStage>();
		}
		else if (event->m_MsgStr == L"ToGameStage")
		{
			ResetActiveStage<GameStage>();
		}
		else if (event->m_MsgStr == L"ToEndingStage")
		{
			ResetActiveStage<EndingStage>();
		}
	}

	void Scene::SetDebugResoruce()
	{
		wstring MediaPath;
		App::GetApp()->GetDataDirectory(MediaPath);
		MediaPath += L"DebugTex/";
		//Tex
		App::GetApp()->RegisterTexture(L"TitleStage_TX", MediaPath + L"TitleStage.png");
		App::GetApp()->RegisterTexture(L"DateSelect_TX", MediaPath + L"DataSelect.png");
		App::GetApp()->RegisterTexture(L"AreaSelect_TX", MediaPath + L"AreaSelect.png");
		App::GetApp()->RegisterTexture(L"GameStage_TX", MediaPath + L"GameStage.png");
		App::GetApp()->RegisterTexture(L"EndingStage_TX", MediaPath + L"EndingStage.png");

		App::GetApp()->RegisterTexture(L"TEST_TX", MediaPath + L"itimatu.bmp");

	}

}
//end basecross

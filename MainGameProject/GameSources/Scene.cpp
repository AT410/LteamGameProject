
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
			_EfkInterface = ObjectFactory::Create<EfkInterface>();
			LoadUnionResource();
			GameManager::CreateManager();
			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToLoadStage");
		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToTitleStage") {
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
		else if (event->m_MsgStr == L"ToLoadStage")
		{
			ResetActiveStage<LoadStage>();
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

	//リソースの初期読込
	void Scene::LoadUnionResource()
	{
		wstring MediaPath;
		wstring EFkPath;
		wstring BGMPath;
		App::GetApp()->GetDataDirectory(MediaPath);
		EFkPath = MediaPath + L"Effect/";
		BGMPath = MediaPath + L"Sound/";
		MediaPath += L"Texture/";
		App::GetApp()->RegisterTexture(L"WAIT_TX", MediaPath + L"wait.png");
		App::GetApp()->RegisterTexture(L"WATER_TX", MediaPath + L"water.jpg");
	}

	void Scene::SetDebugResoruce()
	{
		wstring MediaPath;
		wstring EFkPath;
		wstring BGMPath;
		App::GetApp()->GetDataDirectory(MediaPath);
		EFkPath = MediaPath + L"Effect/";
		BGMPath = MediaPath + L"Sound/";
		MediaPath += L"DebugTex/";
		//Tex
		App::GetApp()->RegisterTexture(L"TitleStage_TX", MediaPath + L"TitleStage.png");
		App::GetApp()->RegisterTexture(L"DateSelect_TX", MediaPath + L"DataSelect.png");
		App::GetApp()->RegisterTexture(L"AreaSelect_TX", MediaPath + L"AreaSelect.png");
		App::GetApp()->RegisterTexture(L"GameStage_TX", MediaPath + L"GameStage.png");
		App::GetApp()->RegisterTexture(L"EndingStage_TX", MediaPath + L"EndingStage.png");

		App::GetApp()->RegisterTexture(L"Stage1_TX", MediaPath + L"Stage1.png");
		App::GetApp()->RegisterTexture(L"Stage2_TX", MediaPath + L"Stage2.png");
		App::GetApp()->RegisterTexture(L"Stage3_TX", MediaPath + L"Stage3.png");
		App::GetApp()->RegisterTexture(L"Stage4_TX", MediaPath + L"Stage4.png");
		App::GetApp()->RegisterTexture(L"Stage5_TX", MediaPath + L"Stage5.png");


		//MDTex
		//App::GetApp()->RegisterTexture(L"TEST_TX", MediaPath + L"itimatu.bmp");
		App::GetApp()->RegisterTexture(L"TEST_TX", MediaPath + L"kabe.jpg");
		App::GetApp()->RegisterTexture(L"FLOOR_TX", MediaPath + L"ytail.jpg");		
		App::GetApp()->RegisterTexture(L"WALL_TX", MediaPath + L"Wall2.png");
		App::GetApp()->RegisterTexture(L"Rock_TX", MediaPath + L"Brown2.png");
		App::GetApp()->RegisterTexture(L"Number_TX", MediaPath + L"number.png");
		App::GetApp()->RegisterTexture(L"SelectNum_TX", MediaPath + L"SelectNum.png");

		App::GetApp()->RegisterEffect(L"TEST_EFK", EFkPath + L"Clear.efk",_EfkInterface,1.0f);
		App::GetApp()->RegisterEffect(L"FIRE_EFK", EFkPath + L"Fire.efk", _EfkInterface);

		App::GetApp()->RegisterWav(L"TEST_SD", BGMPath+L"BGMTest.wav");
	}

}
//end basecross

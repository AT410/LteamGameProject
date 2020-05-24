
/*!
@file Scene.cpp
@brief �V�[������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate(){
		try {
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);
			_EfkInterface = ObjectFactory::Create<EfkInterface>();
			LoadUnionResource();
			GameManager::CreateManager();
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

	void Scene::LoadUnionResource()
	{
		wstring MediaPath;
		wstring EFkPath;
		wstring BGMPath;
		wstring ModelPath;
		App::GetApp()->GetDataDirectory(MediaPath);
		ModelPath = MediaPath + L"Model/";
		EFkPath = MediaPath + L"Effect/";
		BGMPath = MediaPath + L"Sound/";
		MediaPath += L"Texture/";
		App::GetApp()->RegisterTexture(L"WAIT_TX", MediaPath + L"wait.png");
		App::GetApp()->RegisterTexture(L"TEXTTEST_TX", MediaPath + L"PressA.png");
		App::GetApp()->RegisterTexture(L"TITLE_TX", MediaPath + L"title.jpg");
		App::GetApp()->RegisterTexture(L"BackGround_TX", MediaPath + L"BackGround.png");
		App::GetApp()->RegisterTexture(L"StageSelect_TX", MediaPath + L"StageSelect.png");
		App::GetApp()->RegisterTexture(L"One_TX", MediaPath + L"One.png");
		App::GetApp()->RegisterTexture(L"Two_TX", MediaPath + L"Two.png");
		App::GetApp()->RegisterTexture(L"Three_TX", MediaPath + L"Three.png");
		App::GetApp()->RegisterTexture(L"GAMECLEAR_TX", MediaPath + L"GameClear.png");
		auto MeshRes = MeshResource::CreateStaticModelMesh(ModelPath, L"TestN.bmf");
		App::GetApp()->RegisterResource(L"TESTN_MD", MeshRes);
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

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	// -- 初期化 --
	void Scene::OnCreate(){
		try {
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col4(0));
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

	// -- イベント処理 --
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
		else if (event->m_MsgStr == L"ToStageSelectStage")
		{
			ResetActiveStage<StageSelectStage>();
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

	// -- 初期リソース読込 --
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
	}
}
//end basecross

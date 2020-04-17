
/*!
@file Scene.cpp
@brief �V�[������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate(){
		try {
			//�N���A����F��ݒ�
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);
			SetDebugResoruce();
			GameManager::CreateManager();
			//�������g�ɃC�x���g�𑗂�
			//����ɂ��e�X�e�[�W��I�u�W�F�N�g��Create���ɃV�[���ɃA�N�Z�X�ł���
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
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
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

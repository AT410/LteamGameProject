/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�X�e�[�W�x�[�X�N���X����
	//--------------------------------------------------------------------------------------
	void StageBase::CreateViewLight() {
		const Vec3 eye(7.0f, 10.0f, -20.0f);
		const Vec3 at(7.0f,2.0f,0.0f);
		m_MainView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<MyCamera>();
		m_MainView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		PtrCamera->SetExpansionEye(eye);
		PtrCamera->SetExpansionAt(at);
		m_OpeningView = CreateView<SingleView>();
		auto PtrOPCamera = ObjectFactory::Create<OpeningCamera>();
		m_OpeningView->SetCamera(PtrOPCamera);
		PtrOPCamera->SetEye(eye);
		PtrOPCamera->SetAt(at);

		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	void StageBase::SetBGM(const wstring& BGMKey)
	{
		auto XAudioPtr = App::GetApp()->GetXAudio2Manager();
		m_BGMPtr = XAudioPtr->Start(BGMKey,XAUDIO2_LOOP_INFINITE,0.25f);
	}

	void StageBase::OnDestroy()
	{
		//BGM���I��
		auto XAudioPtr = App::GetApp()->GetXAudio2Manager();
		XAudioPtr->Stop(m_BGMPtr);
	}

	wstring StageBase::GetStageTypeStr()const
	{
		wstring TypeStr;
		switch (m_StageType)
		{
		case basecross::StageType::TitleStage:
			TypeStr = L"Title";
			break;
		case basecross::StageType::LoadStage:
			TypeStr = L"Load";
			break;
		case basecross::StageType::DataSelectStage:
			TypeStr = L"DataSelect";
			break;
		case basecross::StageType::AreaSelectStage:
			TypeStr = L"AreaSelect";
			break;
		case basecross::StageType::GameStage:
			TypeStr = L"GameStage";
			break;
		case basecross::StageType::EndingStage:
			TypeStr = L"Ending";
			break;
		default:
			break;
		}
		return TypeStr;
	}

	//--------------------------------------------------------------------------------------
	//	�^�C�g���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void TitleStage::OnCreate()
	{
		try 
		{
			CreateViewLight();
			AddGameObject<UIController>(GetStageType());
			//AddGameObject<ContTest>(L"ToAreaSelectStage");
			//AddGameObject<NormalUI>();
			//AddGameObject<DebugSprite>(L"TitleStage_TX");
			//AddGameObject<ActionTest>();
			GameManager::GetManager()->CreateUISet(GetThis<TitleStage>());
		}
		catch (...)
		{

		}
	}

	void TitleStage::OnUpdate()
	{

	}

	//--------------------------------------------------------------------------------------
	//�Z�[�u�f�[�^�X�e�[�W
	//--------------------------------------------------------------------------------------
	void DataSelectStage::OnCreate()
	{
		try
		{
			CreateViewLight();
			AddGameObject<DebugSprite>(L"DateSelect_TX");
			AddGameObject<ContTest>(L"ToAreaSelectStage");

		}
		catch (...)
		{

		}
	}
	//--------------------------------------------------------------------------------------
	//�G���A�Z���N�g�X�e�[�W
	//--------------------------------------------------------------------------------------
	void AreaSelectStage::OnCreate()
	{
		try
		{
			CreateViewLight();
			//AddGameObject<DebugSprite>(L"AreaSelect_TX");
			//AddGameObject<ContTest>(L"ToGameStage");
			AddGameObject<UIController>(GetStageType());
			GameManager::GetManager()->CreateUISet(GetThis<AreaSelectStage>());
		}
		catch (...)
		{

		}
	}

	//--------------------------------------------------------------------------------------
	//	���[�h�X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void LoadStage::OnCreate()
	{
		CreateViewLight();
		//���\�[�X�̃��[�h���s��
		GameManager::GetManager()->LoadResources();

		AddGameObject<AnimSpriteTest>(5, true);
	}

	void LoadStage::OnUpdate()
	{
		auto IsLoaded = GameManager::GetManager()->GetLoadFlag();
		if (IsLoaded)
		{
			PostEvent(0.0f, GetThis<LoadStage>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
		}
	}

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------

	void GameStage::ToMyCamera()
	{
		this->SetView(m_MainView);
		auto Pair = GameManager::GetManager()->GetStagePair();
		AddGameObject<AnimSpriteTest>(Pair.second);
		PostEvent(0.0f, GetThis<GameStage>(), L"Start", L"StartAction");
	}

	void GameStage::OnCreate()
	{
		try
		{
			SetPhysicsActive(true);
			CreateViewLight();
			wstring Test;
			App::GetApp()->GetDataDirectory(Test);
			GameManager::GetManager()->CreateGameStage(GetThis<StageBase>());

			AddGameObject<UIController>(GetStageType(),false);
			GameManager::GetManager()->CreateUISet(GetThis<StageBase>(), false);
			SetBGM(L"MAIN_SD");
			//AddGameObject<Waterfall>(Vec3(0, 5, 0), Vec3(0, 0, 0), 2.0f, 1.0f);

		}
		catch (...)
		{

		}
	}

	void GameStage::OnUpdate()
	{
		App::GetApp()->GetScene<Scene>()->GetEfkInterface()->OnUpdate();

		auto InputKey = App::GetApp()->GetInputDevice().GetKeyState();
		if (InputKey.m_bPressedKeyTbl['H'])
		{
			PostEvent(0.0f, GetThis<Stage>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
		}
	}

	void GameStage::OnDraw()
	{
		auto& camera = GetView()->GetTargetCamera();
		App::GetApp()->GetScene<Scene>()->GetEfkInterface()->SetViewProj(camera->GetViewMatrix(), camera->GetProjMatrix());
		App::GetApp()->GetScene<Scene>()->GetEfkInterface()->OnDraw();
	}
	//--------------------------------------------------------------------------------------
	//	�^�C�g���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void EndingStage::OnCreate()
	{
		try
		{
			CreateViewLight();
			AddGameObject<DebugSprite>(L"GAMECLEAR_TX");
			AddGameObject<ContTest>(L"ToTitleStage");
		}
		catch (...)
		{

		}
	}
}
//end basecross

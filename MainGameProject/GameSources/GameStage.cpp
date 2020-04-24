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
		const Vec3 eye(0.0f, 2.0f, -15.0f);
		const Vec3 at(0.0f,2.0f,0.0f);
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
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

	//--------------------------------------------------------------------------------------
	//	�^�C�g���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void TitleStage::OnCreate()
	{
		try 
		{
			CreateViewLight();
			AddGameObject<DebugSprite>(L"TitleStage_TX");
			AddGameObject<ContTest>();
		}
		catch (...)
		{

		}
	}

	void TitleStage::OnUpdate()
	{

	}

	//--------------------------------------------------------------------------------------
	//	�^�C�g���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void DataSelectStage::OnCreate()
	{
		try
		{
			CreateViewLight();
			AddGameObject<DebugSprite>(L"DateSelect_TX");
		}
		catch (...)
		{

		}
	}
	//--------------------------------------------------------------------------------------
	//	�^�C�g���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void AreaSelectStage::OnCreate()
	{
		try
		{
			CreateViewLight();
			AddGameObject<DebugSprite>(L"AreaSelect_TX");
			AddGameObject<ContTest>();
		}
		catch (...)
		{

		}
	}
	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void GameStage::GenerateStage()
	{
		//�G���A�Z���N�g�őI�������G���A�E�X�e�[�W���Ɋ�Â��Đ�������B
		//GameObjecttXMLBuilder MapBuilder;
		//�I�u�W�F�N�g�̓o�^

		//MapBuilder.Build(GetThis<Stage>(),L"�}�b�v�t�@�C��",L"�����p�X");
	}

	void GameStage::OnCreate()
	{
		try
		{
			CreateViewLight();
			wstring Test;
			App::GetApp()->GetDataDirectory(Test);
			GameManager::GetManager()->CreateStage(GetThis<StageBase>(), Test + L"DefaultPath.xml",false);
			//AddGameObject<DebugSprite>(L"GameStage_TX");
			//AddGameObject<MoveObj>(Vec3(0), Vec3(0), Vec3(1), L"TEST_TX", L"DEFAULT_CUBE", Vec3(2, 0, 0), Vec3(-2, 0, 0), 2.5f, 10.0f);

			////�����v�Z�L��
			//SetPhysicsActive(true);

			////�i���j�v���C���[
			//auto PlayerPtr = AddGameObject<Player>();

			//////�Ђ�
			//auto HimoPtr = AddGameObject<FixedObj>(Vec3(4, 5, 0), Vec3(0), Vec3(0.5, 2, 0.5), L"TEST_TX", L"DEFAULT_CUBE");
			////�Ђ��̓o�^
			//SetSharedGameObject(L"Himo", HimoPtr);
			//////������
			//auto OmoriPtr = AddGameObject<Omori>(Vec3(4, 3.5, 0), Vec3(0), Vec3(1), L"TEST_TX", L"DEFAULT_CUBE");

			//////�M�_
			//AddGameObject<HeatStick>(Vec3(0, 6, 0), Vec3(0), Vec3(10, 1, 1), L"TEST_TX", L"DEFAULT_CUBE");

			////�{�^��
			//AddGameObject<SwitchObj>(Vec3(4, 1, 0), Vec3(0), Vec3(2, 1, 2), L"TEST_TX", L"DEFAULT_CUBE");

			////��(��)
			//AddGameObject<FixedObj>(Vec3(-8, 0, 0), Vec3(0), Vec3(10, 1, 15), L"TEST_TX", L"DEFAULT_CUBE");
			////��(�E)
			//AddGameObject<FixedObj>(Vec3(8, 0, 0), Vec3(0), Vec3(10, 1, 15), L"TEST_TX", L"DEFAULT_CUBE");
			////��(��)
			//AddGameObject<FixedObj>(Vec3(0, -4, 0), Vec3(0), Vec3(20, 1, 15), L"TEST_TX", L"DEFAULT_CUBE");

		}
		catch (...)
		{

		}
	}
	//--------------------------------------------------------------------------------------
	//	�^�C�g���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void EndingStage::OnCreate()
	{
		try
		{
			CreateViewLight();
			AddGameObject<DebugSprite>(L"EndingStage_TX");
		}
		catch (...)
		{

		}
	}
}
//end basecross

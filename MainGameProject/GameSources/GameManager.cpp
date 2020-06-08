/*!
@breif �Q�[���S�̂̊Ǘ�����
@name�@�쐬��:�����B��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//static�ϐ��̎��2
	unique_ptr<GameManager,GameManager::GMDeleter> GameManager::m_ins;

	GameManager::GameManager()
		:m_SelectStage(0,0),m_MapFile(L"TestStage2.xml"),m_ResFile(L"ResMap.xml"),m_UISetFile(L"TestUI.xml"),m_Loaded(false)
	{

	}

	GameManager::~GameManager()
	{

	}

	//static�֐�
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
					L"GameManager����������Ă��܂���",
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
			//�p������
			m_ins.reset();
		}
	}

	void GameManager::ResorceLoadFunc()
	{
		mutex.lock();
		m_Loaded = false;
		mutex.unlock();

		wstring PathStr;
		App::GetApp()->GetDataDirectory(PathStr);
		XmlDocReader ResFile(PathStr + m_ResFile);
		auto ResPtr = ResFile.GetSelectNodes(L"ResourceDataMap/Resource/ResourceData");

		long count = XmlDocReader::GetLength(ResPtr);
		for (long index = 0; index < count; index++)
		{
			auto ResItem = XmlDocReader::GetItem(ResPtr, index);
			auto TypeStr = XmlDocReader::GetAttribute(ResItem, L"ResourceType");
			auto KeyStr = XmlDocReader::GetAttribute(ResItem, L"ResourceKey");
			auto FileStr = XmlDocReader::GetAttribute(ResItem, L"ResourceFile");

			if (TypeStr == L"Texture")
			{
				auto TexPath = PathStr + L"Texture/";
				App::GetApp()->RegisterTexture(KeyStr, TexPath + FileStr);
				continue;
			}
			else if (TypeStr == L"Mesh")
			{
				auto MDPath = PathStr + L"Model/";
				auto modelMesh = MeshResource::CreateStaticModelMesh(MDPath, FileStr);
				App::GetApp()->RegisterResource(KeyStr, modelMesh);
				continue;
			}
			else if (TypeStr == L"Sound")
			{
				auto SDPath = PathStr + L"Sound/";
				App::GetApp()->RegisterWav(KeyStr, SDPath + FileStr);
				continue;
			}
			else if (TypeStr == L"Effect")
			{
				auto EFKPath = PathStr + L"Effect/";
				auto MagniStr = XmlDocReader::GetAttribute(ResItem, L"EffectMagni");
				float Magni = 1.0f;
				if (MagniStr != L"")
				{
					Magni = (float)_wtof(MagniStr.c_str());
				}
				auto EfkFace = App::GetApp()->GetScene<Scene>()->GetEfkInterface();
				App::GetApp()->RegisterEffect(KeyStr, EFKPath + FileStr, EfkFace, Magni);
				continue;
			}
		}


		mutex.lock();
		m_Loaded = true;
		mutex.unlock();
	}

	//!end static

	//���\�[�X�̓Ǎ�
	void GameManager::LoadResources()
	{
		if (!m_Loaded)
		{
			std::thread Loadthread(&GameManager::ResorceLoadFunc, this);
			Loadthread.detach();
		}
	}

	//�X�e�[�W����
	void GameManager::CreateGameStage(const shared_ptr<StageBase>&StagePtr)
	{
		StageBulider Builder;

		//�Z���N�g��ʂɂ��邩
		StagePtr->CreateSharedObjectGroup(L"Rock");
		Builder.Register<FixedObj>(L"Test");//ok
		Builder.Register<LoopTexObj>(L"Floor");//ok
		Builder.Register<Player>(L"Player");//ok テストマップに書き直し必要あり
		Builder.Register<Omori>(L"Omori");//使用されてない要確認
		Builder.Register<HeatStick>(L"HeatStick");//ok
		Builder.Register<MoveFloor>(L"MoveFloor");//ok
		Builder.Register<FixedObj>(L"Himo");//上のFixedObj同様
		Builder.Register<Fountain>(L"Fountain");
		Builder.Register<GoalTest>(L"Goal");//マップにメッシュ・テクスチャ登録されておらず
		Builder.Register<SwitchObj>(L"Switch");//ok
		Builder.Register<Door>(L"Door");//ok
		Builder.Register<FireLine>(L"FireLine");//アニメーション作動時位置ずれる

		Builder.Register<FixedObj>(L"Wall");
		Builder.Register<FrontWallObj>(L"FrontWall"); //透明なテクスチャが必要
		Builder.Register<PushObj>(L"PullBox");//defaultsettingsにするとメッシュ・テクスチャでエラー起こす

		//�ɓ�:Type�̒ǉ�
		Builder.Register<Slope>(L"Slope");//ok
		Builder.Register<WaterLV>(L"Water");
		Builder.Register<WaterJet>(L"WaterJet");
		Builder.Register<UpDownBox>(L"FloatBox"); //<-����　defaultsettingsにするとメッシュ・テクスチャでエラー起こす
		Builder.Register<WaterLV>(L"WaterLV");
		Builder.Register<WaterDrop>(L"WaterDrop");
		Builder.Register<StageTest>(L"Ladder");//defaultsettingsにするとメッシュ・テクスチャでエラー起こす
		Builder.Register<StageTest>(L"Match");
		Builder.Register<StageTest>(L"FireOn");

		wstring PathStr;
		App::GetApp()->GetDataDirectory(PathStr);

		Builder.StageBuild(StagePtr, PathStr+m_MapFile);

		//�J�n�C�x���g���X�^�[�g
		auto OPCam = dynamic_pointer_cast<OpeningCamera>(StagePtr->GetOpeningView()->GetCamera());
		if (OPCam)
		{
			auto MainCamera = StagePtr->GetMainView()->GetCamera();
			auto Ptr = StagePtr->AddGameObject<OpeningCameraman>(MainCamera->GetEye(),MainCamera->GetAt());
			OPCam->SetCameraObject(Ptr);
			OPCam->SetEye(MainCamera->GetEye());
			OPCam->SetAt(MainCamera->GetAt());
			OPCam->SetFar(MainCamera->GetFar());
			OPCam->SetNear(MainCamera->GetNear());
		}
	}

	void GameManager::CreateUISet(const shared_ptr<StageBase>& StagePtr,const bool DefaultDrawActive)
	{
		StageBulider Builder;
		wstring PathStr;
		App::GetApp()->GetDataDirectory(PathStr);

		Builder.Register<NormalUI>(L"Normal");
		Builder.Register<FlashingUI>(L"Flashing");

		Builder.UISetBuild(StagePtr, PathStr + m_UISetFile,DefaultDrawActive);

	}
}
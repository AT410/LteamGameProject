#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//----------------------------------------------------------------------------
	//セーブデータ
	//----------------------------------------------------------------------------
	// -- Impl --
	struct SaveData::Impl
	{
		float m_TotalTime;
		bool m_clear[3][3];

		wstring m_FileName = L"TEST.GD";
		wstring m_DataDir;

		Impl(const wstring& FilePath)
			:m_DataDir(FilePath)
		{
			m_clear[0][0] = true;
			m_TotalTime = 0;
		}
		
		void UpdateTime(const float delta)
		{
			m_TotalTime += delta;
		}

		void Load(const wstring& FileName)
		{
			//セーブデータの読み込み
			//指定されたファイル名がなければ新規作成
			m_FileName = FileName;
			wstring FullPath = m_DataDir + m_FileName;
			if (PathFileExists(FullPath.c_str()))
			{
				//バイナリ読込
				ifstream ios(FullPath.c_str(), ios::in | ios::binary);
				
				ios.read((char*)&m_TotalTime, sizeof(float));

				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						ios.read((char*)&m_clear[i][j], sizeof(bool));
					}
				}

				ios.close();
			}
			else
			{
				//バイナリ新規
				ofstream ofs(FullPath.c_str(), ios::out | ios::binary|ios::trunc);
				if (!ofs)
				{
					throw(BaseException(L"", L"", L""));
				}
				ofs.close();
			}
		}

		void Save()
		{
			wstring FullPath = m_DataDir + m_FileName;

			ofstream ofs(FullPath.c_str(), ios::out | ios::binary);

			ofs.write((const char*)&m_TotalTime, sizeof(float));

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					ofs.write((const char*)&m_clear[i][j], sizeof(bool));
				}
			}
			ofs.close();
		}

		void Clear(int Area, int Stage)
		{
			m_clear[Area][Stage] = true;
		}

		bool AreaClear(int Area)
		{
			for (int i = 0; i < 3; i++)
			{
				if (m_clear[Area][i])
					return true;
			}
			return false;
		}

		bool StageClear(int Area,int Stage)
		{
			return m_clear[Area][Stage];
		}

		bool DataChack(const wstring&FileName, wstring& DataElem)
		{
			wstring FullPath = m_DataDir + FileName;
			if (PathFileExists(FullPath.c_str()))
			{
				//バイナリ読込
				float AllCount = 0.0f,ClearCount = 0.0f;

				ifstream ios(FullPath.c_str(), ios::in | ios::binary);

				ios.read((char*)&m_TotalTime, sizeof(float));

				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						ios.read((char*)&m_clear[i][j], sizeof(bool));
						AllCount++;
						if (m_clear[i][j])
							ClearCount++;
					}
				}

				ios.close();

				DataElem += L"累計プレイ時間:";
				DataElem += Util::FloatToWStr(m_TotalTime)+L"秒\n";
				
				DataElem += L"クリア率:";
				int Vel = (int)((ClearCount / AllCount) * 100);
				DataElem += Util::IntToWStr(Vel) + L"%\n";
				return true;
			}
			else
			{
				DataElem = L"Data Not Found!!";
				return false;
			}
		}
	};

	SaveData::SaveData(const wstring& FilePath)
		:m_pImpl(make_unique<SaveData::Impl>(FilePath))
	{

	}

	SaveData::~SaveData()
	{
		m_pImpl.reset();
	}

	void SaveData::Save()
	{
		m_pImpl->Save();
	}

	void SaveData::Load(const wstring& FileName)
	{
		m_pImpl->Load(FileName);
	}

	void SaveData::Clear()
	{
		auto pair = GameManager::GetManager()->GetStagePair();
		m_pImpl->Clear(pair.first, pair.second);
	}

	bool SaveData::IsAreaClear(int Area)
	{
		return m_pImpl->AreaClear(Area);
	}

	bool SaveData::IsStageClear(int Stage)
	{
		int Area = GameManager::GetManager()->GetStagePair().first;
		return m_pImpl->StageClear(Area, Stage);
	}

	// -- 更新処理 --
	void SaveData::Update()
	{
		float delta = App::GetApp()->GetElapsedTime();
		m_pImpl->UpdateTime(delta);
	}

	bool SaveData::DataToStr(const wstring& FileName,wstring&result)
	{
		return m_pImpl->DataChack(FileName, result);
	}

	// -- static変数実体 --
	unique_ptr<GameManager,GameManager::GMDeleter> GameManager::m_ins;

	GameManager::GameManager()
		:m_SelectStage(0, 0), m_MapFile(L"StageMap.xml"), m_ResFile(L"ResMap.xml"), m_UISetFile(L"UIMap.xml"), m_Loaded(false), m_StageReloadActive(false)
	{
		wstring DataPath;
		App::GetApp()->GetDataDirectory(DataPath);
		DataPath += L"SaveData/";
		m_Data.reset(new SaveData(DataPath));
	}

	GameManager::~GameManager()
	{

	}

	// -- インスタンスの作成 --
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

	// -- シングルトンアクセサ --
	unique_ptr<GameManager, GameManager::GMDeleter>& GameManager::GetManager()
	{
		try
		{
			if (m_ins.get() == 0)
			{
				throw(BaseException(
					L"GameManager",
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

	// -- 強制破棄 --
	void GameManager::DeleteManager()
	{
		if (m_ins.get() == 0)
		{
			m_ins.reset();
		}
	}

	// -- リソースの読み込み --
	void GameManager::ResorceLoadFunc()
	{
		mutex.lock();
		m_Loaded = false;
		mutex.unlock();

		wstring PathStr;
		App::GetApp()->GetDataDirectory(PathStr);
		XmlDocReader ResFile(PathStr + L"XMLFiles/" + m_ResFile);
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

	// -- リソース読込開始(並列実行) --
	void GameManager::LoadResources()
	{
		if (!m_Loaded)
		{
			std::thread Loadthread(&GameManager::ResorceLoadFunc, this);
			Loadthread.detach();
		}
	}

	// -- マネージャーの更新 --
	void GameManager::UpdateManager()
	{

		// -- ポーズ中はぬける --
		if (m_UpdateActive)
		{
			m_Data->Update();
		}
	}

	// -- ゲームステージの生成 --
	void GameManager::CreateGameStage(const shared_ptr<StageBase>&StagePtr)
	{
		StageBuilder Builder;

		Builder.Register<FrontWallObj>(L"FrontWall"); 
		Builder.Register<LoopTexObj>(L"Floor");
		Builder.Register<MoveFloor>(L"MoveFloor");
		Builder.Register<SwitchObj>(L"Switch");
		Builder.Register<UpDownBox>(L"FloatBox"); 
		Builder.Register<Fountain>(L"Fountain");
		Builder.Register<FireLine>(L"FireLine");

		Builder.Register<WaterDrop>(L"WaterDrop");
		Builder.Register<WaterLV2>(L"Water");
		Builder.Register<WaterLV2>(L"WaterLV");
		Builder.Register<WaterJet>(L"WaterJet");

		Builder.Register<FixedObj>(L"Test");
		Builder.Register<FixedObj>(L"Himo");
		Builder.Register<FixedObj>(L"Wall");
		Builder.Register<FixedObj>(L"Ladder");
		Builder.Register<FixedObj>(L"Match");
		Builder.Register<PushObj>(L"PullBox");
		Builder.Register<Player>(L"Player");
		Builder.Register<Slope>(L"Slope");
		Builder.Register<Door>(L"Door");
		Builder.Register<Goal>(L"Goal");


		wstring PathStr;
		App::GetApp()->GetDataDirectory(PathStr);
		PathStr += L"XMLFiles/";
		Builder.StageBuild(StagePtr, PathStr+m_MapFile);

		auto OPCam = dynamic_pointer_cast<OpeningCamera>(StagePtr->GetOpeningView()->GetCamera());
		if (OPCam)
		{
			auto MainCamera = StagePtr->GetMainView()->GetCamera();
			auto Ptr = StagePtr->AddGameObject<OpeningCameraman>(MainCamera->GetEye(), MainCamera->GetAt());

			OPCam->SetCameraObject(Ptr);
			OPCam->SetEye(MainCamera->GetEye());
			OPCam->SetAt(MainCamera->GetAt());
			OPCam->SetFar(MainCamera->GetFar());
			OPCam->SetNear(MainCamera->GetNear());
			if (m_StageReloadActive && m_StartCametaEnd)
			{
				auto GameStagePtr = dynamic_pointer_cast<GameStage>(StagePtr);
				if (GameStagePtr)
				{
					GameStagePtr->ToReStart();
				}
			}
		}
	}

	// -- UIセットの生成--
	void GameManager::CreateUISet(const shared_ptr<StageBase>& StagePtr,const bool DefaultDrawActive)
	{
		wstring PathStr;
		App::GetApp()->GetDataDirectory(PathStr);
		PathStr += L"XMLFiles/";

		StageBuilder Builder;
		Builder.Register<NormalUI>(L"Normal");
		Builder.Register<FlashingUI>(L"Flashing");

		Builder.UISetBuild(StagePtr, PathStr + m_UISetFile,DefaultDrawActive);

	}
}
/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ステージベースクラス実体
	//--------------------------------------------------------------------------------------
	void StageBase::CreateViewLight() {
		const Vec3 eye(7.0f, 10.0f, -20.0f);
		const Vec3 at(7.0f,2.0f,0.0f);
		m_MainView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		m_MainView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);

		m_OpeningView = CreateView<SingleView>();
		auto PtrOPCamera = ObjectFactory::Create<OpeningCamera>();
		m_OpeningView->SetCamera(PtrOPCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);

		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void StageBase::SetBGM(const wstring& BGMKey)
	{
		auto XAudioPtr = App::GetApp()->GetXAudio2Manager();
		m_BGMPtr = XAudioPtr->Start(BGMKey,XAUDIO2_LOOP_INFINITE,0.5f);
	}

	void StageBase::OnDestroy()
	{
		//BGMを終了
		auto XAudioPtr = App::GetApp()->GetXAudio2Manager();
		XAudioPtr->Stop(m_BGMPtr);
	}

	//--------------------------------------------------------------------------------------
	//	タイトルステージクラス実体
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
	//	タイトルステージクラス実体
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
	//	タイトルステージクラス実体
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
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------

	void GameStage::ToMyCamera()
	{
		this->SetView(m_MainView);
		auto Pair = GameManager::GetManager()->GetStagePair();
		AddGameObject<AnimSpriteTest>(Pair.second);
	}

	void GameStage::OnCreate()
	{
		try
		{
			CreateViewLight();
			wstring Test;
			App::GetApp()->GetDataDirectory(Test);
			GameManager::GetManager()->CreateStage(GetThis<StageBase>(), Test + L"MapData.xml",false);
			//AddGameObject<DebugSprite>(L"GameStage_TX");
			//AddGameObject<MoveObj>(Vec3(0), Vec3(0), Vec3(1), L"TEST_TX", L"DEFAULT_CUBE", Vec3(2, 0, 0), Vec3(-2, 0, 0), 2.5f, 10.0f);

			////物理計算有効
			//SetPhysicsActive(true);

			////（仮）プレイヤー
			//auto PlayerPtr = AddGameObject<Player>();

			//////ひも
			//auto HimoPtr = AddGameObject<FixedObj>(Vec3(4, 5, 0), Vec3(0), Vec3(0.5, 2, 0.5), L"TEST_TX", L"DEFAULT_CUBE");
			////ひもの登録
			//SetSharedGameObject(L"Himo", HimoPtr);
			//////おもり
			//auto OmoriPtr = AddGameObject<Omori>(Vec3(4, 3.5, 0), Vec3(0), Vec3(1), L"TEST_TX", L"DEFAULT_CUBE");

			//////熱棒
			//AddGameObject<HeatStick>(Vec3(0, 6, 0), Vec3(0), Vec3(10, 1, 1), L"TEST_TX", L"DEFAULT_CUBE");

			////ボタン
			//AddGameObject<SwitchObj>(Vec3(4, 1, 0), Vec3(0), Vec3(2, 1, 2), L"TEST_TX", L"DEFAULT_CUBE");

			////床(左)
			//AddGameObject<FixedObj>(Vec3(-8, 0, 0), Vec3(0), Vec3(10, 1, 15), L"TEST_TX", L"DEFAULT_CUBE");
			////床(右)
			//AddGameObject<FixedObj>(Vec3(8, 0, 0), Vec3(0), Vec3(10, 1, 15), L"TEST_TX", L"DEFAULT_CUBE");
			////床(下)
			//AddGameObject<FixedObj>(Vec3(0, -4, 0), Vec3(0), Vec3(20, 1, 15), L"TEST_TX", L"DEFAULT_CUBE");

			SetBGM(L"TEST_SD");

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
	//	タイトルステージクラス実体
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

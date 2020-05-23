/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void GameStage::CreateViewLight() {
		const Vec3 eye(0.0f, 10.0f, -20.0f);
		const Vec3 at(0.0f);
		//auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		//auto PtrCamera = ObjectFactory::Create<Camera>();
		m_MyCameraView = CreateView<SingleView>();
		auto ptrMyCamera = ObjectFactory::Create<MyCamera>();
		ptrMyCamera->SetEye(eye);
		ptrMyCamera->SetExpansionEye(eye);
		ptrMyCamera->SetAt(at);
		ptrMyCamera->SetExpansionAt(at);
		m_MyCameraView->SetCamera(ptrMyCamera);
		//PtrView->SetCamera(PtrCam);
		//PtrCamera->SetEye(eye);
		//PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void GameStage::GamePlayer() {
		auto playerptr = AddGameObject<Player>();
		SetSharedGameObject(L"Player", playerptr);
	}

	void GameStage::Object() {
		auto ptrPlayer = GetSharedGameObject<Player>(L"Player");
		auto ptrMyCamera = dynamic_pointer_cast<MyCamera>(m_MyCameraView->GetCamera());
		ptrMyCamera->SetTargetObject(ptrPlayer);
	}
	void GameStage::OnCreate() {
		try {
			SetPhysicsActive(true);
			//ビューとライトの作成
			CreateViewLight();
			GamePlayer();
			Object();
			

			AddGameObject<WaterJet>();
			AddGameObject<WaterLV>();
			AddGameObject<WaterDrop>();
			AddGameObject<UpDownBox>();
		AddGameObject<FixBox>(
			Vec3(30.0f, 0.1f, 10.0f),
			Vec3(0.0f),
			Vec3(0.0f, -1.0f, 0.0f)
			);
		AddGameObject<FixBox>(
			Vec3(2.0f, 1.0f, 3.0f),
			Vec3(0.0f),
			Vec3(2.0f, -0.5f, 0.0f)
			);
		AddGameObject<ReturnCube>(
			Vec3(0.5f),
			Vec3(0.0f),
			Vec3(5.0f, -1.0f, 0.0f)
			);
		AddGameObject<PushPullObj>(
			Vec3(0.3f),
			Vec3(0.0f),
			Vec3(-1.0f, -0.7f, 0.0f)
			);


			AddGameObject<DebugTest>();
			
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void GameStageHurukawa::CreateViewLight() {
		const Vec3 eye(0.0f, 10.0f, -60.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void GameStageHurukawa::OnCreate() {
		try {

			auto& app = App::GetApp(); // ゲームアプリ全体に関わるオブジェクト

			auto path = app->GetDataDirWString();
			app->RegisterTexture(L"iseki", path + L"Texture/iseki.jpg");
			app->RegisterTexture(L"kabe", path + L"Texture/kabe.jpg");
			app->RegisterTexture(L"button", path + L"Texture/button.png");
			app->RegisterTexture(L"gate", path + L"Texture/tobira.jpg");
			app->RegisterTexture(L"mizuguruma", path + L"Texture/mizuguruma.jpg");
			app->RegisterTexture(L"isi", path + L"Texture/isi.jpg");
			app->RegisterTexture(L"maguma", path + L"Texture/red.jpg");
			SetPhysicsActive(true);
			//ビューとライトの作成
			CreateViewLight();
			//Floorの追加
			AddGameObject<Floor2>
				(
					Vec3(50.0f, 1.0f, 50.0f),//sacle x,y,z
					Vec3(0.0f),//rotate x,y,z
					Vec3(0.0f, -0.5f, 0.0f)//position x,y,zd
					);
			AddGameObject<WaterWheel>
				(
					Vec3(5.0f, 5.0f, 5.0f),//sacle x,y,z
					Vec3(0.0f),//rotate x,y,z
					Vec3(10.0f, 15.0f, 0.0f)//position x,y,z
					);
			auto gateptr = AddGameObject<Gate>
				(
					Vec3(1.0f, 20.0f, 25.0f),//sacle x,y,z
					Vec3(0.0f, 0, 0),//rotate x,y,z
					Vec3(-5.0f, 10.0f, -12.5f)//position x,y,zd
					);

			auto gate2ptr = AddGameObject<Gate2>
				(
					Vec3(1.0f, 20.0f, 25.0f),//sacle x,y,z
					Vec3(0.0f, 0, 0),//rotate x,y,z
					Vec3(-5.0f, 10.0f, 0.0f)//position x,y,zd
					);
			//Playerの追加
			AddGameObject<Playerhurukawa>
				(
					Vec3(1.0f, 3.0f, 1.0f),//sacle x,y,z
					Vec3(0.0f),//rotate x,y,z
					Vec3(-20.0f, +0.5f, 0.0f)//position x,y,z
					);

			auto gimmickptr = AddGameObject<Gimmick>
				(
					Vec3(4.0f, 1.0f, 3.0f),//sacle x,y,z
					Vec3(0.0f),//rotate x,y,z
					Vec3(10.0f, 1.0f, 0.0f)//position x,y,zd
					);
			SetSharedGameObject(L"gimmickobj", gimmickptr);
			SetSharedGameObject(L"gate", gateptr);
			SetSharedGameObject(L"gate2", gate2ptr);
			AddGameObject<Button2>
				(
					Vec3(1.0f, 1.0f, 1.0f),
					Vec3(0.0f),
					Vec3(1.0f, 0.0f, 1.0f)
					);
			AddGameObject<Button22>
				(
					Vec3(1.0f, 1.0f, 1.0f),
					Vec3(0.0f),
					Vec3(-10.0f, 0.0f, 1.0f)
					);
			AddGameObject<Background>
				(
					Vec3(50.0f, 50.0f, 0.5f),
					Vec3(0.0f),
					Vec3(0.0f, 10.0f, 10.0f)
					);
			AddGameObject<Valve>
				(
					Vec3(3.0f, 1.0f, 3.0f),
					Vec3(0.0f),
					Vec3(-10.0f, 10.0f, -25.0f)
					);
			auto Valvegateptr = AddGameObject<ValveGate>
				(
					Vec3(1.0f, 20.0f, 25.0f),//sacle x,y,z
					Vec3(0.0f, 0, 0),//rotate x,y,z
					Vec3(15.0f, 10.0f, -12.5f)//position x,y,zd
					);

			auto Valvegate2ptr = AddGameObject<ValveGate2>
				(
					Vec3(1.0f, 20.0f, 25.0f),//sacle x,y,z
					Vec3(0.0f, 0, 0),//rotate x,y,z
					Vec3(15.0f, 10.0f, 0.0f)//position x,y,zd
					);
			SetSharedGameObject(L"Valvegate", Valvegateptr);
			SetSharedGameObject(L"Valvegate2", Valvegate2ptr);
		}
		catch (...) {
			throw;
		}
	}

	//熱棒のステージ
	void GameStageShogo::CreateViewLight() {
		const Vec3 eye(0.0f, 10.0f, -20.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void GameStageShogo::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();

			//物理計算有効
			SetPhysicsActive(true);

			//（仮）プレイヤー
			auto PlayerPtr = AddGameObject<TestPlayer>(Vec3(-7, 1, 0), Vec3(0), Vec3(1));
			//プレイヤーの登録
			SetSharedGameObject(L"Player", PlayerPtr);

			//燃焼物
			auto CombustionPtr = AddGameObject<Combustion>(Vec3(-4, 2, 0), Vec3(0), Vec3(4, 1, 1));
			//燃焼物の登録
			SetSharedGameObject(L"Combustion", CombustionPtr);

			//ひも
			auto HimoPtr = AddGameObject<Himo>(Vec3(4, 5, 0), Vec3(0), Vec3(0.5, 2, 0.5));
			//ひもの登録
			SetSharedGameObject(L"Himo", HimoPtr);
			//おもり
			auto OmoriPtr = AddGameObject<Omori>(Vec3(4, 3.5, 0), Vec3(0), Vec3(1));
			//おもりの登録
			SetSharedGameObject(L"Omori", OmoriPtr);
			//熱棒
			AddGameObject<HeatStick>(Vec3(0, 6, 0), Vec3(0), Vec3(10, 1, 1));

			//ボタン
			AddGameObject<Button>(Vec3(4, 1, 0), Vec3(0), Vec3(2, 1, 2));

			//床(左)
			AddGameObject<Floor>(Vec3(-8, 0, 0), Vec3(0), Vec3(10, 1, 15));
			//床(右)
			AddGameObject<Floor>(Vec3(8, 0, 0), Vec3(0), Vec3(10, 1, 15));
			//床(下)
			AddGameObject<Floor>(Vec3(0, -4, 0), Vec3(0), Vec3(20, 1, 15));

			//床(ナナメ動く)
			auto MoveFloorPtr1 = AddGameObject<MoveFloor>(Vec3(0, 0.01f, 5), Vec3(0, 0, -45), Vec3(8, 1, 3));
			//動く床登録
			SetSharedGameObject(L"MoveFloor1", MoveFloorPtr1);
			//床(ヨコ動く)
			auto MoveFloorPtr2 = AddGameObject<MoveFloor>(Vec3(8, -0.5f, -5), Vec3(0), Vec3(8, 1, 3));
			//動く床登録
			SetSharedGameObject(L"MoveFloor2", MoveFloorPtr2);

			//噴水（ポジション、スケール）
			auto Fountain1 = AddGameObject<Fountain>(Vec3(0.0f, -3.0f, 0.0f), Vec3(1.0f));
			//噴水
			SetSharedGameObject(L"Fountain1", Fountain1);

			//点火先
			auto FireOnPtr = AddGameObject<FireOn>(Vec3(-4, 1, -3), Vec3(0), Vec3(1));
			SetSharedGameObject(L"FireOn", FireOnPtr);
			//導火線
			auto FireLinePtr = AddGameObject<FireLine>(Vec3(0, 2, -3), Vec3(0), Vec3(8, 1, 1));
			SetSharedGameObject(L"FireLine", FireLinePtr);
			//ボム
			auto BombPtr = AddGameObject<Bomb>(Vec3(4, 1, -3), Vec3(0), Vec3(1));
			SetSharedGameObject(L"Bomb", BombPtr);

		}
		catch (...) {
			throw;
		}


	}

	int state = 0;
	void Stagechoice::OnCreate()
	{
		
		try
		{
			CreateViewLight();
			CreateSharedObjectGroup(L"UIGroup");
			auto& app = App::GetApp(); // ゲームアプリ全体に関わるオブジェクト
			auto path = app->GetDataDirWString();
			app->RegisterTexture(L"isi", path + L"Texture/isi.jpg");
			app->RegisterTexture(L"iseki", path + L"Texture/iseki.jpg");
			app->RegisterTexture(L"kabe", path + L"Texture/kabe.jpg");
			app->RegisterTexture(L"sima", path + L"Texture/sima.jpg");
			app->RegisterTexture(L"maguma", path + L"Texture/red.jpg");
			auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
			if (KeyState.m_bPushKeyTbl[VK_DOWN])
			{
				state + 1;
			}
			if (state == 0)
			{
				CreateTraceSprite();
			}
			if (state == 1)
			{
				CreateTraceSprite2();
			}
			if (state == 2)
			{
				CreateTraceSprite3();
			}
			if (state == 3)
			{
				CreateTraceSprite4();
			}
		}
		catch (...) 
		{
			throw;
		}
	}
	void Stagechoice::OnUpdate()
	{
		
	}
	void Stagechoice::CreateTraceSprite()
	{
		float Sizex = (float)App::GetApp()->GetGameWidth() ;
		float Sizey = (float)App::GetApp()->GetGameHeight() ;
		AddGameObject<TraceSprite>(true,
			Vec2(Sizex, Sizey), Vec3(-250, 200.0f, 0.0f));
	}
	void Stagechoice::CreateTraceSprite2()
	{
		float Sizex = (float)App::GetApp()->GetGameWidth();
		float Sizey = (float)App::GetApp()->GetGameHeight();
		AddGameObject<TraceSprite2>(true,
			Vec2(Sizex, Sizey), Vec3(-250, -200, 0.0f));
	}
	void Stagechoice::CreateTraceSprite3()
	{
		float Sizex = (float)App::GetApp()->GetGameWidth();
		float Sizey = (float)App::GetApp()->GetGameHeight();
		AddGameObject<TraceSprite3>(true,
			Vec2(Sizex, Sizey), Vec3(250, 200, 0.0f));
	}
	void Stagechoice::CreateTraceSprite4()
	{
		float Sizex = (float)App::GetApp()->GetGameWidth();
		float Sizey = (float)App::GetApp()->GetGameHeight();
		AddGameObject<TraceSprite4>(true,
			Vec2(Sizex, Sizey), Vec3(250, -200, 0.0f));
	}



	void Stagechoice::CreateViewLight() {
		const Vec3 eye(0.0f, 10.0f, -60.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}



	void SelectStage::CreateViewLight()
	{
		const Vec3 eye(0.0f, 10.0f, -60.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void SelectStage::CreateTraceSprite2()
	{
		float Sizex = (float)App::GetApp()->GetGameWidth();
		float Sizey = (float)App::GetApp()->GetGameHeight();
		AddGameObject<SelectSprite>(true,
			Vec2(Sizex, Sizey), Vec3(0.0f, 0.0f, 0.0f));
	}

	void SelectStage::OnCreate()
	{
		try
		{
			auto& app = App::GetApp(); // ゲームアプリ全体に関わるオブジェクト
			auto path = app->GetDataDirWString();
			app->RegisterTexture(L"isi", path + L"Texture/isi.jpg");
			app->RegisterTexture(L"iseki", path + L"Texture/iseki.jpg");
			CreateViewLight();
			CreateTraceSprite2();
			AddGameObject<Select>();
		}

		catch (...)
		{
			throw;
		}
	}


	void TitleStage::CreateTitleSprite()
	{
		float Sizex = (float)App::GetApp()->GetGameWidth();
		float Sizey = (float)App::GetApp()->GetGameHeight();
		AddGameObject<TitleSprite>(true,
			Vec2(Sizex, Sizey), Vec3(0.0f, 0.0f, 0.0f));
	}


	void TitleStage::OnCreate()
	{
		try {

			auto& app = App::GetApp(); // ゲームアプリ全体に関わるオブジェクト

			auto path = app->GetDataDirWString();
			app->RegisterTexture(L"iseki", path + L"Texture/iseki.jpg");
			app->RegisterTexture(L"kabe", path + L"Texture/kabe.jpg");
			app->RegisterTexture(L"button", path + L"Texture/button.png");
			app->RegisterTexture(L"gate", path + L"Texture/tobira.jpg");
			app->RegisterTexture(L"mizuguruma", path + L"Texture/mizuguruma.jpg");
			app->RegisterTexture(L"isi", path + L"Texture/isi.jpg");
			app->RegisterTexture(L"maguma", path + L"Texture/red.jpg");
			SetPhysicsActive(true);
			//ビューとライトの作成
			CreateViewLight();
			CreateTitleSprite();
		}
		catch (...)
		{
			throw;
		}
	}
	void TitleStage::CreateViewLight()
	{
		const Vec3 eye(0.0f, 10.0f, -60.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}
}
//end basecross

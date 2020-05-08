/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//燃焼物
	void Combustion::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();

	}
	void Combustion::OnUpdate()
	{
		if (m_Active) {
			m_Time += App::GetApp()->GetElapsedTime();
			if (m_Time > 10.0f) {
				m_Active = false;
			}
			auto ptrTrans = GetComponent<Transform>();
			m_Scale = ptrTrans->GetScale();
			m_Pos = ptrTrans->GetPosition();

			//横に長い燃焼物
			if (m_Scale.x > 0) {
				//火のエフェクト
				m_Scale.x += -0.05;
				m_Pos.x += 0.025;
				if (m_Scale.x <= 0) {
					//煙のエフェクト
					GetStage()->RemoveGameObject<Combustion>(GetThis<Combustion>());
				}
			}
			////縦に長い燃焼物
			//if (m_Scale.y < 0) {
			//	//火のエフェクト
			//	m_Scale.y += -0.05;
			//	m_Pos.y += -0.025;
			//	if (m_Scale.y <= 0) {
			//		//煙のエフェクト
			//		GetStage()->RemoveGameObject<Combustion>(GetThis<Combustion>());
			//	}
			//}
			ptrTrans->SetScale(m_Scale);
			ptrTrans->SetPosition(m_Pos);
		}
	}
	void Combustion::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		//プレイヤーの登録
		auto ObjectPtr_P = GetStage()->GetSharedGameObject<TestPlayer>(L"Player");
		//燃焼物の登録
		auto ObjectPtr_C = GetStage()->GetSharedGameObject<Combustion>(L"Combustion");

		//実行時間計測
		auto elapsedTime = App::GetApp()->GetElapsedTime();

		//Playerタグを持った相手と衝突した時
		if (Other->FindTag(L"Player")) {
			ObjectPtr_C->SetActive(true);
		}
	}

	//熱棒
	void HeatStick::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);

		//熱棒のエリア-5、-1、０からポジションまで
		m_HeatArea = AABB(Vec3(-5, -1, 0), m_Pos);

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);

		//ひもの登録
		m_himo = GetStage()->GetSharedGameObject<Himo>(L"Himo");
	}
	void HeatStick::OnUpdate()
	{
		//プレイヤーの登録
		auto ObjectPtr_P = GetStage()->GetSharedGameObject<TestPlayer>(L"Player");

		auto stage = GetStage();

		//おもりの登録
		auto ObjectPtr_O = GetStage()->GetSharedGameObject<Omori>(L"Omori");

		//実行時間計測
		auto elapsedTime = App::GetApp()->GetElapsedTime();

		if (ObjectPtr_P) {
			//プレイヤーのポジションを取得
			auto PlayerPos = ObjectPtr_P->GetComponent<Transform>()->GetPosition();
			//重りの重力
			auto OmoriGrav = ObjectPtr_O->GetComponent<Gravity>();

			//プレイヤーの中心から１，１，１の範囲
			AABB PlayerAABB = AABB(PlayerPos, 1, 1, 1);
			//プレイヤーが熱棒の範囲に入ったら
			if (HitTest::AABB_AABB(m_HeatArea, PlayerAABB)) {
				count++;
				//約５秒後に消す
				if (count >= 250) {
					if (m_himo) {
						//ひもを消す
						stage->RemoveGameObject<GameObject>(m_himo->GetThis<GameObject>());
						//重りの重力をオンにする
						OmoriGrav->SetUpdateActive(true);
					}
				}
			}
		}
	}
	//（仮）プレイヤー
	void TestPlayer::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CAPSULE");

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);
		//重力
		auto grav = AddComponent<Gravity>();
		//タグをつける
		AddTag(L"Player");

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
	}
	void TestPlayer::OnUpdate()
	{
		auto transComp = GetComponent<Transform>();
		Vec3 pos = transComp->GetPosition();
		//実行時間計測		
		auto elapsedTime = App::GetApp()->GetElapsedTime();
		float speed = 2.0f;
		//キーボードの取得(キーボード優先)
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (KeyState.m_bPushKeyTbl['W']) {
			//前
			pos.z += speed * elapsedTime;
		}
		if (KeyState.m_bPushKeyTbl['A']) {
			//左
			pos.x += -speed * elapsedTime;
		}
		if (KeyState.m_bPushKeyTbl['S']) {
			//後ろ
			pos.z += -speed * elapsedTime;
		}
		if (KeyState.m_bPushKeyTbl['D']) {
			//右
			pos.x += speed * elapsedTime;
		}
		if (KeyState.m_bLastKeyTbl[VK_SPACE]) {
			//上
			pos.y += 5.0f* elapsedTime;
		}
		if (KeyState.m_bLastKeyTbl[VK_LSHIFT]) {
			//下
			pos.y += -1.0f* elapsedTime;
		}
		transComp->SetPosition(pos);

	}
	//ひも
	void Himo::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}
	//重り
	void Omori::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();

		//重力
		auto grav = AddComponent<Gravity>();
		//無効にしておく
		grav->SetUpdateActive(false);

		//タグ
		AddTag(L"Omori");
	}
	//ボタン
	void Button::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
	}
	void Button::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		//Omoriタグを持った相手と衝突した
		if (Other->FindTag(L"Omori")) {
			//ナナメ動く床
			auto ObjectPtr_MF = GetStage()->GetSharedGameObject<MoveFloor>(L"MoveFloor1");
			auto transComp = ObjectPtr_MF->GetComponent<Transform>();
			Vec3 pos = transComp->GetPosition();
			Vec3 rot = transComp->GetRotation();

			//実行時間計測		
			auto elapsedTime = App::GetApp()->GetElapsedTime();

			ObjectPtr_MF->SetActive(true);
		}
	}
	void Button::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		//Omoriタグを持った相手と衝突した
		if (Other->FindTag(L"Omori")) {
			//ヨコ動く床
			auto ObjectPtr_MF = GetStage()->GetSharedGameObject<MoveFloor>(L"MoveFloor2");
			auto transComp = ObjectPtr_MF->GetComponent<Transform>();
			Vec3 pos = transComp->GetPosition();
			Vec3 rot = transComp->GetRotation();

			//実行時間計測		
			auto elapsedTime = App::GetApp()->GetElapsedTime();

			if (pos.x > 0) {
				pos.x += -2.0f * elapsedTime;
				transComp->SetPosition(pos);
			}
		}
	}

	//床
	void Floor::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}
	//動く床
	void MoveFloor::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);
		//原点の座標を変更
		transComp->SetPivot(3, 0.1f, 5);

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}
	void MoveFloor::OnUpdate()
	{
		if (m_Active)
		{
			m_Time += App::GetApp()->GetElapsedTime();
			if (m_Time > 5.0f) {
				m_Active = false;
			}
			auto ptrTrans = GetComponent<Transform>();
			m_Rotation = ptrTrans->GetRotation();
			Easing<Vec3> easing;
			auto Force = easing.Linear(m_Rotation, Vec3(0, 0, 0), m_Time, 5.0f);
			const float LerpFact = 1.0f;
			bsm::Quat FQuat;
			FQuat.rotationRollPitchYawFromVector(Force);
			FQuat.normalize();
			ptrTrans->SetQuaternion(FQuat);
		}
	}

	//噴水
	void Fountain::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
		//ptrColl->SetFixed(true);

		//重力
		auto grav = AddComponent<Gravity>();

		//噴水エリアからポジションまで
		m_FountainArea = AABB(Vec3(0, -2, 0), 6, 1, 14);
		m_FountainArea2 = AABB(Vec3(0, 3, 0), 5, 6, 14);

	}

	void Fountain::OnUpdate()
	{
		auto transComp = GetComponent<Transform>();
		Vec3 pos = transComp->GetPosition();

		//プレイヤーの登録
		auto ObjectPtr_P = GetStage()->GetSharedGameObject<TestPlayer>(L"Player");
		auto stage = GetStage();

		if (ObjectPtr_P) {
			//プレイヤーのポジションを取得
			auto PlayerPos = ObjectPtr_P->GetComponent<Transform>()->GetPosition();
			//プレイヤーの中心から１，１，１の範囲
			AABB PlayerAABB = AABB(PlayerPos, 1, 1, 1);
			if (HitTest::AABB_AABB(m_FountainArea, PlayerAABB)) {
				//プレイヤーのポジションをスタートに
				PlayerPos = Vec3(-7, 2, 0);
				ObjectPtr_P->GetComponent<Transform>()->SetPosition(PlayerPos);

				//プレイヤーを消す
				//ObjectPtr_P->SetDrawActive(false);
			}
			if (m_Active) {
				m_Time += App::GetApp()->GetElapsedTime();
				if (m_Time > 5.0f) {
					m_Active = false;
					AddComponent<Gravity>()->StartJump(Vec3(0, 12.5f, 0.0f));
					m_Time = 0;
				}
			}
			else {
				m_Time += App::GetApp()->GetElapsedTime();
				if (m_Time > 1.5f) {
					if (HitTest::AABB_AABB(m_FountainArea2, PlayerAABB)) {
						//プレイヤーのポジションをスタートに
						PlayerPos = Vec3(-7, 2, 0);
						ObjectPtr_P->GetComponent<Transform>()->SetPosition(PlayerPos);

						//プレイヤーを消す
						//ObjectPtr_P->SetDrawActive(false);
					}
				}
			}
		}
	}

	void Fountain::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		auto ObjectPtr_F = GetStage()->GetSharedGameObject<Fountain>(L"Fountain1");

		ObjectPtr_F->SetActive(true);

	}


	//導火線
	void FireLine::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}
	void FireLine::OnUpdate()
	{
		if (m_Active)
		{
			m_Time += App::GetApp()->GetElapsedTime();
			if (m_Time > 10.0f) {
				m_Active = false;
			}
			auto ptrTrans = GetComponent<Transform>();
			m_Scale = ptrTrans->GetScale();
			m_Pos = ptrTrans->GetPosition();

			if (m_Scale.x > 0) {
				m_Scale.x += -0.05;
				m_Pos.x += 0.025;
			}
			else {
				GetStage()->RemoveGameObject<FireLine>(GetThis<FireLine>());
			}
			ptrTrans->SetScale(m_Scale);
			ptrTrans->SetPosition(m_Pos);

		}
	}

	//点火先
	void FireOn::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
	}
	void FireOn::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		//Playerタグを持った相手と衝突した
		if (Other->FindTag(L"Player")) {
			if (GetStage()->GetSharedGameObject<FireOn>(L"FireOn")) {
				//点火先を消す
				GetStage()->RemoveGameObject<FireOn>(GetThis<FireOn>());

				//短くなる導火線
				auto ObjectPtr_FL = GetStage()->GetSharedGameObject<FireLine>(L"FireLine");
				ObjectPtr_FL->SetActive(true);

				//ボム
				auto ObjectPtr_B = GetStage()->GetSharedGameObject<Bomb>(L"Bomb");
				ObjectPtr_B->SetActive(true);
				
			}
		}
	}

	//ボム
	void Bomb::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);
		
		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}
	void Bomb::OnUpdate()
	{
		if (m_Active) {
			m_Time += App::GetApp()->GetElapsedTime();
			if (m_Time >= 6) {
				auto drawComp = AddComponent<PNTStaticDraw>();
				drawComp->SetEmissive(Col4(1.0, 0, 0, 0));
			}
		}
	}

}
//end basecross

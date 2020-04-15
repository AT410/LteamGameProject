/*!
@breif	ギミック関連実装
@name	作成者：阿部達哉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	///スイッチオブジェクト
	void SwitchObj::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(m_meshKey);
		drawComp->SetTextureResource(m_texKey);

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_pos);
		transComp->SetScale(m_scal);
		transComp->SetQuaternion(Quat(m_rot));

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetAfterCollision(AfterCollision::Auto);
	}

	void SwitchObj::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		//Omoriタグを持った相手と衝突した
		if (Other->FindTag(L"EnabledSwitch")) {
			//スイッチが押された時の判定
			//GetStage()->RemoveGameObject<GameObject>(Other);
			auto ObjectPtr_MF = GetStage()->GetSharedGameObject<MoveFloor>(L"MoveFloor");
			ObjectPtr_MF->SetActive(true);
		}
	}

	void SwitchObj::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		if (!m_IsKeep)
			return;
		//Omoriタグを持った相手と衝突した
		if (Other->FindTag(L"EnabledSwitch")) {
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

	void SwitchObj::OnCollisionExit(shared_ptr<GameObject>& Other) {
		if (!m_IsKeep)
			return;
		if (Other->FindTag(L"EnabledSwitch")) {
			m_Active = m_IsKeep ? false : m_Active;
		}
	}


	//熱棒
	void HeatStick::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_pos);
		transComp->SetScale(m_scal);
		transComp->SetQuaternion(Quat(m_rot));

		//熱棒のエリア-5、-1、０からポジションまで
		//可変可能に修正
		m_HeatArea = AABB(Vec3(-3, -1, 0), m_pos);

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}


	void HeatStick::OnUpdate()
	{
		if (m_player) {
			//プレイヤーのポジションを取得
			auto PlayerPos = m_player->GetComponent<Transform>()->GetPosition();

			//プレイヤーの中心から１，１，１の範囲
			AABB PlayerAABB = AABB(PlayerPos, 1, 1, 1);
			//プレイヤーが熱棒の範囲に入ったら
			if (HitTest::AABB_AABB(m_HeatArea, PlayerAABB)) {
				//実行時間計測
				auto elapsedTime = App::GetApp()->GetElapsedTime();

				m_count += elapsedTime;
				//約５秒後に消す
				if (m_count >= 5.0f) {
					if (m_himo) {
						//ひもを消す
						GetStage()->RemoveGameObject<GameObject>(m_himo->GetThis<GameObject>());
						//重りの重力をオンにする
					//重りの重力
					//おもりの登録
					auto ObjectPtr_O = GetStage()->GetSharedGameObject<ObjectBase>(L"Omori");

					auto OmoriGrav = ObjectPtr_O->GetComponent<Gravity>();
						OmoriGrav->SetUpdateActive(true);
					}
				}
			}
		}
		else
		{
			//ひもの登録
			m_himo = GetStage()->GetSharedGameObject<FixedObj>(L"Himo");
			m_player = GetStage()->GetSharedGameObject<Player>(L"Player");
		}
	}


	//重り
	void Omori::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(m_meshKey);
		drawComp->SetTextureResource(m_texKey);

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_pos);
		transComp->SetScale(m_scal);
		transComp->SetQuaternion(Quat(m_rot));

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetAfterCollision(AfterCollision::None);

		//重力
		auto grav = AddComponent<Gravity>();
		//無効にしておく
		grav->SetUpdateActive(false);

		//タグ
		for (auto Tag : m_tag) {
			AddTag(Tag);
		}

		//AddTag(L"EnabledSwitch");
		//共有設定
		GetStage()->SetSharedGameObject(L"Omori", GetThis<Omori>());
	}

	void Omori::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		GetComponent<Gravity>()->SetGravityZero();
	}


	//動く床
	void MoveFloor::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(m_meshKey);
		drawComp->SetTextureResource(m_texKey);

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_pos);
		transComp->SetScale(m_scal);
		transComp->SetQuaternion(Quat(m_rot));
		//原点の座標を変更
		transComp->SetPivot(3, 0.1f, 5);

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);

		if (m_SharedActive)
		{
			GetStage()->SetSharedGameObject(m_SharedName, GetThis<MoveFloor>());
		}
	}
	void MoveFloor::OnUpdate()
	{
		if (m_Active)
		{
			auto transComp = GetComponent<Transform>();
			Vec3 pos = transComp->GetPosition();
			Vec3 rot = transComp->GetRotation();

			//実行時間計測		
			auto elapsedTime = App::GetApp()->GetElapsedTime();

			if (pos.x > 0) {
				pos.x += -2.0f * elapsedTime;
				transComp->SetPosition(pos);
			}

			//m_Time += App::GetApp()->GetElapsedTime();
			//if (m_Time > 5.0f) {
			//	m_Active = false;
			//}
			//auto ptrTrans = GetComponent<Transform>();
			//auto Rotation = ptrTrans->GetRotation();
			//Easing<Vec3> easing;
			//auto Force = easing.Linear(Rotation, Vec3(0, 0, 0), m_Time, 5.0f);
			//const float LerpFact = 1.0f;
			//bsm::Quat FQuat;
			//FQuat.rotationRollPitchYawFromVector(Force);
			//FQuat.normalize();
			//ptrTrans->SetQuaternion(FQuat);
		}
	}


	//噴水
	void Fountain::OnCreate()
	{
		//描画
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_pos);
		transComp->SetScale(m_scal);

		//コリジョンを付ける
		auto ptrColl = AddComponent<CollisionObb>();
		//ptrColl->SetFixed(true);

		//重力
		auto grav = AddComponent<Gravity>();

		//噴水エリアからポジションまで
		m_FountainArea = AABB(Vec3(0, -2, 0), 6, 1, 14);
		m_FountainArea2 = AABB(Vec3(-1, -1, -3),Vec3(1, 4, 0));

	}

	void Fountain::OnUpdate()
	{
		auto transComp = GetComponent<Transform>();
		Vec3 pos = transComp->GetPosition();

		//プレイヤーの登録
		auto ObjectPtr_P = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto stage = GetStage();

		if (ObjectPtr_P) {
			//プレイヤーのポジションを取得
			auto PlayerPos = ObjectPtr_P->GetComponent<Transform>()->GetPosition();
			//プレイヤーの中心から１，１，１の範囲
			AABB PlayerAABB = AABB(PlayerPos, 1, 1, 1);
			if (HitTest::AABB_AABB(m_FountainArea, PlayerAABB)) {
				//プレイヤーのポジションをスタートに
				ObjectPtr_P->ResetPositon();

				//プレイヤーを消す
				//ObjectPtr_P->SetDrawActive(false);
			}
			if (m_Active) {
				m_Time += App::GetApp()->GetElapsedTime();
				if (m_Time > 3.5f) {
					m_Active = false;
					AddComponent<Gravity>()->StartJump(Vec3(0, 12.5f, 0.0f));
					m_Time = 0;
				}
				else
				{
					if (pos.y < 0)
						return;
					if (HitTest::AABB_AABB(m_FountainArea2, PlayerAABB)) {
						//プレイヤーのポジションをスタートに
						ObjectPtr_P->ResetPositon();

						//プレイヤーを消す
						//ObjectPtr_P->SetDrawActive(false);
					}
				}
			}
			else {
				m_Time += App::GetApp()->GetElapsedTime();
				if (m_Time > 1.5f) {
					if (HitTest::AABB_AABB(m_FountainArea2, PlayerAABB)) {
						//プレイヤーのポジションをスタートに
						ObjectPtr_P->ResetPositon();

						//プレイヤーを消す
						//ObjectPtr_P->SetDrawActive(false);
					}
				}
			}
		}
	}

	void Fountain::OnCollisionEnter(shared_ptr<GameObject>& Other) {

		m_Active = true;
	}

}
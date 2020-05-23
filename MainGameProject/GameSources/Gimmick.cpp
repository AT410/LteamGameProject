/*!
@breif	ギミック関連実装
@name	作成者：阿部達哉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//----------------------------------------------------------------------------
	//スイッチオブジェクトの実体
	//----------------------------------------------------------------------------
	// -- Xmlマップデータ用コンストラクタ --
	SwitchObj::SwitchObj(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr, pNode)
	{
		m_RecipientKey = XmlDocReader::GetAttribute(pNode, L"EventRecipientKey");
		m_EventMsg = XmlDocReader::GetAttribute(pNode, L"EventMsgStr");
	}

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
			PostEvent(0.0f, GetThis<SwitchObj>(), m_RecipientKey, m_EventMsg);
		}
	}

	void SwitchObj::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		if (!m_IsKeep)
			return;
		//Omoriタグを持った相手と衝突した
		if (Other->FindTag(L"EnabledSwitch")) {

		}
	}

	void SwitchObj::OnCollisionExit(shared_ptr<GameObject>& Other) {
		if (!m_IsKeep)
			return;
		if (Other->FindTag(L"EnabledSwitch")) {
			m_Active = m_IsKeep ? false : m_Active;
		}
	}

	//----------------------------------------------------------------------------
	//導火線実体
	//----------------------------------------------------------------------------
	FireLine::FireLine(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr, pNode)
	{
		m_RecipientKey = XmlDocReader::GetAttribute(pNode, L"EventRecipientKey");
		m_EventMsg = XmlDocReader::GetAttribute(pNode, L"EventMsgStr");

	}

	void FireLine::OnCreate()
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
		//auto ptrColl = AddComponent<CollisionObb>();
		//ptrColl->SetAfterCollision(AfterCollision::Auto);

		if (m_EventActive)
		{
			App::GetApp()->GetEventDispatcher()->AddEventReceiverGroup(m_ReceiverKey, GetThis<FireLine>());
		}
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
			m_scal = ptrTrans->GetScale();
			m_pos = ptrTrans->GetPosition();

			if (m_scal.x > 0) {
				m_scal.x += -0.05;
				m_pos.x += -0.025;
			}
			else {
				PostEvent(0.0f, GetThis<FireLine>(), m_RecipientKey, m_EventMsg);
				GetStage()->RemoveGameObject<FireLine>(GetThis<FireLine>());
			}
			ptrTrans->SetScale(m_scal);
			ptrTrans->SetPosition(m_pos);

		}

	}
	
	void FireLine::OnEvent(const shared_ptr<Event>&event)
	{
		if (event->m_MsgStr == L"FireSwitch")
		{
			m_Active = true;
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


	//----------------------------------------------------------------------------
	//扉クラスの実態
	//----------------------------------------------------------------------------
	// -- Xmlファイル用コンストラクタ --
	Door::Door(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr, pNode),m_OpenActive(false),m_MoveEnd(false),m_TotalTime(0.0f)
	{

	}

	// -- 初期化 --
	void Door::OnCreate()
	{
		// -- 描画設定 --
		auto DrawComp = AddComponent<PNTPointDraw>();
		DrawComp->SetMeshResource(m_meshKey);
		DrawComp->SetTextureResource(m_texKey);

		// -- 配置設定 --
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_pos);
		transComp->SetScale(m_scal);
		transComp->SetQuaternion(Quat(m_rot));

		// -- 物理判定 --
		auto ColComp = AddComponent<CollisionObb>();
		ColComp->SetFixed(true);

		// -- イベント設定 --
		if (m_EventActive)
		{
			App::GetApp()->GetEventDispatcher()->AddEventReceiverGroup(m_ReceiverKey, GetThis<Door>());
		}
	}
	
	// -- 更新処理 --
	void Door::OnUpdate()
	{
		if (m_MoveEnd)
			return;

		if (m_OpenActive)
		{
			OpenProcess();
		}
		else
		{
			//CloseProcess();
		}
	}

	void Door::OpenProcess()
	{
		const float AllTime = 5.0f;
		Vec3 Start, End;
		Start = m_pos;
		End = m_pos + Vec3(0, 5, 0);
		m_MoveEnd = MoveBehavior(Start, End, AllTime);
	}

	void Door::CloseProcess()
	{
		const float AllTime = 5.0f;
		Vec3 Start, End;
		Start = m_pos + Vec3(0, 5, 0);
		End = m_pos;
		m_MoveEnd = !MoveBehavior(Start, End, AllTime);
	}

	bool Door::MoveBehavior(Vec3 Start,Vec3 End, const float AllTime)
	{
		float delta = App::GetApp()->GetElapsedTime();
		m_TotalTime += delta;
		if (m_TotalTime > AllTime)
		{
			m_TotalTime = 0.0f;
			return true;
		}

		// -- イージング処理 --
		Easing<Vec3> easing;
		Vec3 NewPos = easing.EaseInOut(EasingType::Cubic, Start, End, m_TotalTime, AllTime);

		auto TransComp = GetComponent<Transform>();
		TransComp->SetPosition(NewPos);
		return false;
	}

	// -- イベント関数 --
	void Door::OnEvent(const shared_ptr<Event>&event)
	{
		if (event->m_MsgStr == L"ToActive")
		{
			m_OpenActive = true;
		}
	}
	//----------------------------------------------------------------------------
	//噴水クラスの実体
	//----------------------------------------------------------------------------
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


	//-----------------------------------------------------------------------------
	//滝の実体
	//-----------------------------------------------------------------------------
	void Waterfall::OnCreate()
	{
		//支店と終点からメッシュの作成

		float width = m_Width / 2.0f;

		vector<VertexPositionNormalTexture> vertices = 
		{
			{Vec3(m_EndPoint.x- width,m_EndPoint.y,0.0f), Vec3(1,0,1),Vec2(0,0)},
			{Vec3(m_EndPoint.x + width,m_EndPoint.y,0.0f), Vec3(1,0,1),Vec2(1,0)},
			{Vec3(m_StartPoint.x - width,m_StartPoint.y,0.0f), Vec3(1,0,1),Vec2(0,1)},
			{Vec3(m_StartPoint.x+ width,m_StartPoint.y,0.0f), Vec3(1,0,1),Vec2(1,1)},
		};

		vector<uint16_t> indices =
		{
			0,1,2,
			2,1,3,
		};


		auto DrawComp = AddComponent<PNTWaterDraw>();
		DrawComp->CreateOriginalMesh<VertexPositionNormalTexture>(vertices, indices);
		DrawComp->SetOriginalMeshUse(true);
		DrawComp->SetTextureResource(L"WATER_TX");
		DrawComp->SetSubTexResource(L"WATERSUB_TX");
		DrawComp->SetMaskTexResource(L"WATERMASK_TX");
		DrawComp->SetDiffuse(Col4(1, 1, 1, 1));
		DrawComp->SetSamplerState(SamplerState::LinearWrap);

		SetAlphaActive(true);
		
		//当たり判定

		Vec3 EfkPoint = m_EndPoint;

		//水しぶきのエフェクトの再生
		m_EfkPlay = ObjectFactory::Create<EfkPlay>(L"WATERFALL_EFK", EfkPoint);
	}



	void Waterfall::OnUpdate()
	{
		auto DrawComp = GetComponent<PNTWaterDraw>();
		 m_TotalTime += App::GetApp()->GetElapsedTime()*m_FallSpeed;

		DrawComp->UpdateUV(0.0f, -m_TotalTime);

	}
}
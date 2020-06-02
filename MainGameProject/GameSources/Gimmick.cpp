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
			if (!m_Active)
			{
				Vec3 EfkPoint = GetComponent<Transform>()->GetPosition();
				m_ActiveEfk = ObjectFactory::Create<EfkPlay>(L"GOAL_EFK", EfkPoint);
				m_Active = true;
			}
			else
			{
				m_ActiveEfk->StopEffect();
				m_Active = false;
			}
			auto PlayerPtr = dynamic_pointer_cast<Player>(Other);
			if (PlayerPtr)
			{
				PlayerPtr->UpdateResetPositon();
			}
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
			m_ActiveEfk->StopEffect();
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

		auto ConfiguStr = XmlDocReader::GetAttribute(pNode, L"FireLineDirection");
		m_Configu = (FireLineConfigu)_wtoi(ConfiguStr.c_str());
		
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
			auto ptrTrans = GetComponent<Transform>();
			Vec3 ChangeScale = ptrTrans->GetScale();
			Vec3 MovePos = ptrTrans->GetPosition();

			switch (m_Configu)
			{
			case basecross::FireLine::None:
				break;
			case basecross::FireLine::LeftToRight:
				MovePos.x = FireLineBehaviorPos(-1, MovePos.x);
				ChangeScale.x = FireLineBehaviorScale(ChangeScale.x);
				break;
			case basecross::FireLine::RightToLeft:
				MovePos.x = FireLineBehaviorPos(+1, MovePos.x);
				ChangeScale.x = FireLineBehaviorScale(ChangeScale.x);
				break;
			case basecross::FireLine::UpToDown:
				MovePos.y = FireLineBehaviorPos(-1, MovePos.y);
				ChangeScale.y = FireLineBehaviorScale(ChangeScale.y);
				break;
			case basecross::FireLine::DownToUp:
				MovePos.y = FireLineBehaviorPos(+1, MovePos.y);
				ChangeScale.y = FireLineBehaviorScale(ChangeScale.y);
				break;
			case basecross::FireLine::FrontToBack:
				MovePos.z = FireLineBehaviorPos(+1, MovePos.z);
				ChangeScale.z = FireLineBehaviorScale(ChangeScale.z);
				break;
			case basecross::FireLine::BackToFront:
				MovePos.z = FireLineBehaviorPos(-1, MovePos.z);
				ChangeScale.z = FireLineBehaviorScale(ChangeScale.z);
				break;
			default:
				break;
			}

			if (ChangeScale.x <= 0|| ChangeScale.y <= 0|| ChangeScale.z <= 0)
			{
				m_Active = false;
				PostEvent(0.0f, GetThis<FireLine>(), m_RecipientKey, m_EventMsg);
				GetStage()->RemoveGameObject<FireLine>(GetThis<FireLine>());
			}
			ptrTrans->SetScale(ChangeScale);
			ptrTrans->SetPosition(MovePos);

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
	//スロープの実態
	//----------------------------------------------------------------------------
	// -- Xmlファイル用コンストラクタ --
	Slope::Slope(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr, pNode)
	{

	}

	void Slope::OnCreate()
	{
		DefaultSettings();
		SetActions();
	}

	void Slope::OnUpdate()
	{
		if (GetComponent<Actions>()->GetArrived())
		{
			GetComponent<Collision>()->SetUpdateActive(true);
		}
	}

	void Slope::OnEvent(const shared_ptr<Event>& event)
	{
		if (event->m_MsgStr == L"StartAction")
		{
			GetComponent<Collision>()->SetUpdateActive(false);
			GetComponent<Actions>()->Run(L"Start");
		}
		else if (event->m_MsgStr == L"EndAction")
		{
			GetComponent<Collision>()->SetUpdateActive(false);
			GetComponent<Actions>()->Run(L"End");
		}
		else if (event->m_MsgStr == L"TestEvent")
		{
			GetComponent<Collision>()->SetUpdateActive(false);
			GetComponent<Actions>()->Run(L"OnEvent");
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
		//// -- 描画設定 --
		//auto DrawComp = AddComponent<PNTPointDraw>();
		//DrawComp->SetMeshResource(m_meshKey);
		//DrawComp->SetTextureResource(m_texKey);

		//// -- 配置設定 --
		//auto transComp = GetComponent<Transform>();
		//transComp->SetPosition(m_pos);
		//transComp->SetScale(m_scal);
		//transComp->SetQuaternion(Quat(m_rot));

		//// -- 物理判定 --
		//auto ColComp = AddComponent<CollisionObb>();
		//ColComp->SetFixed(true);

		//// -- イベント設定 --
		//if (m_EventActive)
		//{
		//	App::GetApp()->GetEventDispatcher()->AddEventReceiverGroup(m_ReceiverKey, GetThis<Door>());
		//}

		DefaultSettings();
		SetActions();
	}
	
	// -- 更新処理 --
	void Door::OnUpdate()
	{
		if (m_MoveEnd)
			return;

		if (GetComponent<Actions>()->GetArrived())
		{
			GetComponent<Collision>()->SetUpdateActive(true);
		}

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
		else if (event->m_MsgStr == L"StartAction")
		{
			GetComponent<Collision>()->SetUpdateActive(false);
			GetComponent<Actions>()->Run(L"Start");
		}
		else if (event->m_MsgStr == L"EndAction")
		{
			GetComponent<Collision>()->SetUpdateActive(false);
			GetComponent<Actions>()->Run(L"End");
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
		//m_EfkPlay = ObjectFactory::Create<EfkPlay>(L"WATERFALL_EFK", EfkPoint);
	}



	void Waterfall::OnUpdate()
	{
		auto DrawComp = GetComponent<PNTWaterDraw>();
		 m_TotalTime += App::GetApp()->GetElapsedTime()*m_FallSpeed;

		DrawComp->UpdateUV(0.0f, -m_TotalTime);

	}

	WaterJet::WaterJet(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode) :
		ObjectBase(Stageptr, pNode), m_WaterJetmode(false), m_WaterJetDirToUp(false),
		m_SizeAABBX(2.0f), m_SizeAABBY(20.0f), m_SizeAABBZ(2.0f)
	{

	}
	//＠AABBX用のセッター関数
	//＠松崎　洸樹
	//＠m_SizeAABBXのセッター用の関数
	void WaterJet::SetSizeAABBX(const float sizeX) {
		m_SizeAABBX = sizeX;
	}
	//＠AABBY用のセッター関数
	//＠松崎　洸樹
	//＠m_SizeAABBYのセッター用の関数
	void WaterJet::SetSizeAABBY(const float sizeY) {
		m_SizeAABBY = sizeY;
	}
	//＠AABBZ用のセッター関数
	//＠松崎　洸樹
	//＠m_SizeAABBZのセッター用の関数
	void WaterJet::SetSizeAABBZ(const float sizeZ) {
		m_SizeAABBZ = sizeZ;
	}

	void WaterJet::OnCreate() {
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(m_meshKey);
		ptrDraw->SetTextureResource(m_texKey);

		auto ptrTransform = AddComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
		ptrTransform->SetQuaternion(Quat(m_rot));
		ptrTransform->SetScale(m_scal);

		m_StartPos = ptrTransform->GetPosition();
		if (m_WaterJetDirToUp) {
			m_WaterJetAABB = AABB(Vec3(m_StartPos.x - m_SizeAABBX, m_StartPos.y, m_StartPos.z - m_SizeAABBZ),
				Vec3(m_StartPos.x + m_SizeAABBX, m_StartPos.y + m_SizeAABBY, m_StartPos.z + m_SizeAABBZ));
		}
		else {
			m_WaterJetAABB = AABB(Vec3(m_StartPos.x - m_SizeAABBX, m_StartPos.y - m_SizeAABBY, m_StartPos.z - m_SizeAABBZ),
				Vec3(m_StartPos.x + m_SizeAABBX, m_StartPos.y, m_StartPos.z + m_SizeAABBZ));
		}

		//m_efk = ObjectFactory::Create<EfkPlay>(L"WATERFALL_EFK", m_StartPos);
		
		m_WaterFall = GetStage()->AddGameObject<Waterfall>(Vec3(m_StartPos.x,m_StartPos.y,m_StartPos.z),
			Vec3(m_StartPos.x, m_StartPos.y - m_SizeAABBY/2, m_StartPos.z), 2.0f, 1.0f);
		m_WaterFall->SetDrawActive(m_WaterJetmode);
	}

	void WaterJet::OnUpdate() {
		StartJudgment();
		WaterJetJudgment();
	}

	//＠作動判定関数
	//＠松崎　洸樹
	//＠水噴射の機能を作動させるかどうかの判断をさせる関数
	void WaterJet::StartJudgment() {
		auto Elapsedtime = App::GetApp()->GetElapsedTime();
		m_JudmentTime += Elapsedtime;
		if (m_JudmentTime > 4.0f) {
			m_JudmentTime = 0.0f;
			if (m_WaterJetmode) {
				m_WaterJetmode = false;
				m_WaterFall->SetDrawActive(false);
				m_WaterFall->StopSound();
			}
			else {
				m_WaterJetmode = true;
				m_WaterFall->SetDrawActive(true);
				m_WaterFall->StartSound();
				//m_efk = ObjectFactory::Create<EfkPlay>(L"WATERFALL_EFK", m_StartPos);
			}
		}
	}

	//＠水噴射接触判定関数
	//＠松崎　洸樹
	//＠作動している水噴射とプレイヤーに接触した際にプレイヤーで起こることを記述した関数
	void WaterJet::WaterJetJudgment() {
		auto ptrTransform = GetComponent<Transform>();
		m_Pos = ptrTransform->GetPosition();
		auto GetPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = GetPlayer->GetComponent<Transform>()->GetPosition();
		AABB PlayerAABB = AABB(PlayerPos, 1, 1, 1);
		auto ptrDraw = GetComponent<PNTStaticDraw>();
		if (m_WaterJetmode) {
			ptrDraw->SetDiffuse(Col4(0.0f, 1.0f, 0.0f, 1.0f));
			if (HitTest::AABB_AABB(m_WaterJetAABB, PlayerAABB)) {
				GetPlayer->ResetPositon();
			}
		}
		else {
			ptrDraw->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}


	WaterDrop::WaterDrop(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode) :
		ObjectBase(Stageptr, pNode), m_time(3.0f), m_Cooltime(0.0f), m_CooltimeMax(1.5f)
	{}

	void WaterDrop::OnCreate() {
		AddComponent<Gravity>();
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
		ptrTransform->SetQuaternion(Quat(m_rot));
		ptrTransform->SetScale(m_scal);

		auto ptrDraw = AddComponent<PNTPointDraw>();
		ptrDraw->SetMeshResource(m_meshKey);
		ptrDraw->SetTextureResource(m_texKey);

		m_OldPos = ptrTransform->GetPosition();
		m_CurrentPos = ptrTransform->GetPosition();
		AddComponent<CollisionObb>();
	}

	void WaterDrop::OnUpdate() {
		CoolTime();
	}
	//ウォータードロップクール関数
	//松崎　洸樹
	//ウォータードロップの時間的感覚を作る関数
	void WaterDrop::CoolTime() {
		auto ptrGrav = GetComponent<Gravity>();
		auto ptrDraw = GetComponent<PNTPointDraw>();
		auto ptrColl = GetComponent<CollisionObb>();
		auto Elapsedtime = App::GetApp()->GetElapsedTime();
		if (m_Cooltime > 0.0f) {
			GetComponent<Transform>()->SetPosition(m_pos);

			m_Cooltime += -Elapsedtime;
			ptrGrav->SetUpdateActive(false);
			ptrDraw->SetDrawActive(false);
			ptrColl->SetUpdateActive(false);
		}
		else {
			ptrGrav->SetUpdateActive(true);
			ptrDraw->SetDrawActive(true);
			ptrColl->SetUpdateActive(true);
		}

	}

	void WaterDrop::OnCollisionEnter(shared_ptr<GameObject>&Obj)
	{
		auto PPtr = dynamic_pointer_cast<Player>(Obj);
		if (PPtr)
		{
			PPtr->ResetPositon();
		}
		m_Cooltime = m_CooltimeMax;
		App::GetApp()->GetXAudio2Manager()->Start(L"WaterDrop_SD", 0, 0.1f);
	}


	WaterLV::WaterLV(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode) :
		ObjectBase(Stageptr, pNode)
	{}

	void WaterLV::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
		ptrTransform->SetQuaternion(Quat(m_rot));
		ptrTransform->SetScale(m_scal);

		auto ptrScale = ptrTransform->GetScale();
		m_WaterOldPos = ptrTransform->GetPosition();
		m_WaterCurrentPos = ptrTransform->GetPosition();
		m_AABBX = ptrScale.x;
		m_AABBY = ptrScale.y;
		m_AABBZ = ptrScale.z;
		m_PlayerAABBX = ptrScale.x / 2;
		m_PlayerAABBY = ptrScale.y / 2;
		m_PlayerAABBZ = ptrScale.z / 2;
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(m_meshKey);
		ptrDraw->SetTextureResource(m_texKey);

		//m_WaterLVMode = true;

		if (m_EventActive)
		{
			App::GetApp()->GetEventDispatcher()->AddEventReceiverGroup(m_ReceiverKey, GetThis<WaterLV>());
		}
	}

	void WaterLV::OnUpdate() {
		WaterLVChange();
		WaterLVJudgement();
	}

	//＠ウォーターレベルモードセット関数
	//＠松崎　洸樹
	//bool型のウォーターレベルモードのセット関数
	void WaterLV::SetWaterLVMode(const bool LVMode) {
		m_WaterLVMode = LVMode;
	}
	//＠ウォーターレベル変更関数
	//＠松崎　洸樹
	//＠ウォーターレベルの水位状態変化を行う関数
	void WaterLV::WaterLVChange() {
		auto Elapsedtime = App::GetApp()->GetElapsedTime();
		auto ptrTransform = GetComponent<Transform>();
		auto ptrObjPos = ptrTransform->GetPosition();
		if (m_WaterLVMode) {
			if (m_WaterCurrentPos.y > m_WaterOldPos.y - 4.0f) {
				m_WaterCurrentPos.y += -0.5f * Elapsedtime;
			}
			else if (m_WaterCurrentPos.y < m_WaterOldPos.y - 4.0f) {
				m_WaterTime += Elapsedtime;
			}
			if (m_WaterTime > 6.0f) {
				m_WaterCurrentPos.y += 1.0f * Elapsedtime;
			}
			if (m_WaterCurrentPos.y > m_WaterOldPos.y) {
				m_WaterTime = 0.0f;
				m_WaterCurrentPos.y = m_WaterOldPos.y;
				m_WaterLVMode = false;
			}
		}
		ptrTransform->SetPosition(m_WaterCurrentPos);
	}
	//＠ウォーターレベル判定関数
	//＠松崎　洸樹
	//＠ウォーターレベルとプレイヤーのボリューム境界における接触判定を行う関数
	void WaterLV::WaterLVJudgement() {
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = ptrPlayer->GetComponent<Transform>()->GetPosition();
		auto ptrTransform = GetComponent<Transform>();
		auto ptrPos = ptrTransform->GetPosition();
		m_WaterLVAABB = AABB(ptrPos, m_AABBX, m_AABBY, m_AABBZ);
		auto PlayerAABB = AABB(Vec3(PlayerPos.x - m_PlayerAABBX, PlayerPos.y, PlayerPos.z - m_PlayerAABBZ),
			Vec3(PlayerPos.x + m_PlayerAABBX, PlayerPos.y + m_PlayerAABBY, PlayerPos.z + m_PlayerAABBZ));
		if (HitTest::AABB_AABB(m_WaterLVAABB, PlayerAABB)) {
			ptrPlayer->ResetPositon();
		}
	}

	void WaterLV::OnEvent(const shared_ptr<Event>& event)
	{
		if (event->m_MsgStr == L"WaterDown")
		{
			m_WaterLVMode = true;
		}
	}

	UpDownBox::UpDownBox(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode) :
		ObjectBase(Stageptr, pNode), m_Speed(2.0f), m_OldPos(0.0f), m_parenttime(0.0f)
	{}

	void UpDownBox::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
		ptrTransform->SetQuaternion(Quat(m_rot));
		ptrTransform->SetScale(m_scal);

		auto ptrPos = ptrTransform->GetPosition();
		auto ptrColl = AddComponent<CollisionObb>();

		auto ptrDraw = AddComponent<PNTPointDraw>();
		ptrDraw->SetMeshResource(m_meshKey);
		ptrDraw->SetTextureResource(m_texKey);

		m_OldPos = ptrTransform->GetPosition();
		m_CurrentPos = ptrTransform->GetPosition();

		for (auto tag : m_tag)
		{
			if (tag == L"")
				continue;
			AddTag(tag);
		}
	}

	void UpDownBox::OnUpdate() {
		BoxJudgment();
		//浮く処理
		m_CurrentPos = GetComponent<Transform>()->GetPosition();
		//FloatMove();
	}
	//ボックス判定関数
	//松崎　洸樹
	//プレイヤーが離れた時にプレイヤーとボックスの親子化解除をするための関数
	void UpDownBox::BoxJudgment() {
		auto Elapsedtime = App::GetApp()->GetElapsedTime();
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto ptrTransform = GetComponent<Transform>();
		if (m_ParentJudge) {
			m_CurrentPos.y += -Elapsedtime;
			ptrTransform->SetPosition(m_CurrentPos);
		}
		else {
			m_parenttime += -Elapsedtime;
			if (m_parenttime <= 0.0f) {
				FloatMove();
			}
			else if (m_parenttime > 0.0f) {
				m_CurrentPos.y += -Elapsedtime;
				ptrTransform->SetPosition(m_CurrentPos);

			}
		}
	}
	//衝突判定関数（衝突している間）
	//松崎　洸樹
	//プレイヤーと衝突したときボックスは沈みプレイヤーと親子になる
	void UpDownBox::OnCollisionExcute(shared_ptr<GameObject>& Obj) {
		auto Elapsedtime = App::GetApp()->GetElapsedTime();
		auto obj = GetComponent<Transform>()->GetGameObject();
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");

		if (Obj->FindTag(L"EnabledSwitch")) {
			m_parenttime = 2.0f;
			m_ParentJudge = true;
		}
	}
	//衝突判定関数（衝突から離れた時）
	//松崎　洸樹
	//プレイヤーと離れた時親子化解除するためのbool型の処理をする
	void UpDownBox::OnCollisionExit(shared_ptr<GameObject>& Obj) {
		m_ParentJudge = false;
	}

	//床が動く関数
	//
	//床が浮き上がる関数
	bool UpDownBox::FloatMove() {
		//BoxJudgment();
		//浮く処理
		m_totaltime += App::GetApp()->GetElapsedTime();
		if (m_totaltime > 5.0f)
		{
			m_totaltime = 0;
			return true;
		}

		Easing<Vec3> easing;

		auto ep = easing.Linear(m_OldPos, m_CurrentPos, m_totaltime, 5.0);

		GetComponent<Transform>()->SetPosition(ep);
	}

}
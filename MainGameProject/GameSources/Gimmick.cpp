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
		:ObjectBase(StagePtr, pNode),m_IsKeep(false)
	{
		m_RecipientKey = XmlDocReader::GetAttribute(pNode, L"EventRecipientKey");
		m_EventMsg = XmlDocReader::GetAttribute(pNode, L"EventMsgStr");
	}

	void SwitchObj::OnCreate()
	{
		DefaultSettings();
		//描画
		auto drawComp = AddComponent<PNTStaticDraw>();

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();

		//コリジョンを付ける
		auto ptrColl = GetComponent<CollisionObb>();
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
				m_Active = true;
				if(!m_ActiveEfk)
					m_ActiveEfk = ObjectFactory::Create<EfkPlay>(L"GOAL_EFK", EfkPoint);
			}
			else
			{
				//m_ActiveEfk.reset();
				//m_Active = false;
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
		//この状態だと導火線作動時に位置がずれる
		DefaultSettings();
		SetActions();
		//描画 
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();

		//ポジション、スケール、回転
		auto transComp = GetComponent<Transform>();

	}

	void FireLine::OnUpdate()
	{
		if (m_Active)
		{
			auto ptrTrans = GetComponent<Transform>();
			Vec3 ChangeScale = ptrTrans->GetScale();
			Vec3 MovePos = ptrTrans->GetPosition();
			Vec3 EfkPoint = MovePos;

			switch (m_Configu)
			{
			case basecross::FireLine::None:
				break;
			case basecross::FireLine::LeftToRight:
				MovePos.x = FireLineBehaviorPos(-1, MovePos.x);
				ChangeScale.x = FireLineBehaviorScale(ChangeScale.x);
				EfkPoint.x = MovePos.x + ChangeScale.x/2.0f;
				break;
			case basecross::FireLine::RightToLeft:
				MovePos.x = FireLineBehaviorPos(+1, MovePos.x);
				ChangeScale.x = FireLineBehaviorScale(ChangeScale.x);
				EfkPoint.x = MovePos.x - ChangeScale.x/2.0f;
				break;
			case basecross::FireLine::UpToDown:
				MovePos.y = FireLineBehaviorPos(-1, MovePos.y);
				ChangeScale.y = FireLineBehaviorScale(ChangeScale.y);
				EfkPoint.y = MovePos.y + ChangeScale.y / 2.0f;
				break;
			case basecross::FireLine::DownToUp:
				MovePos.y = FireLineBehaviorPos(+1, MovePos.y);
				ChangeScale.y = FireLineBehaviorScale(ChangeScale.y);
				EfkPoint.y = MovePos.y - ChangeScale.y / 2.0f;
				break;
			case basecross::FireLine::FrontToBack:
				MovePos.z = FireLineBehaviorPos(+1, MovePos.z);
				ChangeScale.z = FireLineBehaviorScale(ChangeScale.z);
				EfkPoint.z = MovePos.z - ChangeScale.z / 2.0f;
				break;
			case basecross::FireLine::BackToFront:
				MovePos.z = FireLineBehaviorPos(-1, MovePos.z);
				ChangeScale.z = FireLineBehaviorScale(ChangeScale.z);
				EfkPoint.z = MovePos.z + ChangeScale.z / 2.0f;
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
			m_Fire->SetLocation(EfkPoint);
		}

	}
	
	void FireLine::OnEvent(const shared_ptr<Event>&event)
	{
		if (event->m_MsgStr == L"FireSwitch")
		{
			m_Active = true;
			m_Fire = ObjectFactory::Create<EfkPlay>(L"GOAL_EFK", Vec3(0));
		}
	}

	//動く床
	void MoveFloor::OnCreate()
	{
		DefaultSettings();
		SetActions();

		GetComponent<Collision>()->AddExcludeCollisionTag(L"Fixed");

	}
	void MoveFloor::OnUpdate()
	{

	}

	void MoveFloor::OnEvent(const shared_ptr<Event>&event)
	{
		ObjectBase::OnEvent(event);
		
		if (event->m_MsgStr == L"MoveFloor")
		{
			GetComponent<Actions>()->Run(L"OnEvent",m_IsEventActionLoop);
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

		auto DrawComp = GetComponent<SmBaseDraw>();
		auto MeshRes = DrawComp->GetMeshResource();
		DrawComp->SetSamplerState(SamplerState::LinearWrap);

		auto Vertex = MeshRes->GetBackupVerteces<VertexPositionNormalTexture>();
		for (int i = 0; i < Vertex.size(); i++)
		{
			auto v = Vertex[i].textureCoordinate;
			float Value = m_scal.length();
			v.x *= Value;
			Vertex[i].textureCoordinate = v;
		}
		//_cnt++;
		DrawComp->UpdateVertices(Vertex);

		AddTag(L"Fixed");
		GetComponent<Collision>()->AddExcludeCollisionTag(L"Fixed");
	}

	void Slope::OnUpdate()
	{
		//if (GetComponent<Actions>()->GetArrived())
		//{
		//	GetComponent<Collision>()->SetUpdateActive(true);
		//}
	}

	void Slope::OnEvent(const shared_ptr<Event>& event)
	{
		ObjectBase::OnEvent(event);

		if (event->m_MsgStr == L"TestEvent")
		{
			if(m_EventActionActive)
				GetComponent<Actions>()->Run(L"OnEvent",m_IsEventActionLoop);
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
		auto ActionPtr = GetComponent<Actions>(false);
		if (ActionPtr)
		{
			if(ActionPtr->GetArrived())
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
					}
				}
			}
			else {
				m_Time += App::GetApp()->GetElapsedTime();
				if (m_Time > 1.5f) {
					if (HitTest::AABB_AABB(m_FountainArea2, PlayerAABB)) {
						//プレイヤーのポジションをスタートに
						ObjectPtr_P->ResetPositon();
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

		m_vertices = 
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
		DrawComp->CreateOriginalMesh<VertexPositionNormalTexture>(m_vertices, indices);
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
	}



	void Waterfall::OnUpdate()
	{
		auto DrawComp = GetComponent<PNTWaterDraw>();
		m_TotalTime += App::GetApp()->GetElapsedTime()*m_FallSpeed;

		DrawComp->UpdateUV(0.0f, -m_TotalTime);

		DrawComp->UpdateVertices<VertexPositionNormalTexture>(m_vertices);
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
		
		GetUnderFloor();
		auto Floor = m_Floor.lock();
		Vec3 End = Vec3(m_StartPos.x, m_StartPos.y - m_SizeAABBY / 2, m_StartPos.z);
		if (Floor)
		{
			auto TransComp = GetComponent<Transform>();

			auto FloorAABB = Floor->GetComponent<CollisionObb>()->GetWrappedAABB();
			auto Pos = TransComp->GetPosition();

			Vec3 recVec;
			HitTest::ClosestPtPointAABB(Pos, FloorAABB, recVec);

			End = recVec;
		}

		m_WaterFall = GetStage()->AddGameObject<Waterfall>(Vec3(m_StartPos.x,m_StartPos.y,m_StartPos.z),
			End, 2.0f, 1.0f);
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
		auto ptrDraw = GetComponent<PNTStaticDraw>();
		float Elapsedtime = App::GetApp()->GetElapsedTime();
		if (m_IntervalFlag)
		{
			m_IntervalTime += Elapsedtime;

			m_FlashTime += Elapsedtime * 5.0f;
			if (m_FlashTime >= XM_2PI) {
				m_FlashTime = 0;
			}
			auto Diffuse = ptrDraw->GetDiffuse();
			Diffuse.y = abs(sin(m_FlashTime));
			ptrDraw->SetDiffuse(Diffuse);

			if (m_IntervalTime > 2.0f)
			{
				m_JudmentTime = 0.0f;
				m_IntervalTime = 0.0f;
				m_IntervalFlag = false;
			}
		}
		else
		{
			m_JudmentTime += Elapsedtime;
			if (m_JudmentTime > 3.0f) {
				m_JudmentTime = 0.0f;
				if (m_WaterJetmode) {
					m_WaterJetmode = false;
					m_WaterFall->SetDrawActive(false);
					m_WaterFall->StopSound();
					ptrDraw->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 1.0f));
					//m_IntervalFlag = true;
				}
				else {
					m_WaterJetmode = true;
					ptrDraw->SetDiffuse(Col4(0.0f, 1.0f, 0.0f, 1.0f));
					m_IntervalFlag = true;
				}
			}
		}
	}

	//＠水噴射接触判定関数
	//＠松崎　洸樹
	//＠作動している水噴射とプレイヤーに接触した際にプレイヤーで起こることを記述した関数
	void WaterJet::WaterJetJudgment() {
		auto ptrTransform = GetComponent<Transform>();
		auto GetPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		Vec3 PlayerPos = GetPlayer->GetComponent<Transform>()->GetPosition();
		AABB PlayerAABB = AABB(PlayerPos, 1, 1, 1);
		if (m_WaterJetmode&& !m_IntervalFlag) {
			if (HitTest::AABB_AABB(m_WaterJetAABB, PlayerAABB)) {
				GetPlayer->SetPlayerState(PlayerState::Restart);
			}
			// -- 真下にエフェクトを飛ばす --
			auto Floor = m_Floor.lock();

			if (Floor)
			{
				auto TransComp = GetComponent<Transform>();

				AABB FloorAABB = Floor->GetComponent<CollisionObb>()->GetWrappedAABB();
				Vec3 Pos = TransComp->GetPosition();

				Vec3 recVec;
				HitTest::ClosestPtPointAABB(Pos, FloorAABB, recVec);

				m_WaterFall->UpdateEndPoint(recVec);

				if(!m_efk)
					m_efk = ObjectFactory::Create<EfkPlay>(L"WATERFALL_EFK", recVec);
			}
			else
			{
				GetUnderFloor();
			}

			m_WaterFall->SetDrawActive(true);
			m_WaterFall->StartSound();

		}
		else {
			if (m_efk)
				m_efk.reset();
		}
	}

	void WaterJet::GetUnderFloor()
	{
		Vec3 Near, Far;
		Near = GetComponent<Transform>()->GetPosition();
		Far = Near + Vec3(0, -50, 0);
		for (auto& v : GetStage()->GetGameObjectVec()) {
			if (v->FindTag(L"WaterJetEnd")) {
				auto ColObb = v->GetComponent<CollisionObb>(false);
				if (ColObb) {
					auto Obb = ColObb->GetObb();
					if (HitTest::SEGMENT_OBB(Near, Far, Obb)) {
						m_Floor = v;
					}
				}
			}
		}

	}

	WaterDrop::WaterDrop(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode) :
		ObjectBase(Stageptr, pNode), m_Time(3.0f), m_Cooltime(0.0f), m_CooltimeMax(1.5f)
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
		float Elapsedtime = App::GetApp()->GetElapsedTime();
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
	//接触時関数
	//
	//プレイヤーが水滴に接触した際の関数
	void WaterDrop::OnCollisionEnter(shared_ptr<GameObject>&Obj)
	{
		auto PPtr = dynamic_pointer_cast<Player>(Obj);
		if (PPtr)
		{
			PPtr->SetPlayerState(PlayerState::Restart);
		}
		m_Cooltime = m_CooltimeMax;
		App::GetApp()->GetXAudio2Manager()->Start(L"WaterDrop_SD", 0, 0.1f);
	}

	//----------------------------------------------------------------------------
	//水位変更
	//----------------------------------------------------------------------------
	WaterLV2::WaterLV2(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr, pNode),m_IntervalActive(false)
	{}

	void WaterLV2::OnCreate()
	{
		DefaultSettings();

		// -- 衝突判定(無効化) --
		auto CollComp = GetComponent<Collision>();
		CollComp->SetAfterCollision(AfterCollision::None);
		CollComp->AddExcludeCollisionTag(L"PushPullObj");
		CollComp->SetUpdateActive(false);

		AddTag(L"Water");
	}

	void WaterLV2::OnUpdate()
	{
		if (m_MainActive)
		{
			ChangeLevel();
		}
		else
		{
			HitResponseAABB();
		}
	}
	void WaterLV2::ChangeLevel()
	{
		if (m_IntervalActive)
		{
			m_IntervalTime += App::GetApp()->GetElapsedTime();
			if (m_IntervalTime >= 10.0f)
			{
				m_IntervalActive = false;
				m_IntervalTime = 0.0f;
			}
			return;
		}
		else
		{
			HitResponseAABB();
		}

		if (LVBhavior(0.5f))
		{
			if (!m_ContFlag)
			{
				m_ContFlag = true;
			}
		}
	}

	void WaterLV2::HitResponseAABB()
	{
		// -- プレイヤー --
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player", false);
		if (PlayerPtr)
		{
			// -- 自らのAABB --
			auto TransComp = GetComponent<Transform>();
			Vec3 MyPos = TransComp->GetPosition();
			Vec3 MyScalehalf = TransComp->GetScale() / 2.0f;
			AABB MyAABB = AABB(MyPos - MyScalehalf, MyPos + MyScalehalf);

			// -- プレイヤー位置 --
			Vec3 PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
			PlayerPos.y += 1.0f;

			// -- プレイヤーの中心点が、含まれているか --
			if (MyAABB.PtInAABB(PlayerPos))
			{
				PlayerPtr->SetState(PlayerState::Restart);
			}
		}
	}

	bool WaterLV2::LVBhavior(const float ScaleVal)
	{
		bool recflag = false;
		auto TransComp = GetComponent<Transform>();
		Vec3 CurrntPos = TransComp->GetPosition();
		Vec3 CurrntScale = TransComp->GetScale();

		float key = m_ContFlag ? +1.0f : -1.0f;

		float deltatime = App::GetApp()->GetElapsedTime();

		CurrntScale.y += key * ScaleVal*deltatime;
		CurrntPos.y += key * ScaleVal / 2.0f*deltatime;


		if (m_ContFlag)
		{
			if (CurrntScale.y >= m_scal.y)
			{
				CurrntScale.y = m_scal.y;
				CurrntPos.y = m_pos.y;
				recflag = true;
				m_End = true;
			}
			this->SetDrawActive(true);
		}
		else
		{
			if (CurrntScale.y <= 0.0f) 
			{
				CurrntScale.y = 0.0f;
				m_IntervalActive = true;
				recflag = true;
				this->SetDrawActive(false);
			}
		}

		if (!recflag)
		{
			TransComp->SetPosition(CurrntPos);
			TransComp->SetScale(CurrntScale);
		}
		return recflag;
	}

	void WaterLV2::OnEvent(const shared_ptr<Event>&event)
	{
		if (event->m_MsgStr == L"WaterDown")
		{
			m_MainActive = true;
			if (m_IntervalActive||m_ContFlag)
			{
				m_ContFlag = false;
				m_IntervalActive = false;
				m_IntervalTime = 0.0f;
				m_End = false;
			}
		}
	}

	void WaterLV2::OnCollisionEnter(shared_ptr<GameObject>&Obj)
	{
		// -- 対PullBox --
		if (Obj->FindTag(L"PushPullObj"))
		{
			Vec3 BoxPos = Obj->GetComponent<Transform>()->GetWorldPosition();
			auto TransComp = GetComponent<Transform>();
			Vec3 MyPos = TransComp->GetPosition();
			Vec3 MyScale = TransComp->GetScale();
			auto WaterAABB = GetComponent<CollisionObb>()->GetWrappedAABB();

			if (WaterAABB.PtInAABB(BoxPos))
			{
				float UpHeigth = MyPos.y + MyScale.y / 2.0f;
				Obj->GetComponent<Transform>()->SetPosition(BoxPos.x, UpHeigth, BoxPos.z);
			}
		}
		else if (Obj->FindTag(L"Player"))// -- 対プレイヤ --
		{
			Vec3 Pos = Obj->GetComponent<Transform>()->GetWorldPosition();
			auto WaterAABB = GetComponent<CollisionObb>()->GetWrappedAABB();
			if (WaterAABB.PtInAABB(Pos))
			{
				auto PlayePtr = dynamic_pointer_cast<Player>(Obj);
				if (PlayePtr)
				{
					PlayePtr->SetState(PlayerState::Restart);
				}
			}
		}
	}

	void WaterLV2::OnCollisionExcute(shared_ptr<GameObject>&Obj)
	{
		// -- 対PullBox --
		if (Obj->FindTag(L"PushPullObj"))
		{
			Vec3 BoxPos = Obj->GetComponent<Transform>()->GetWorldPosition();
			auto TransComp = GetComponent<Transform>();
			Vec3 MyPos = TransComp->GetPosition();
			Vec3 MyScale = TransComp->GetScale();

			auto WaterAABB = GetComponent<CollisionObb>()->GetWrappedAABB();

			if (WaterAABB.PtInAABB(BoxPos))
			{
				float UpHeigth = MyPos.y + MyScale.y / 2.0f;
				Obj->GetComponent<Transform>()->SetPosition(BoxPos.x, UpHeigth, BoxPos.z);
				Obj->GetComponent<Gravity>()->SetUpdateActive(false);
			}
			else
			{
				//Obj->GetComponent<Gravity>()->SetUpdateActive(true);
			}
		}
		else if (Obj->FindTag(L"Player"))// -- 対プレイヤ --
		{
			Vec3 Pos = Obj->GetComponent<Transform>()->GetWorldPosition();
			auto WaterAABB = GetComponent<CollisionObb>()->GetWrappedAABB();
			if (WaterAABB.PtInAABB(Pos))
			{
				auto PlayePtr = dynamic_pointer_cast<Player>(Obj);
				if (PlayePtr)
				{
					PlayePtr->SetState(PlayerState::Restart);
				}
			}
		}
	}

	void WaterLV2::OnCollisionExit(shared_ptr<GameObject>&Obj)
	{
		// -- 対PullBox --
		if (Obj->FindTag(L"PushPullObj"))
		{
			Obj->GetComponent<Gravity>()->SetUpdateActive(true);
		}
		else if (Obj->FindTag(L"Player"))// -- 対プレイヤ --
		{

		}
	}

	//!End　WaterLV2

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
			ptrPlayer->SetPlayerState(PlayerState::Restart);
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
		ObjectBase(Stageptr, pNode), m_OldPos(0.0f), m_Parenttime(0.0f)
	{}

	void UpDownBox::OnCreate() {

		DefaultSettings();
		auto ptrTransform = GetComponent<Transform>();

		m_OldPos = ptrTransform->GetPosition();
		m_CurrentPos = ptrTransform->GetPosition();

	}

	void UpDownBox::OnUpdate() {
		BoxJudgment();
		//浮く処理
		m_CurrentPos = GetComponent<Transform>()->GetPosition();
	}
	//ボックス判定関数
	//松崎　洸樹
	//プレイヤーとボックスの親子関係の関数
	void UpDownBox::BoxJudgment() {
		float Elapsedtime = App::GetApp()->GetElapsedTime();
		auto ptrTransform = GetComponent<Transform>();
		if (m_ParentJudge) {
			m_CurrentPos.y += -Elapsedtime;
			ptrTransform->SetPosition(m_CurrentPos);
		}
		else {
			m_Parenttime += -Elapsedtime;
			if (m_Parenttime <= 0.0f) {
				FloatMove();
			}
			else if (m_Parenttime > 0.0f) {
				m_CurrentPos.y += -Elapsedtime;
				ptrTransform->SetPosition(m_CurrentPos);

			}
		}
	}
	//衝突判定関数（衝突している間）
	//松崎　洸樹
	//オブジェクトが衝突している間の関数
	void UpDownBox::OnCollisionExcute(shared_ptr<GameObject>& Obj) {
		if (Obj->FindTag(L"EnabledSwitch")) {
			m_Parenttime = 2.0f;
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
		m_Totaltime += App::GetApp()->GetElapsedTime();
		if (m_Totaltime > 5.0f)
		{
			m_Totaltime = 0;
			return true;
		}

		Easing<Vec3> easing;

		auto ep = easing.Linear(m_OldPos, m_CurrentPos, m_Totaltime, 5.0);

		GetComponent<Transform>()->SetPosition(ep);
		return false;
	}

	PushObj::PushObj(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode):
	ObjectBase(StagePtr, pNode),m_Boxmode(true)
	{}

	void PushObj::OnCreate() {
		DefaultSettings();
		AddComponent<Gravity>();
	}

	void PushObj::OnUpdate() {
		BoxState();
	}

	//ボックス状態関数
	//松崎　洸樹
	//ボックスの動かせる状態と静止している状態を分けている
	void PushObj::BoxState() {
		auto ptrTransform = GetComponent<Transform>();
		auto ptrPos = ptrTransform->GetPosition();
		
		if (m_Boxmode) {
			m_CurrentPos = ptrPos;
			m_PastPos = ptrPos;
			ptrTransform->SetPosition(m_CurrentPos);
		}
		else {
			
			ptrTransform->SetPosition(m_StopPos);
		}

	}
	//接触関数
	//松崎　洸樹
	//接触した際の位置情報更新関数
	void PushObj::OnCollisionEnter(shared_ptr<GameObject>& Obj) {
			m_StopPos = m_PastPos;
			m_CurrentPos = m_PastPos;
	}
	//接触中関数
	//松崎　洸樹
	//オブジェクトの移動機能動作できるようにする関数
	void PushObj::OnCollisionExcute(shared_ptr<GameObject>& Obj) {
		auto ptrPlayer = dynamic_pointer_cast<Player>(Obj);
		
		if (ptrPlayer) {
			m_Boxmode = false;
			if (ptrPlayer->GetPushBoxActiv()) {
				m_Boxmode = true;
			}
			else if (!ptrPlayer->GetPushBoxActiv()) {
				m_Boxmode = false;
			}
		}
	}

	Goal::Goal(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr, pNode), m_IsGoal(false), m_count(0.0f)
	{

	}

	void Goal::OnCreate()
	{
		DefaultSettings();
		//描画設定
		//auto DrawComp = AddComponent<PNTStaticDraw>();
		//マップに登録されてない（DEFAULT_CUBE, TEST_TXになってる）↓
		//DrawComp->SetMeshResource(L"GOAL_MD");
		//DrawComp->SetTextureResource(L"GOAL_TX");

		//配置設定
		auto TransComp = GetComponent<Transform>();
		m_pos.y += -0.5f;

		auto CollComp = AddComponent<CollisionObb>();
		CollComp->SetAfterCollision(AfterCollision::None);
		//マップにアクティブ化してない↓
		GetStage()->SetSharedGameObject(L"Goal", GetThis<Goal>());


	}

	void Goal::OnUpdate()
	{
		if (m_IsGoal)
		{
			m_count += App::GetApp()->GetElapsedTime();
			if (m_count > 2.0f)
			{
				auto  StageSelect = GameManager::GetManager()->GetStagePair();
				auto MaxStageCount = GameManager::GetManager()->GetMaxStagePair();
				if (StageSelect.second != MaxStageCount.second - 1)
				{
					StageSelect.second += 1;
					GameManager::GetManager()->SetStagePair(StageSelect);
					PostEvent(0.0, GetThis<ObjectInterface>(), L"Camera", L"Clear", L"ToGameStage");
					GameManager::GetManager()->SetStartCameraActive(false);
					GameManager::GetManager()->SetUpdateActive(false);
				}
				else
				{
					if (StageSelect.first != MaxStageCount.first - 1)
					{
						StageSelect.first += 1;
						StageSelect.second = 0;
						GameManager::GetManager()->SetStagePair(StageSelect);
						PostEvent(0.0, GetThis<ObjectInterface>(), L"Camera", L"Clear", L"ToGameStage");
						GameManager::GetManager()->SetStartCameraActive(false);
						GameManager::GetManager()->SetUpdateActive(false);
					}
					else
					{
						GameManager::GetManager()->SetUpdateActive(false);
						PostEvent(0.0, GetThis<ObjectInterface>(), L"Camera", L"Clear", L"ToEndingStage");
					}
				}
				GameManager::GetManager()->GetSaveData()->Clear();
				m_IsGoal = false;
			}
		}
	}

}
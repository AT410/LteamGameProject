
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	WaterWheel::WaterWheel(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	WaterWheel::~WaterWheel() {}
	void WaterWheel::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
		//タグをつける
		AddTag(L"WaterWheel");
		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"mizuguruma");
		ptrDraw->SetOwnShadowActive(true);

		PsBoxParam param(ptrTransform->GetWorldMatrix(), 0.0f, true,
			PsMotionType::MotionTypeFixed);
		auto PsPtr = AddComponent<RigidbodyBox>(param);
		PsPtr->SetAutoTransform(false);
		AddComponent<CollisionObb>();
	}
	void WaterWheel::OnUpdate()
	{
		//      Time += App::GetApp()->GetElapsedTime();
			  //auto ptrTrans = GetComponent<Transform>();
			  //m_Rotation = ptrTrans->GetRotation();
			  //Easing<Vec3> easing;
			  //auto Force = easing.Linear(m_Rotation, Vec3(0, 0, 0), Time, 5.0f);
			  //const float LerpFact = 1.0f;
			  //bsm::Quat FQuat;
			  //FQuat.rotationRollPitchYawFromVector(Force);
			  //FQuat.normalize();
			  //ptrTrans->SetQuaternion(FQuat);

		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= 4.0f)
		{
			m_TotalTime = 0.0f;
			m_Swap = 1 - m_Swap;
		}
		auto PtrTrans = GetComponent<Transform>();
		auto StartPos = PtrTrans->GetPosition();
		StartPos.x = m_Position.x;
		auto EndPos = StartPos;
		EndPos.x += 4.0f;
		Vec3 StartRot(0, 0, 0);
		Vec3 EndRot(0, 0, 180);
		Vec3 TgtPos, TgtRot;
		Easing<Vec3> easing;
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (KeyState.m_bPushKeyTbl['R'])
		{

			TgtRot = easing.EaseInOut(EasingType::Exponential, EndRot, StartRot, m_TotalTime, 4.0f);

		}
		if (KeyState.m_bPushKeyTbl['L'])
		{

			TgtRot = easing.EaseInOut(EasingType::Exponential, StartRot, EndRot, m_TotalTime, 4.0f);

		}

		bsm::Quat FQuat;
		FQuat.rotationRollPitchYawFromVector(TgtRot);
		FQuat.normalize();
		PtrTrans->SetQuaternion(FQuat);
		PtrTrans->SetQuaternion(FQuat);


	}


	//バルブ
	Valve::Valve(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}

	Valve::~Valve() {}
	void Valve::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
		//タグをつける
		AddTag(L"Valve");
		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//ptrDraw->SetTextureResource(L"SKY_TX");
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetOwnShadowActive(true);

		PsBoxParam param(ptrTransform->GetWorldMatrix(), 0.0f, true,
			PsMotionType::MotionTypeFixed);
		auto PsPtr = AddComponent<RigidbodyBox>(param);
		PsPtr->SetAutoTransform(false);
		AddComponent<CollisionObb>();
	}

	void Valve::OnUpdate()
	{
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= 4.0f)
		{
			m_TotalTime = 0.0f;
			m_Swap = 1 - m_Swap;
		}
		auto PtrTrans = GetComponent<Transform>();
		auto StartPos = PtrTrans->GetPosition();
		StartPos.x = m_Position.x;
		auto EndPos = StartPos;
		EndPos.x += 4.0f;
		Vec3 StartRot(0, 0, 0);
		Vec3 EndRot(0, 40, 0);
		Vec3 TgtPos, TgtRot;
		Easing<Vec3> easing;
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (KeyState.m_bPushKeyTbl['K'])
		{

			TgtRot = easing.EaseInOut(EasingType::Exponential, EndRot, StartRot, m_TotalTime, 4.0f);
			m_valveTime += 1 * ElapsedTime;

		}
		if (KeyState.m_bPushKeyTbl['J'])
		{

			TgtRot = easing.EaseInOut(EasingType::Exponential, StartRot, EndRot, m_TotalTime, 4.0f);
			m_valveTime -= 1 * ElapsedTime;
		}


		if (m_valveTime > 1)
		{
			GetStage()->GetSharedGameObject<ValveGate>(L"Valvegate")->setopenflg(true);
			GetStage()->GetSharedGameObject<ValveGate2>(L"Valvegate2")->setopenflg2(true);
		}
		//if (m_valveTime < 0 && m_valveTime < 5)
		//{

		//}
		bsm::Quat FQuat;
		FQuat.rotationRollPitchYawFromVector(TgtRot);
		FQuat.normalize();
		PtrTrans->SetQuaternion(FQuat);
		PtrTrans->SetQuaternion(FQuat);
	}



	//バルブで開くゲートの処理
	ValveGate::ValveGate(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	ValveGate::~ValveGate() {}
	void ValveGate::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
		//タグをつける
		AddTag(L"Floor");
		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetTextureResource(L"isi");
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetOwnShadowActive(true);

		PsBoxParam param(ptrTransform->GetWorldMatrix(), 0.0f, true,
			PsMotionType::MotionTypeFixed);
		auto PsPtr = AddComponent<RigidbodyBox>(param);
		PsPtr->SetAutoTransform(false);
		AddComponent<CollisionObb>();
	}

	void ValveGate::ValveGateOpen()
	{
		auto deltatime = App::GetApp()->GetElapsedTime();
		auto Pos = GetComponent<Transform>()->GetPosition();
		Pos.y += OpenSpeed * deltatime;
		GetComponent<Transform>()->SetPosition(Pos);
	}
	void ValveGate::OnUpdate()
	{
		if (gateopen)
		{
			ValveGate::ValveGateOpen();
		}
	}




	ValveGate2::ValveGate2(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	ValveGate2::~ValveGate2() {}
	void ValveGate2::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
		//タグをつける
		AddTag(L"Floor");
		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetTextureResource(L"isi");
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetOwnShadowActive(true);

		PsBoxParam param(ptrTransform->GetWorldMatrix(), 0.0f, true,
			PsMotionType::MotionTypeFixed);
		auto PsPtr = AddComponent<RigidbodyBox>(param);
		PsPtr->SetAutoTransform(false);
		AddComponent<CollisionObb>();
	}

	void ValveGate2::ValveGate2Open()
	{
		auto deltatime = App::GetApp()->GetElapsedTime();
		auto Pos = GetComponent<Transform>()->GetPosition();
		Pos.y += OpenSpeed * deltatime;
		GetComponent<Transform>()->SetPosition(Pos);
	}
	void ValveGate2::OnUpdate()
	{
		if (gateopen)
		{
			ValveGate2::ValveGate2Open();
		}
	}




	TraceSprite::TraceSprite(const shared_ptr<Stage>& StagePtr, bool Trace,
		const Vec2& StartScale, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0),
		m_changespeed(1),//ここで点滅の速度を変える
		m_changespeed2(1)
	{}
	TraceSprite::~TraceSprite() {}
	void TraceSprite::OnCreate() {
		float helfSize = 0.50f;
		//頂点配列
		float sizex = 200 ;
		float sizey = 200;
		m_BackupVertices.clear();
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(-sizex, sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f),Vec2(0,0)));
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(sizex, sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1,0)));
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(-sizex, -sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0,1)));
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(sizex, -sizey, 0), Col4(1.0f, 1.0f, 1, 1.0f), Vec2(1,1)));
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//SetAlphaActive(m_Trace);
		auto ptrTrans = GetComponent<Transform>();
		//ptrTrans->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		ptrTrans->SetRotation(0, 0, 0);
		ptrTrans->SetPosition(m_StartPos);
		//頂点とインデックスを指定してスプライト作成
		auto drawcomp = AddComponent<PCTSpriteDraw>();
		drawcomp->CreateMesh<VertexPositionColorTexture>(m_BackupVertices, indices);
		drawcomp->SetTextureResource(L"iseki");
		SetAlphaActive(true);
		GetStage()->GetSharedObjectGroup(L"UIGroup")->IntoGroup(GetThis<TraceSprite>());


	}	
	
	
	
	
	TraceSprite2::TraceSprite2(const shared_ptr<Stage>& StagePtr, bool Trace,
		const Vec2& StartScale, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0),
		m_changespeed(1),
		m_changespeed2(1)
	{}
	TraceSprite2::~TraceSprite2() {}
	void TraceSprite2::OnCreate() {
		float helfSize = 0.50f;
		//頂点配列
		float sizex = 200;
		float sizey = 200;
		m_BackupVertices.clear();
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(-sizex, sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0, 0)));
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(sizex, sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1, 0)));
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(-sizex, -sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0, 1)));
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(sizex, -sizey, 0), Col4(1.0f, 1.0f, 1, 1.0f), Vec2(1, 1)));
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//SetAlphaActive(m_Trace);
		auto ptrTrans = GetComponent<Transform>();
		//ptrTrans->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		ptrTrans->SetRotation(0, 0, 0);
		ptrTrans->SetPosition(m_StartPos);
		//頂点とインデックスを指定してスプライト作成
		auto drawcomp = AddComponent<PCTSpriteDraw>();
		drawcomp->CreateMesh<VertexPositionColorTexture>(m_BackupVertices, indices);
		drawcomp->SetTextureResource(L"isi");
		SetAlphaActive(true);
		GetStage()->GetSharedObjectGroup(L"UIGroup")->IntoGroup(GetThis<TraceSprite2>());
	}


	TraceSprite3::TraceSprite3(const shared_ptr<Stage>& StagePtr, bool Trace,
		const Vec2& StartScale, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0),
		m_changespeed(1),
		m_changespeed2(1)
	{}
	TraceSprite3::~TraceSprite3() {}
	void TraceSprite3::OnCreate() {
		float helfSize = 0.50f;
		//頂点配列
		float sizex = 200;
		float sizey = 200;
		m_BackupVertices.clear();
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(-sizex, sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0, 0)));
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(sizex, sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1, 0)));
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(-sizex, -sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0, 1)));
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(sizex, -sizey, 0), Col4(1.0f, 1.0f, 1, 1.0f), Vec2(1, 1)));
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//SetAlphaActive(m_Trace);
		auto ptrTrans = GetComponent<Transform>();
		//ptrTrans->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		ptrTrans->SetRotation(0, 0, 0);
		ptrTrans->SetPosition(m_StartPos);
		//頂点とインデックスを指定してスプライト作成
		auto drawcomp = AddComponent<PCTSpriteDraw>();
		drawcomp->CreateMesh<VertexPositionColorTexture>(m_BackupVertices, indices);
		drawcomp->SetTextureResource(L"sima");
		SetAlphaActive(true);
		GetStage()->GetSharedObjectGroup(L"UIGroup")->IntoGroup(GetThis<TraceSprite3>());
	}


	TraceSprite4::TraceSprite4(const shared_ptr<Stage>& StagePtr, bool Trace,
		const Vec2& StartScale, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0),
		m_changespeed(1),
		m_changespeed2(1)
	{}
	TraceSprite4::~TraceSprite4() {}
	void TraceSprite4::OnCreate() {
		float helfSize = 0.50f;
		//頂点配列
		float sizex = 200;
		float sizey = 200;
		m_BackupVertices.clear();
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(-sizex, sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0, 0)));
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(sizex, sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1, 0)));
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(-sizex, -sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0, 1)));
		m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(sizex, -sizey, 0), Col4(1.0f, 1.0f, 1, 1.0f), Vec2(1, 1)));
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//SetAlphaActive(m_Trace);
		auto ptrTrans = GetComponent<Transform>();
		//ptrTrans->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		ptrTrans->SetRotation(0, 0, 0);
		ptrTrans->SetPosition(m_StartPos);
		//頂点とインデックスを指定してスプライト作成
		auto drawcomp = AddComponent<PCTSpriteDraw>();
		drawcomp->CreateMesh<VertexPositionColorTexture>(m_BackupVertices, indices);
		drawcomp->SetTextureResource(L"maguma");
		SetAlphaActive(true);
		GetStage()->GetSharedObjectGroup(L"UIGroup")->IntoGroup(GetThis<TraceSprite4>());
	}



	//点滅のupdate処理
	void TraceSprite::OnUpdate()
	{
		//↓stateが0の時
		//0の時は10秒点滅
		///////////////////////////////////////////
		//if (state == 0)
	//	{
			float elapsedTime = App::GetApp()->GetElapsedTime();
			m_TotalTime += elapsedTime;
			if (m_TotalTime >= m_changespeed) {
				m_TotalTime = 0;
			}
			auto DrawComp = GetComponent<PCTSpriteDraw>();
			vector<VertexPositionColorTexture> newVertices;
			m_BackupVertices = DrawComp->GetMeshResource()->GetBackupVerteces<VertexPositionColorTexture>();
			for (size_t i = 0; i < m_BackupVertices.size(); i++) {
				Col4 col = m_BackupVertices[i].color;
				col.w = sin(m_TotalTime);
				auto v = VertexPositionColorTexture(
					m_BackupVertices[i].position,
					col,
					m_BackupVertices[i].textureCoordinate
				);
				newVertices.push_back(v);
			}
			auto ptrDraw = GetComponent<PCTSpriteDraw>();
			ptrDraw->UpdateVertices(newVertices);
			auto Diffuse = DrawComp->GetDiffuse();
			Diffuse.w = sin(m_TotalTime);//x,y,z,wを変えると色が変わる
			DrawComp->SetDiffuse(Diffuse);
		//}

		//↓stateが1の時
		//1の時は1秒点滅
		///////////////////////////////////////////////////////////
		//auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		//if (KeyState.m_bPushKeyTbl[VK_LEFT])
		//{
		//	m_state = 1;
		//}
		//if (KeyState.m_bPushKeyTbl['O'])
		//{
		//	m_state = 0;
		//}
		//if (KeyState.m_bPushKeyTbl[VK_RIGHT])
		//{
		//	m_state = 2;
		//}

		//if (m_state == 1)
		//{
		//	float elapsedTime = App::GetApp()->GetElapsedTime();
		//	m_TotalTime += elapsedTime;
		//	if (m_TotalTime >= m_changespeed2) {
		//		m_TotalTime = 0;
		//	}
		//	auto DrawComp = GetComponent<PCTSpriteDraw>();

		//	auto Diffuse = DrawComp->GetDiffuse();
		//	Diffuse.w = sin(m_TotalTime);
		//	DrawComp->SetDiffuse(Diffuse);
		//};
	}

	//入力を別に描くgamestage.cppに   5/14

	void TraceSprite2::OnUpdate()
	{
		//↓stateが0の時
		//0の時は10秒点滅
		///////////////////////////////////////////
		//auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		//if (state == 0 && KeyState.m_bPushKeyTbl[VK_DOWN])
			//if (state == 1)
			//{
				float elapsedTime = App::GetApp()->GetElapsedTime();
				m_TotalTime += elapsedTime;
				if (m_TotalTime >= m_changespeed) {
					m_TotalTime = 0;
				}
				auto DrawComp = GetComponent<PCTSpriteDraw>();
				vector<VertexPositionColorTexture> newVertices;
				m_BackupVertices = DrawComp->GetMeshResource()->GetBackupVerteces<VertexPositionColorTexture>();
				for (size_t i = 0; i < m_BackupVertices.size(); i++) {
					Col4 col = m_BackupVertices[i].color;
					col.w = sin(m_TotalTime);
					auto v = VertexPositionColorTexture(
						m_BackupVertices[i].position,
						col,
						m_BackupVertices[i].textureCoordinate
					);
					newVertices.push_back(v);
				}
				auto ptrDraw = GetComponent<PCTSpriteDraw>();
				ptrDraw->UpdateVertices(newVertices);
				auto Diffuse = DrawComp->GetDiffuse();
				Diffuse.w = sin(m_TotalTime);
				DrawComp->SetDiffuse(Diffuse);
		//	}
	}

	void TraceSprite3::OnUpdate()
	{
		//↓stateが0の時
		//0の時は10秒点滅
		///////////////////////////////////////////
		//auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		//if (state == 1 && KeyState.m_bPushKeyTbl[VK_DOWN])
		//{
		//	state = 2;
		//}
		//if (state == 2)
		//{
			float elapsedTime = App::GetApp()->GetElapsedTime();
			m_TotalTime += elapsedTime;
			if (m_TotalTime >= m_changespeed) {
				m_TotalTime = 0;
			}
			auto DrawComp = GetComponent<PCTSpriteDraw>();
			vector<VertexPositionColorTexture> newVertices;
			m_BackupVertices = DrawComp->GetMeshResource()->GetBackupVerteces<VertexPositionColorTexture>();
			for (size_t i = 0; i < m_BackupVertices.size(); i++) {
				Col4 col = m_BackupVertices[i].color;
				col.w = sin(m_TotalTime);
				auto v = VertexPositionColorTexture(
					m_BackupVertices[i].position,
					col,
					m_BackupVertices[i].textureCoordinate
				);
				newVertices.push_back(v);
			}
			auto ptrDraw = GetComponent<PCTSpriteDraw>();
			ptrDraw->UpdateVertices(newVertices);
			auto Diffuse = DrawComp->GetDiffuse();
			Diffuse.w = sin(m_TotalTime);
			DrawComp->SetDiffuse(Diffuse);
		//}
	}


	void TraceSprite4::OnUpdate()
	{
		//↓stateが0の時
		//0の時は10秒点滅
		///////////////////////////////////////////
		//auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		//if (state == 2 && KeyState.m_bPushKeyTbl[VK_DOWN])
		//{
		//	state = 3;
		//}
		//if (state == 3)
		{
			float elapsedTime = App::GetApp()->GetElapsedTime();
			m_TotalTime += elapsedTime;
			if (m_TotalTime >= m_changespeed) {
				m_TotalTime = 0;
			}
			auto DrawComp = GetComponent<PCTSpriteDraw>();
			vector<VertexPositionColorTexture> newVertices;
			m_BackupVertices = DrawComp->GetMeshResource()->GetBackupVerteces<VertexPositionColorTexture>();
			for (size_t i = 0; i < m_BackupVertices.size(); i++) {
				Col4 col = m_BackupVertices[i].color;
				col.w = sin(m_TotalTime);
				auto v = VertexPositionColorTexture(
					m_BackupVertices[i].position,
					col,
					m_BackupVertices[i].textureCoordinate
				);
				newVertices.push_back(v);
			}
			auto ptrDraw = GetComponent<PCTSpriteDraw>();
			ptrDraw->UpdateVertices(newVertices);
			auto Diffuse = DrawComp->GetDiffuse();
			Diffuse.w = sin(m_TotalTime);
			DrawComp->SetDiffuse(Diffuse);
		}
	}


		//半透明の処理

		//選択点滅の処理



		void Select::OnCreate()
		{
			auto drawComp = AddComponent<PNTStaticDraw>();
			drawComp->SetMeshResource(L"DEFAULT_CUBE");

			auto transComp = GetComponent<Transform>();
			transComp->SetScale(5, 2, 5);
		}


		SelectSprite::SelectSprite(const shared_ptr<Stage>& StagePtr, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos) :
			GameObject(StagePtr),
			m_Trace(Trace),
			m_StartScale(StartScale),
			m_StartPos(StartPos),
			m_TotalTime(0),
			m_changespeed(10),
			m_state(0),
			m_changespeed2(1)
		{}
		SelectSprite::~SelectSprite() {}
		void SelectSprite::OnCreate() {
			float helfSize = 0.50f;
			//頂点配列
			float sizex = 200 / 2.0f;
			float sizey = 200 / 2.0f;
			m_BackupVertices.clear();
			m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(-sizex, sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0, 0)));
			m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(sizex, sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1, 0)));
			m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(-sizex, -sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0, 1)));
			m_BackupVertices.push_back(VertexPositionColorTexture(Vec3(sizex, -sizey, 0), Col4(1.0f, 1.0f, 1, 1.0f), Vec2(1, 1)));
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//SetAlphaActive(m_Trace);
			auto ptrTrans = GetComponent<Transform>();
			//ptrTrans->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
			ptrTrans->SetRotation(0, 0, 0);
			ptrTrans->SetPosition(m_StartPos);
			//頂点とインデックスを指定してスプライト作成
			auto drawcomp = AddComponent<PCTSpriteDraw>();
			drawcomp->CreateMesh<VertexPositionColorTexture>(m_BackupVertices, indices);
			drawcomp->SetTextureResource(L"isi");

		}
	}
	//void Stagechoice::UIctr()
	//{
	//	std::vector<std::shared_ptr<TraceSprite>>;
	//	auto TranceSprite = dynamic_pointer_cast<TraceSprite>(obj);

	//}
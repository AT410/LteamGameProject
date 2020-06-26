/*!
@breif 汎用オブジェクト実装
@name　作成者：阿部達哉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	
	void FixedObj::OnCreate()
	{
		DefaultSettings();
		SetActions();

		AddTag(L"Fixed");
		GetComponent<Collision>()->AddExcludeCollisionTag(L"Fixed");
	}

	void FixedObj::OnEvent(const shared_ptr<Event>&event)
	{
		ObjectBase::OnEvent(event);

		if (event->m_MsgStr == L"TestEvent")
		{
			this->SetDrawActive(true);
			this->SetUpdateActive(true);
		}
	}

	void LoopTexObj::OnCreate()
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
			v.y *= Value;
			//v.y = PALSE * sinf((i + _cnt) / 20.0f);
			Vertex[i].textureCoordinate = v;
		}
		//_cnt++;
		DrawComp->UpdateVertices(Vertex);

		if (m_StartActionActive)
		{
			GetComponent<Collision>()->SetUpdateActive(false);
			GetComponent<Actions>()->Run(L"Start");
		}
	}

	void LoopTexObj::OnEvent(const shared_ptr<Event>&event)
	{
		ObjectBase::OnEvent(event);

		if (event->m_MsgStr == L"TestEvent")
		{
			this->SetDrawActive(true);
			this->SetUpdateActive(true);
		}
	}

	//透明な壁：前に落ちないようにするため
	void FrontWallObj::OnCreate()
	{
		//セットしたらテクスチャ貼った壁になるためコメント化↓
		//DefaultSettings();
		//配置設定　透明なテクスチャ出来たら削除
		auto TransComp = GetComponent<Transform>();
		TransComp->SetPosition(m_pos);
		TransComp->SetQuaternion(Quat(m_rot));
		TransComp->SetScale(m_scal);

		AddTag(L"Fixed");		///<-固定オブジェクト同士の物理判定から除外
		AddTag(L"FireIgnore");	///<-火との当たり判定から除外

		//物理判定
		auto CollComp = AddComponent<CollisionObb>();
		CollComp->SetFixed(true);
		CollComp->AddExcludeCollisionTag(L"Fixed");
	}
	//----------------------------------------------------------------------------
	//基本入力クラス
	//----------------------------------------------------------------------------
	void DefaultInput::OnUpdate()
	{
		m_handler.PushHandler(GetThis<DefaultInput>());
	}

	void DefaultInput::OnPushA()
	{
		if (m_IsSend)
			return;
		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), m_StageStr);
		m_IsSend = true;
	}

	//--------------------------------------------------------------------------------------
	//　オープニングカメラマン
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CameraMan::CameraMan(const shared_ptr<Stage>& StagePtr ,const Vec3& StartEye, const Vec3& StartAt) :
		GameObject(StagePtr),
		m_StartEye(StartEye),
		m_EndEye(0.0f, 0.0f, -1.0f),
		m_AtStartPos(StartAt),
		m_AtEndPos(18.0f, 0.0f, 18.0f),
		m_AtPos(StartAt),
		m_CreatePosEye(StartEye),
		m_CreatePosAt(StartAt),
		m_TotalTime(0.0f)
	{}

	CameraMan::~CameraMan() {}
	//初期化
	void CameraMan::OnCreate() {
		//初期位置などの設定
		auto ptr = GetComponent<Transform>();
		ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(m_CreatePosEye);
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<CameraMan>(GetThis<CameraMan>()));
		//最初のステートをCameramanToGoalStateに設定
		m_StateMachine->ChangeState(CameramanToGoalState::Instance());

		App::GetApp()->GetEventDispatcher()->AddEventReceiverGroup(L"Camera", GetThis<ObjectInterface>());
	}
	//操作
	void CameraMan::OnUpdate() {
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	void CameraMan::OnEvent(const shared_ptr<Event>&event)
	{
		if (event->m_MsgStr == L"Clear")
		{
			m_StateMachine->ChangeState(CameramanClearState::Instance());
			m_MsgEvent = event->m_MsgStr2;
		}
	}

	void CameraMan::ToGoalEnterBehavior() {
		m_AtStartPos = m_CreatePosAt;
		m_AtEndPos = Vec3(0.0f, 0.0f, 0.0f);
		m_AtPos = m_AtStartPos;
		m_TotalTime = 0.0f;

		auto PlayerPtr = GetStage()->GetSharedGameObject<Goal>(L"Goal");
		m_AtEndPos = PlayerPtr->GetComponent<Transform>()->GetPosition();


		// -- Eyeを求める --
		Vec3 Eye = Vec3(m_AtEndPos.x, m_AtEndPos.y + 2.0f, m_AtEndPos.z - 6.0f);
		Vec3 SightVec = Eye - m_AtEndPos;
		SightVec.normalize();
		SightVec *= 5.0f;
		m_EndEye = Vec3(m_AtEndPos.x, Eye.y + SightVec.y, Eye.z + SightVec.z);
	}

	void CameraMan::ToStartEnterBehavior() {
		m_EndEye = m_CreatePosEye;
		m_AtStartPos = Vec3(0.0f, 0.0f, 0.0f);
		m_AtEndPos = m_CreatePosAt;
		m_TotalTime = 0.0f;

		auto PlayerPtr = GetStage()->GetSharedGameObject<Goal>(L"Goal");
		m_AtStartPos = PlayerPtr->GetComponent<Transform>()->GetPosition();

		m_AtPos = m_AtStartPos;

		m_StartEye = GetComponent<Transform>()->GetPosition();
	}

	bool CameraMan::ExcuteBehavior(float totaltime) {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime > totaltime) {
			return true;
		}
		Easing<Vec3> easing;
		auto TgtPos = easing.EaseInOut(EasingType::Quartic, m_StartEye, m_EndEye, m_TotalTime, totaltime);
		m_AtPos = easing.EaseInOut(EasingType::Quartic, m_AtStartPos, m_AtEndPos, m_TotalTime, totaltime);
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(TgtPos);
		return false;
	}

	void CameraMan::EndStateEnterBehavior() {
		auto ptrGameGtage = GetTypeStage<GameStage>();
		ptrGameGtage->ToMyCamera();
		GameManager::GetManager()->SetStartCameraActive(true);
	}

	void CameraMan::ClearStateEnterBehavior()
	{
		auto ptrGameGtage = GetTypeStage<GameStage>();
		ptrGameGtage->ToEventCamera();
	}

	void CameraMan::ToClearMoveEnterBehavior()
	{
		// -- 現在のカメラ情報取得 --
		auto Camera = GetStage()->GetView()->GetTargetCamera();
		m_StartEye = Camera->GetEye();
		m_AtStartPos = Camera->GetAt();

		m_AtEndPos = Vec3(0.0f, 0.0f, 0.0f);
		m_AtPos = m_AtStartPos;
		m_TotalTime = 0.0f;

		auto PlayerPtr = GetStage()->GetSharedGameObject<Goal>(L"Goal");
		m_AtEndPos = PlayerPtr->GetComponent<Transform>()->GetWorldPosition();


		// -- Eyeを求める --
		Vec3 Eye = Vec3(m_AtEndPos.x, m_AtEndPos.y + 2.0f, m_AtEndPos.z - 6.0f);
		Vec3 SightVec = Eye - m_AtEndPos;
		SightVec.normalize();
		SightVec *= 5.0f;
		m_EndEye = Vec3(m_AtEndPos.x, Eye.y + SightVec.y, Eye.z + SightVec.z);

		GetComponent<Transform>()->SetPosition(m_StartEye);
	}

	void CameraMan::EventStart()
	{
		PostEvent(0.0f, GetThis<ObjectInterface>(), L"Fade", m_MsgEvent, L"FadeOutGoal");
	}
	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanToGoalState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CameramanToGoalState)

	void CameramanToGoalState::Enter(const shared_ptr<CameraMan>&Obj)
	{
		Obj->ToGoalEnterBehavior();
	}

	void CameramanToGoalState::Execute(const shared_ptr<CameraMan>&Obj)
	{
		if (Obj->ExcuteBehavior(3.0f))
		{
			Obj->GetStateMachine()->ChangeState(CameramanToStartState::Instance());
		}
	}

	void CameramanToGoalState::Exit(const shared_ptr<CameraMan>& Obj)
	{

	}

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanToStartState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CameramanToStartState)

	void CameramanToStartState::Enter(const shared_ptr<CameraMan>& Obj)
	{
		Obj->ToStartEnterBehavior();
	}

	void CameramanToStartState::Execute(const shared_ptr<CameraMan>&Obj)
	{
		if (Obj->ExcuteBehavior(3.0f))
		{
			Obj->GetStateMachine()->ChangeState(CameramanEndState::Instance());
		}

	}

	void CameramanToStartState::Exit(const shared_ptr<CameraMan>&Obj) {}

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanEndState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CameramanEndState)

	void CameramanEndState::Enter(const shared_ptr<CameraMan>&Obj)
	{
		Obj->EndStateEnterBehavior();
	}

	void CameramanEndState::Execute(const shared_ptr<CameraMan>&Obj) {}

	void CameramanEndState::Exit(const shared_ptr<CameraMan>&Obj) {}

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanEndState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CameramanClearState)

	void CameramanClearState::Enter(const shared_ptr<CameraMan>&Obj)
	{
		Obj->ToClearMoveEnterBehavior();
		Obj->ClearStateEnterBehavior();
	}

	void CameramanClearState::Execute(const shared_ptr<CameraMan>&Obj)
	{
		if (Obj->ExcuteBehavior(2.0f))
		{
			Obj->GetStateMachine()->ChangeState(CameraNoneState::Instance());
		}
	}

	void CameramanClearState::Exit(const shared_ptr<CameraMan>&Obj)
	{
		Obj->EventStart();
	}

	IMPLEMENT_SINGLETON_INSTANCE(CameraNoneState)

}
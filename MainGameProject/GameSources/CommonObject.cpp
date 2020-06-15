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
	OpeningCameraman::OpeningCameraman(const shared_ptr<Stage>& StagePtr ,const Vec3& StartEye, const Vec3& StartAt) :
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

	OpeningCameraman::~OpeningCameraman() {}
	//初期化
	void OpeningCameraman::OnCreate() {
		//初期位置などの設定
		auto ptr = GetComponent<Transform>();
		ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(m_CreatePosEye);
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<OpeningCameraman>(GetThis<OpeningCameraman>()));
		//最初のステートをOpeningCameramanToGoalStateに設定
		m_StateMachine->ChangeState(OpeningCameramanToGoalState::Instance());

		App::GetApp()->GetEventDispatcher()->AddEventReceiverGroup(L"Camera", GetThis<ObjectInterface>());
	}
	//操作
	void OpeningCameraman::OnUpdate() {
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	void OpeningCameraman::OnEvent(const shared_ptr<Event>&event)
	{
		if (event->m_MsgStr == L"Clear")
		{
			m_StateMachine->ChangeState(CameramanClearState::Instance());
			m_MsgEvent = event->m_MsgStr2;
		}
	}

	void OpeningCameraman::ToGoalEnterBehavior() {
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

	void OpeningCameraman::ToStartEnterBehavior() {
		m_EndEye = m_CreatePosEye;
		m_AtStartPos = Vec3(0.0f, 0.0f, 0.0f);
		m_AtEndPos = m_CreatePosAt;
		m_TotalTime = 0.0f;

		auto PlayerPtr = GetStage()->GetSharedGameObject<Goal>(L"Goal");
		m_AtStartPos = PlayerPtr->GetComponent<Transform>()->GetPosition();

		m_AtPos = m_AtStartPos;

		m_StartEye = GetComponent<Transform>()->GetPosition();
	}

	bool OpeningCameraman::ExcuteBehavior(float totaltime) {
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

	void OpeningCameraman::EndStateEnterBehavior() {
		auto ptrGameGtage = GetTypeStage<GameStage>();
		ptrGameGtage->ToMyCamera();
		GameManager::GetManager()->SetStartCameraActive(true);
	}

	void OpeningCameraman::ClearStateEnterBehavior()
	{
		auto ptrGameGtage = GetTypeStage<GameStage>();
		ptrGameGtage->ToEventCamera();
	}

	void OpeningCameraman::ToClearMoveEnterBehavior()
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

	void OpeningCameraman::EventStart()
	{
		PostEvent(0.0f, GetThis<ObjectInterface>(), L"Fade", m_MsgEvent, L"FadeOutGoal");
	}
	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanToGoalState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(OpeningCameramanToGoalState)

		void OpeningCameramanToGoalState::Enter(const shared_ptr<OpeningCameraman>&Obj)
	{
		Obj->ToGoalEnterBehavior();
	}

	void OpeningCameramanToGoalState::Execute(const shared_ptr<OpeningCameraman>&Obj)
	{
		if (Obj->ExcuteBehavior(3.0f))
		{
			Obj->GetStateMachine()->ChangeState(OpeningCameramanToStartState::Instance());
		}
	}

	void OpeningCameramanToGoalState::Exit(const shared_ptr<OpeningCameraman>& Obj)
	{

	}

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanToStartState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(OpeningCameramanToStartState)

		void OpeningCameramanToStartState::Enter(const shared_ptr<OpeningCameraman>& Obj)
	{
		Obj->ToStartEnterBehavior();
	}

	void OpeningCameramanToStartState::Execute(const shared_ptr<OpeningCameraman>&Obj)
	{
		if (Obj->ExcuteBehavior(3.0f))
		{
			Obj->GetStateMachine()->ChangeState(OpeningCameramanEndState::Instance());
		}

	}

	void OpeningCameramanToStartState::Exit(const shared_ptr<OpeningCameraman>&Obj) {}

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanEndState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(OpeningCameramanEndState)

		void OpeningCameramanEndState::Enter(const shared_ptr<OpeningCameraman>&Obj)
	{
		Obj->EndStateEnterBehavior();
	}

	void OpeningCameramanEndState::Execute(const shared_ptr<OpeningCameraman>&Obj) {}

	void OpeningCameramanEndState::Exit(const shared_ptr<OpeningCameraman>&Obj) {}

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanEndState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(CameramanClearState)

	void CameramanClearState::Enter(const shared_ptr<OpeningCameraman>&Obj)
	{
		Obj->ToClearMoveEnterBehavior();
		Obj->ClearStateEnterBehavior();
	}

	void CameramanClearState::Execute(const shared_ptr<OpeningCameraman>&Obj) 
	{
		if (Obj->ExcuteBehavior(2.0f))
		{
			Obj->GetStateMachine()->ChangeState(CameraNoneState::Instance());
		}
	}

	void CameramanClearState::Exit(const shared_ptr<OpeningCameraman>&Obj) 
	{
		Obj->EventStart();
	}

	IMPLEMENT_SINGLETON_INSTANCE(CameraNoneState)

}
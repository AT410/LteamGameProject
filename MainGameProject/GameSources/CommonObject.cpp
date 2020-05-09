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
		//描画設定
		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetMeshResource(m_meshKey);
		DrawComp->SetTextureResource(m_texKey);

		//配置設定
		auto TransComp = GetComponent<Transform>();
		TransComp->SetPosition(m_pos);
		TransComp->SetQuaternion(Quat(m_rot));
		TransComp->SetScale(m_scal);

		//物理判定
		auto CollComp = AddComponent<CollisionObb>();
		CollComp->SetFixed(true);

		for (auto tag : m_tag)
		{
			if (tag == L"")
				continue;
			AddTag(tag);
		}

		if (m_SharedActive)
		{
			GetStage()->SetSharedGameObject(m_SharedName, GetThis<FixedObj>());
		}

	}

	///<breif>汎用移動オブジェクト<breif/>
	MoveObj::MoveObj(const shared_ptr<Stage>&StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
		const wstring TexKey, const wstring MeshKey, const Vec3 Start, const Vec3 End, const float Speed, const float TotalTime)
		:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		,m_Start(Start),m_End(End),m_speed(Speed),m_Movetime(TotalTime), m_CurrntTime(0.0f),m_Type(MovingType::Rotation)
	{
	
	}

	MoveObj::MoveObj(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr,pNode)
	{
		
	}

	MoveObj::~MoveObj()
	{

	}

	void MoveObj::OnCreate()
	{
		//描画設定
		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetMeshResource(m_meshKey);
		DrawComp->SetTextureResource(m_texKey);

		//配置設定
		auto TransComp = GetComponent<Transform>();
		TransComp->SetPosition(m_pos);
		TransComp->SetScale(m_scal);
		TransComp->SetQuaternion(Quat(m_rot));

		//物理判定
		auto CollComp = AddComponent<CollisionObb>();
		CollComp->SetFixed(true);

		for (auto tag : m_tag)
		{
			AddTag(tag);
		}

		if (m_SharedActive)
		{
			GetStage()->SetSharedGameObject(m_SharedName, GetThis<MoveObj>());
		}
	}

	void MoveObj::OnUpdate()
	{
		//移動処理
		switch (m_Type)
		{
		case basecross::MovingType::Position:
			PosMove();
			break;
		case basecross::MovingType::Rotation:
			RotMove();
			break;
		case basecross::MovingType::Scaling:
			break;
		default:
			break;
		}
	}

	void MoveObj::PosMove()
	{
		if (LerpMove(m_Start, m_End))
		{
			Swap<Vec3>(m_Start,m_End);
		}
		else
		{

		}
	}

	bool MoveObj::LerpMove( Vec3 Start, Vec3 End)
	{
		m_CurrntTime += m_speed * App::GetApp()->GetElapsedTime();
		if (m_Movetime < m_CurrntTime)
		{
			m_CurrntTime = 0.0f;
			return true;
		}

		Easing<Vec3> easing;
		auto Force = easing.EaseInOut(EasingType::Cubic, Start, End, m_CurrntTime, m_Movetime);
		
		auto TransComp = GetComponent<Transform>();
		TransComp->SetPosition(Force);
		return false;
	}

	void MoveObj::RotMove()
	{
		auto ptrTrans = GetComponent<Transform>();
		auto Rotation = ptrTrans->GetRotation();
		Vec3 Force = Vec3(0, 0, 10)*m_speed*App::GetApp()->GetElapsedTime();
		bsm::Quat FQuat;
		FQuat.rotationRollPitchYawFromVector(Force);
		FQuat.normalize();
		ptrTrans->SetQuaternion(FQuat);
	}

	//--------------------------------------------------------------------------------------
//　オープニングカメラマン
//--------------------------------------------------------------------------------------
	//構築と破棄
	OpeningCameraman::OpeningCameraman(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_StartPos(0.0f, 0.0f, -75.0f),
		m_EndPos(0.0f, 0.0f, -1.0f),
		m_AtStartPos(0.0f, 0.0f, 0.0f),
		m_AtEndPos(18.0f, 0.0f, 18.0f),
		m_AtPos(m_AtStartPos),
		m_TotalTime(0.0f)
	{}

	OpeningCameraman::~OpeningCameraman() {}
	//初期化
	void OpeningCameraman::OnCreate() {
		//初期位置などの設定
		auto ptr = GetComponent<Transform>();
		ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(m_StartPos);
		//ステートマシンの構築
		m_StateMachine.reset(new StateMachine<OpeningCameraman>(GetThis<OpeningCameraman>()));
		//最初のステートをOpeningCameramanToGoalStateに設定
		m_StateMachine->ChangeState(OpeningCameramanToGoalState::Instance());
	}
	//操作
	void OpeningCameraman::OnUpdate() {
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	void OpeningCameraman::ToGoalEnterBehavior() {
		m_StartPos = Vec3(7.0f, 10.0f, -20.0f);
		m_AtStartPos = Vec3(7.0f, 2.0f, 0.0f);
		m_AtEndPos = Vec3(0.0f, 0.0f, 0.0f);
		m_AtPos = m_AtStartPos;
		m_TotalTime = 0.0f;

		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		m_AtEndPos = PlayerPtr->GetComponent<Transform>()->GetPosition();

		m_EndPos = m_AtEndPos;
		m_EndPos.z -= 10.0f;
	}

	void OpeningCameraman::ToStartEnterBehavior() {
		m_StartPos = Vec3(0.0f, 0.0f, -60.0f);
		m_EndPos = Vec3(7.0f, 10.0f, -20.0f);
		m_AtStartPos = Vec3(0.0f, 0.0f, 0.0f);
		m_AtEndPos = Vec3(7.0f, 2.0f, 0.0f);
		m_AtPos = m_AtStartPos;
		m_TotalTime = 0.0f;

		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		m_AtStartPos = PlayerPtr->GetComponent<Transform>()->GetPosition();

		m_StartPos = GetComponent<Transform>()->GetPosition();
	}

	bool OpeningCameraman::ExcuteBehavior(float totaltime) {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime > totaltime) {
			return true;
		}
		Easing<Vec3> easing;
		auto TgtPos = easing.EaseInOut(EasingType::Cubic, m_StartPos, m_EndPos, m_TotalTime, totaltime);
		m_AtPos = easing.EaseInOut(EasingType::Cubic, m_AtStartPos, m_AtEndPos, m_TotalTime, totaltime);
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(TgtPos);
		return false;
	}

	void OpeningCameraman::EndStateEnterBehavior() {
		auto ptrGameGtage = GetTypeStage<GameStage>();
		ptrGameGtage->ToMyCamera();
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
		if (Obj->ExcuteBehavior(2.0f))
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
		if (Obj->ExcuteBehavior(2.0f))
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

}
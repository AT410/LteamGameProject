/*!
@breif �ėp�I�u�W�F�N�g����
@name�@�쐬�ҁF�����B��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	
	void FixedObj::OnCreate()
	{
		DefaultSettings();
		SetActions();

		if (m_StartActionActive)
		{
			GetComponent<Collision>()->SetUpdateActive(false);
			GetComponent<Actions>()->Run(L"Start");
		}
	}

	void FixedObj::OnEvent(const shared_ptr<Event>&event)
	{
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
		if (event->m_MsgStr == L"TestEvent")
		{
			this->SetDrawActive(true);
			this->SetUpdateActive(true);
		}
	}

	//�����ȕǁF�O�ɗ����Ȃ��悤�ɂ��邽��
	void FrontWallObj::OnCreate()
	{
		//�Z�b�g������e�N�X�`���\�����ǂɂȂ邽�߃R�����g����
		//DefaultSettings();
		//�z�u�ݒ�@�����ȃe�N�X�`���o������폜
		auto TransComp = GetComponent<Transform>();
		TransComp->SetPosition(m_pos);
		TransComp->SetQuaternion(Quat(m_rot));
		TransComp->SetScale(m_scal);

		//��������
		auto CollComp = AddComponent<CollisionObb>();
		CollComp->SetFixed(true);
	}

	///<breif>�ėp�ړ��I�u�W�F�N�g<breif/>
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
		//�`��ݒ�
		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetMeshResource(m_meshKey);
		DrawComp->SetTextureResource(m_texKey);

		//�z�u�ݒ�
		auto TransComp = GetComponent<Transform>();
		TransComp->SetPosition(m_pos);
		TransComp->SetScale(m_scal);
		TransComp->SetQuaternion(Quat(m_rot));

		//��������
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
		//�ړ�����
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
	//�@�I�[�v�j���O�J�����}��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
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
	//������
	void OpeningCameraman::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptr = GetComponent<Transform>();
		ptr->SetScale(0.25f, 0.25f, 0.25f);	//���a25�Z���`�̋���
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(m_CreatePosEye);
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<OpeningCameraman>(GetThis<OpeningCameraman>()));
		//�ŏ��̃X�e�[�g��OpeningCameramanToGoalState�ɐݒ�
		m_StateMachine->ChangeState(OpeningCameramanToGoalState::Instance());
	}
	//����
	void OpeningCameraman::OnUpdate() {
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}

	void OpeningCameraman::ToGoalEnterBehavior() {
		m_AtStartPos = m_CreatePosAt;
		m_AtEndPos = Vec3(0.0f, 0.0f, 0.0f);
		m_AtPos = m_AtStartPos;
		m_TotalTime = 0.0f;

		auto PlayerPtr = GetStage()->GetSharedGameObject<GoalTest>(L"Goal");
		m_AtEndPos = PlayerPtr->GetComponent<Transform>()->GetPosition();


		// -- Eye�����߂� --
		Vec3 recVec = m_CreatePosEye*2.0f+m_AtEndPos;
		recVec /= 3.0f;
		m_EndEye = recVec;
	}

	void OpeningCameraman::ToStartEnterBehavior() {
		m_EndEye = m_CreatePosEye;
		m_AtStartPos = Vec3(0.0f, 0.0f, 0.0f);
		m_AtEndPos = m_CreatePosAt;
		m_TotalTime = 0.0f;

		auto PlayerPtr = GetStage()->GetSharedGameObject<GoalTest>(L"Goal");
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
		auto TgtPos = easing.EaseInOut(EasingType::Quintic, m_StartEye, m_EndEye, m_TotalTime, totaltime);
		m_AtPos = easing.EaseInOut(EasingType::Quintic, m_AtStartPos, m_AtEndPos, m_TotalTime, totaltime);
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

}
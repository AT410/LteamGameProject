/*!
@breif	�J�����N���X����
@name	�쐬�ҁF�����B��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	MyCamera::MyCamera() :
		m_TargetToAt(0.0f, 0.0f, 0.0f),
		m_ArmLen(10.0f),
		m_CameraJudge(false), Camera(), PawnBase()
	{}

	void MyCamera::SetEye(const bsm::Vec3& Eye) {
		Camera::SetEye(Eye);
	}

	void MyCamera::SetEye(float x, float y, float z) {
		Camera::SetEye(x, y, z);
	}

	void MyCamera::SetAt(const bsm::Vec3& At) {
		Camera::SetAt(At);
	}

	void MyCamera::SetAt(float x, float y, float z) {
		Camera::SetAt(x, y, z);
	}

	shared_ptr<GameObject> MyCamera::GetTargetObject() const {
		if (!m_TargetObj.expired()) {
			return m_TargetObj.lock();
		}
		return nullptr;
	}

	void MyCamera::SetTargetObject(const shared_ptr<GameObject>& Obj) {
		m_TargetObj = Obj;
	}

	Vec3 MyCamera::GetExpansionEye() const {
		return m_ExpansionEye;
	}

	void MyCamera::SetExpansionEye(const bsm::Vec3& ExpansionEye) {
		m_ExpansionEye = ExpansionEye;
	}

	Vec3 MyCamera::GetExpansionAt() const {
		return m_ExpansionAt;
	}

	void MyCamera::SetExpansionAt(const bsm::Vec3& ExpansionAt) {
		m_ExpansionAt = ExpansionAt;
	}

	void MyCamera::OnPushRB() {
		if (m_CameraJudge) {
			m_StateMachine->ChangeState(ExpansionState::Instance());
			m_CameraJudge = false;
		}
		else if (!m_CameraJudge) {
			m_StateMachine->ChangeState(FocusState::Instance());
			m_CameraJudge = true;
		}
	}

	void MyCamera::SetExpansion(const Vec3& Eye, const Vec3& At) {
		SetEye(Eye);
		SetAt(At);
	}

	void MyCamera::SetFocus(const Vec3& Eye, const Vec3& At) {
		SetEye(Eye);
		SetAt(At);
	}

	void MyCamera::ControlForcus()
	{
		auto Cont = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		float Val = -Cont.fThumbRY;
		m_ArmLen += Val;
		if (m_ArmLen < 5.0f)
		{
			m_ArmLen = 5.0f;
		}
		else if (m_ArmLen > 15.0f)
		{
			m_ArmLen = 15.0f;
		}
		Vec3 ptrPlayerPos = GetTargetObject()->GetComponent<Transform>()->GetWorldPosition();
		Vec3 Eye = Vec3(ptrPlayerPos.x, ptrPlayerPos.y + 2.0f, ptrPlayerPos.z - 6.0f);
		Vec3 At = Vec3(ptrPlayerPos.x, ptrPlayerPos.y, ptrPlayerPos.z);
		Vec3 SightVec = Eye - At;
		SightVec.normalize();
		SightVec *= m_ArmLen;
		SetFocus(Vec3(ptrPlayerPos.x, Eye.y+SightVec.y,Eye.z+SightVec.z), At);

	}

	void MyCamera::OnCreate() {
		m_ExpansionEye = GetEye();
		m_ExpansionAt = GetAt();
		m_StateMachine.reset(new StateMachine<MyCamera>(GetThis<MyCamera>()));
		m_StateMachine->ChangeState(ExpansionState::Instance());

	}
	void MyCamera::OnUpdate() {
		if (!GameManager::GetManager()->GetUpdateActive())
			return;
		m_handler.PushHandler(GetThis<MyCamera>());
		m_StateMachine->Update();
		Camera::OnUpdate();
	}

	//---------------------------------------------------
	///�ʏ�p�J�����X�e�[�g
	//---------------------------------------------------
	shared_ptr<ExpansionState> ExpansionState::Instance() {
		static shared_ptr<ExpansionState> instance(new ExpansionState);
		return instance;
	}

	void ExpansionState::Enter(const shared_ptr<MyCamera>& Cam) {
		Vec3 ptrEye = Cam->GetExpansionEye();
		Vec3 ptrAt = Cam->GetExpansionAt();
		Cam->SetExpansion(ptrEye, ptrAt);
	}
	void ExpansionState::Execute(const shared_ptr<MyCamera>& Cam) {

	}
	void ExpansionState::Exit(const shared_ptr<MyCamera>& Cam) {

	}


	//---------------------------------------------------
	///�L��J�����X�e�[�g
	//---------------------------------------------------
	shared_ptr<FocusState> FocusState::Instance() {
		static shared_ptr<FocusState> instance(new FocusState);
		return instance;
	}

	void FocusState::Enter(const shared_ptr<MyCamera>& Cam) {
	}
	void FocusState::Execute(const shared_ptr<MyCamera>& Cam) {
		Cam->ControlForcus();
	}
	void FocusState::Exit(const shared_ptr<MyCamera>& Cam) {

	}


	//--------------------------------------------------------------------------------------
//	�I�[�v�j���O�J�����i�R���|�[�l���g�ł͂Ȃ��j
//--------------------------------------------------------------------------------------
	OpeningCamera::OpeningCamera() :
		Camera()
	{}
	OpeningCamera::~OpeningCamera() {}

	void OpeningCamera::OnUpdate() {
		auto ptrOpeningCameraman = dynamic_pointer_cast<OpeningCameraman>(GetCameraObject());
		if (ptrOpeningCameraman) {
			auto pos = ptrOpeningCameraman->GetAtPos();
			SetAt(pos);
		}
		Camera::OnUpdate();
	}

}
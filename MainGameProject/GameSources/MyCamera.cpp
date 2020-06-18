/*!
@breif	�J�����N���X����
@name	�쐬�ҁF�����B��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	MyCamera::MyCamera() :
		m_ArmLen(10.0f),
		m_CameraJudge(false), Camera(), PawnBase()
	{}
	//�J�����Z�b�g�֐�
	//����@����
	//�J�����̈ʒu��������֐��iVec3�p�j
	void MyCamera::SetEye(const bsm::Vec3& Eye) {
		Camera::SetEye(Eye);
	}
	//�J�����Z�b�g�֐�
	//����@����
	//�J�����̈ʒu��������֐��iX���AY���AZ���p�j
	void MyCamera::SetEye(float x, float y, float z) {
		Camera::SetEye(x, y, z);
	}
	//�J�����ڕW�ʒu�Z�b�g�֐�
	//����@����
	//�J������������ʒu��������֐��iVec3�p�j
	void MyCamera::SetAt(const bsm::Vec3& At) {
		Camera::SetAt(At);
	}
	//�J�����ڕW�ʒu�Z�b�g�֐�
	//����@����
	//�J������������ʒu��������֐��iX���AY���AZ���p�j
	void MyCamera::SetAt(float x, float y, float z) {
		Camera::SetAt(x, y, z);
	}
	//�^�[�Q�b�g�擾�֐�
	//����@����
	//�^�[�Q�b�g�ƂȂ�I�u�W�F�N�g�̏����擾����֐�
	shared_ptr<GameObject> MyCamera::GetTargetObject() const {
		if (!m_TargetObj.expired()) {
			return m_TargetObj.lock();
		}
		return nullptr;
	}
	//�^�[�Q�b�g����֐�
	//����@����
	//�^�[�Q�b�g�ƂȂ�I�u�W�F�N�g�����߂�֐�
	void MyCamera::SetTargetObject(const shared_ptr<GameObject>& Obj) {
		m_TargetObj = Obj;
	}
	//�S��J�����ʒu�擾�֐�
	//����@����
	//�S��J�����̈ʒu���擾����֐�
	Vec3 MyCamera::GetExpansionEye() const {
		return m_ExpansionEye;
	}
	//�S��J�����ʒu����֐�
	//����@����
	//�S��J�����̈ʒu��������֐�
	void MyCamera::SetExpansionEye(const bsm::Vec3& ExpansionEye) {
		m_ExpansionEye = ExpansionEye;
	}
	//�S��J�����ڕW�ʒu�擾�֐�
	//����@����
	//�S��J�����̌��߂�ʒu���擾����֐�
	Vec3 MyCamera::GetExpansionAt() const {
		return m_ExpansionAt;
	}
	//�S��J�����ڕW�ʒu����֐�
	//����@����
	//�S��J�����̌��߂�ʒu��������֐�
	void MyCamera::SetExpansionAt(const bsm::Vec3& ExpansionAt) {
		m_ExpansionAt = ExpansionAt;
	}
	//�J�����X�e�[�g�ύX�֐�
	//����@����
	//RB�{�^�����������ƂŃJ�����̃X�e�[�g��ύX����֐�
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
	//�S��J��������֐�
	//����@����
	//�S��J�����̈ʒu�ƖڕW�ʒu��������
	void MyCamera::SetExpansion(const Vec3& Eye, const Vec3& At) {
		SetEye(Eye);
		SetAt(At);
	}
	//�ΏۏW���J��������֐�
	//����@����
	//�ΏۏW���J�����̈ʒu�ƖڕW�ʒu��������
	void MyCamera::SetFocus(const Vec3& Eye, const Vec3& At) {
		SetEye(Eye);
		SetAt(At);
	}
	//�ΏۏW���J�����R���g���[���֐�
	//
	//�ΏۏW���J�����ƖڕW�̋����𑀍삷��֐�
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
	///�S��p�J�����X�e�[�g
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
	///�ΏۏW���J�����X�e�[�g
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
	//	�I�[�v�j���O�J����
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
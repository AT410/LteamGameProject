#include"stdafx.h"
#include"Project.h"
namespace basecross {
	MyCamera::MyCamera():
		m_TargetToAt(0.0f, 0.0f, 0.0f),
		m_ArmLen(5.0f)
	{}

	void MyCamera::SetEye(const bsm::Vec3& Eye) {
		Camera::SetEye(Eye);
	}

	void MyCamera::SetEye(float x, float y, float z) {
		Camera::SetEye(x, y, z);
	}

	void MyCamera::SetAt(const bsm::Vec3& At) {
		Camera::SetAt(At);
		Vec3 armVec = GetEye() - GetAt();
		armVec.normalize();
		armVec *= m_ArmLen;
		Vec3 newEye = GetAt() + armVec;
		Camera::SetEye(newEye);
	}

	void MyCamera::SetAt(float x, float y, float z) {
		Camera::SetAt(x,y,z);
		Vec3 armVec = GetEye() - GetAt();
		armVec.normalize();
		armVec *= m_ArmLen;
		Vec3 newEye = GetAt() + armVec;
		Camera::SetEye(newEye);

	}

	void MyCamera::OnPushRB() {
		if (m_CameraJudge) {
			m_StateMachine->ChangeState(ExpansionState::Instance());
			m_CameraJudge = false;
		}
		else if (!m_CameraJudge) {
			m_StateMachine->ChangeState(FocusState::Instatnce());
			m_CameraJudge = true;
		}
	}
	void MyCamera::OnCreate() {
		m_StateMachine->ChangeState(ExpansionState::Instance());
	}
	void MyCamera::OnUpdate(){
		m_StateMachine->Update();
		Camera::OnUpdate();
	}

	shared_ptr<ExpansionState> ExpansionState::Instance() {
		static shared_ptr<ExpansionState> instance(new ExpansionState);
		return instance;
	}
	
	void ExpansionState::Enter(const shared_ptr<MyCamera>& Cam) {

	}
	void ExpansionState::Execute(const shared_ptr<MyCamera>& Cam) {
		Cam->SetEye(0.0f, 0.0f, -10.0f);
		Cam->SetAt(0.0f, 0.0f, 0.0f);
	}
	void ExpansionState::Exit(const shared_ptr<MyCamera>& Cam) {

	}

	shared_ptr<FocusState> FocusState::Instatnce() {
		static shared_ptr<FocusState> instance(new FocusState);
		return instance;
	}

	void FocusState::Enter(const shared_ptr<MyCamera>& Cam) {

	}
	void FocusState::Execute(const shared_ptr<MyCamera>& Cam) {
		Cam->SetAt(0.0f, 0.0f, 0.0f);
	}
	void FocusState::Exit(const shared_ptr<MyCamera>& Cam) {

	}
};
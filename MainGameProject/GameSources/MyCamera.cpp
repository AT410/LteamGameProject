/*!
@breif	�J�����N���X����
@name	�쐬�ҁF�����B��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	MyCamera::MyCamera()
		:Camera()
	{

	}

	MyCamera::~MyCamera(){}	

	void MyCamera::OnCreate()
	{
		m_StateMachine.reset(new StateMachine<MyCamera>(GetThis<MyCamera>()));
		m_StateMachine->ChangeState(NormalCameraMode::Instance());
	}

	void MyCamera::OnUpdate()
	{
		m_StateMachine->Update();
	}

	//R�X�e�B�b�N�����ݏ���
	void MyCamera::OnPushR3()
	{
		//�J���������̐؂�ւ����s��
		
	}

	//---------------------------------------------------
	///�ʏ�p�J�����X�e�[�g
	//---------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(NormalCameraMode);

	void NormalCameraMode::Enter(const shared_ptr<MyCamera>& Obj)
	{

	}

	void NormalCameraMode::Execute(const shared_ptr<MyCamera>& Obj)
	{

	}

	void NormalCameraMode::Exit(const shared_ptr<MyCamera>& Obj)
	{

	}


	//---------------------------------------------------
	///�L��J�����X�e�[�g
	//---------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(AreaCameraMode);

	void AreaCameraMode::Enter(const shared_ptr<MyCamera>& Obj)
	{

	}

	void AreaCameraMode::Execute(const shared_ptr<MyCamera>& Obj)
	{

	}

	void AreaCameraMode::Exit(const shared_ptr<MyCamera>& Obj)
	{

	}


	//---------------------------------------------------
	///�f�o�b�N�p�J�����X�e�[�g
	//---------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(DebugCameraMode);

	void DebugCameraMode::Enter(const shared_ptr<MyCamera>& Obj)
	{

	}

	void DebugCameraMode::Execute(const shared_ptr<MyCamera>& Obj)
	{

	}

	void DebugCameraMode::Exit(const shared_ptr<MyCamera>& Obj)
	{

	}
}
/*!
@breif	カメラクラス実体
@name	作成者：阿部達哉
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

	//Rスティック押込み処理
	void MyCamera::OnPushR3()
	{
		//カメラ距離の切り替えを行う
		
	}

	//---------------------------------------------------
	///通常用カメラステート
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
	///広域カメラステート
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
	///デバック用カメラステート
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
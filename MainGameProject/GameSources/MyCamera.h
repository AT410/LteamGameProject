/*!
@breif カメラクラス
@name　作成者：阿部達哉
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	class MyCamera :public Camera, public PawnBase<MyCamera>
	{
	public:
		MyCamera();
		virtual ~MyCamera();

		void OnCreate()override;
		void OnUpdate()override;

		void OnPushR3()override;

		//視野角移動
		void AngleUpdate();

	private:
		unique_ptr<StateMachine<MyCamera>>m_StateMachine;
	};

	///通常カメラステート
	class NormalCameraMode :public ObjState<MyCamera>
	{
	private:
		NormalCameraMode() {}
	public:
		DECLARE_SINGLETON_INSTANCE(NormalCameraMode);
		void Enter(const shared_ptr<MyCamera>& Obj)override;
		void Execute(const shared_ptr<MyCamera>& Obj)override;
		void Exit(const shared_ptr<MyCamera>& Obj)override;
	};
	

	///広域カメラステート
	class AreaCameraMode :public ObjState<MyCamera>
	{
	private:
		AreaCameraMode() {}
	public:
		DECLARE_SINGLETON_INSTANCE(AreaCameraMode);
		void Enter(const shared_ptr<MyCamera>& Obj)override;
		void Execute(const shared_ptr<MyCamera>& Obj)override;
		void Exit(const shared_ptr<MyCamera>& Obj)override;
	};


	///Debug用カメラステート
	class DebugCameraMode :public ObjState<MyCamera>
	{
	private:
		DebugCameraMode(){}
	public:
		DECLARE_SINGLETON_INSTANCE(DebugCameraMode)
		void Enter(const shared_ptr<MyCamera>& Obj)override;
		void Execute(const shared_ptr<MyCamera>& Obj)override;
		void Exit(const shared_ptr<MyCamera>& Obj)override;
	};
}

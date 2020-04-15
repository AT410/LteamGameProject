/*!
@breif �J�����N���X
@name�@�쐬�ҁF�����B��
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

		//����p�ړ�
		void AngleUpdate();

	private:
		unique_ptr<StateMachine<MyCamera>>m_StateMachine;
	};

	///�ʏ�J�����X�e�[�g
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
	

	///�L��J�����X�e�[�g
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


	///Debug�p�J�����X�e�[�g
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

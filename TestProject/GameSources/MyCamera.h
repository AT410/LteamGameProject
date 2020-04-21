//@breif ゲーム画面用のカメラ
//@name 松崎　洸樹

#pragma once
#include"stdafx.h"

namespace basecross {
	class MyCamera :public Camera, public PawnBase<MyCamera>{
		unique_ptr< StateMachine<MyCamera> > m_StateMachine;
		weak_ptr<GameObject> m_TargetObj;
		//shared_ptr<GameObject> m_Obj;
		bsm::Vec3 m_TargetToAt;
		float m_ArmLen;
		bool m_CameraJudge;
	public:
		MyCamera();
		virtual void SetEye(const bsm::Vec3& Eye)override;
		virtual void SetEye(float x, float y, float z)override;
		virtual void SetAt(const bsm::Vec3& At)override;
		virtual void SetAt(float x, float y, float z)override;
		void OnPushRB()override;
		virtual ~MyCamera() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	class ExpansionState : public ObjState<MyCamera> {
		ExpansionState() {}
	public:
		static shared_ptr<ExpansionState> Instance();
		virtual void Enter(const shared_ptr<MyCamera>& Cam)override;
		virtual void Execute(const shared_ptr<MyCamera>& Cam)override;
		virtual void Exit(const shared_ptr<MyCamera>& Cam)override;
	};

	class FocusState : public ObjState<MyCamera> {
		FocusState() {}
	public:
		static shared_ptr<FocusState> Instatnce();
		virtual void Enter(const shared_ptr<MyCamera>& Cam)override;
		virtual void Execute(const shared_ptr<MyCamera>& Cam)override;
		virtual void Exit(const shared_ptr<MyCamera>& Cam)override;
	};
}
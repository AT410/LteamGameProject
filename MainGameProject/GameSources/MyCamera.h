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
		//カメラ用ステートマシン
		unique_ptr< StateMachine<MyCamera> > m_StateMachine;
		//目標となるオブジェクト
		weak_ptr<GameObject> m_TargetObj;
		//対象との距離
		float m_ArmLen;
		//カメラ状態の切り替え
		bool m_CameraJudge;
		//全域用カメラ位置
		Vec3 m_ExpansionEye;
		//全域用カメラ目標位置
		Vec3 m_ExpansionAt;
		//対象集中カメラ位置
		Vec3 m_FocusEye;
		//対象集中カメラ目標位置
		Vec3 m_FocusAt;
	public:
		MyCamera();
		virtual ~MyCamera() {}
		virtual void SetEye(const bsm::Vec3& Eye)override;
		virtual void SetEye(float x, float y, float z)override;
		virtual void SetAt(const bsm::Vec3& At)override;
		virtual void SetAt(float x, float y, float z)override;
		shared_ptr<GameObject> GetTargetObject() const;
		void SetTargetObject(const shared_ptr<GameObject>& Obj);
		Vec3 GetExpansionEye() const;
		void SetExpansionEye(const bsm::Vec3& ExpansionEye);
		Vec3 GetExpansionAt() const;
		void SetExpansionAt(const bsm::Vec3& ExpansionAt);
		void SetExpansion(const Vec3& Eye, const Vec3& At);
		void SetFocus(const Vec3& Eye, const Vec3& At);
		void ControlForcus();
		void OnPushRB() override;
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	///通常カメラステート
	class ExpansionState : public ObjState<MyCamera> {
		ExpansionState() {}
	public:
		static shared_ptr<ExpansionState> Instance();
		virtual void Enter(const shared_ptr<MyCamera>& Cam)override;
		virtual void Execute(const shared_ptr<MyCamera>& Cam)override;
		virtual void Exit(const shared_ptr<MyCamera>& Cam)override;
	};


	///広域カメラステート
	class FocusState : public ObjState<MyCamera> {
		FocusState() {}
	public:
		static shared_ptr<FocusState> Instance();
		virtual void Enter(const shared_ptr<MyCamera>& Cam)override;
		virtual void Execute(const shared_ptr<MyCamera>& Cam)override;
		virtual void Exit(const shared_ptr<MyCamera>& Cam)override;
	};

	//--------------------------------------------------------------------------------------
	//　@breif ここより上述のMyCamera・MyCameraステート作成
	//　@name 松崎　洸樹
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//	イベントカメラ
	//--------------------------------------------------------------------------------------
	///<name>作成者：阿部達哉</name>
	class EventCamera :public Camera
	{
	public:
		EventCamera();

		virtual~EventCamera();

		virtual void OnUpdate()override;
	};


}

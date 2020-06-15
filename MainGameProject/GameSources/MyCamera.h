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
		unique_ptr< StateMachine<MyCamera> > m_StateMachine;
		weak_ptr<GameObject> m_TargetObj;
		shared_ptr<GameObject> m_Obj;
		bsm::Vec3 m_TargetToAt;
		float m_ArmLen;
		bool m_CameraJudge;
		Vec3 m_ExpansionEye;
		Vec3 m_ExpansionAt;
		Vec3 m_FocusEye;
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

	///�ʏ�J�����X�e�[�g
	class ExpansionState : public ObjState<MyCamera> {
		ExpansionState() {}
	public:
		static shared_ptr<ExpansionState> Instance();
		virtual void Enter(const shared_ptr<MyCamera>& Cam)override;
		virtual void Execute(const shared_ptr<MyCamera>& Cam)override;
		virtual void Exit(const shared_ptr<MyCamera>& Cam)override;
	};


	///�L��J�����X�e�[�g
	class FocusState : public ObjState<MyCamera> {
		FocusState() {}
	public:
		static shared_ptr<FocusState> Instance();
		virtual void Enter(const shared_ptr<MyCamera>& Cam)override;
		virtual void Execute(const shared_ptr<MyCamera>& Cam)override;
		virtual void Exit(const shared_ptr<MyCamera>& Cam)override;
	};

	//--------------------------------------------------------------------------------------
	//�@@breif ��������q��MyCamera�EMyCamera�X�e�[�g�쐬
	//�@@name ����@����
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//	�I�[�v�j���O�J����
	//--------------------------------------------------------------------------------------
	class OpeningCamera :public Camera
	{
	public:
		OpeningCamera();

		virtual~OpeningCamera();

		virtual void OnUpdate()override;
	};


}

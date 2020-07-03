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
		//�J�����p�X�e�[�g�}�V��
		unique_ptr< StateMachine<MyCamera> > m_StateMachine;
		//�ڕW�ƂȂ�I�u�W�F�N�g
		weak_ptr<GameObject> m_TargetObj;
		//�ΏۂƂ̋���
		float m_ArmLen;
		//�J������Ԃ̐؂�ւ�
		bool m_CameraJudge;
		//�S��p�J�����ʒu
		Vec3 m_ExpansionEye;
		//�S��p�J�����ڕW�ʒu
		Vec3 m_ExpansionAt;
		//�ΏۏW���J�����ʒu
		Vec3 m_FocusEye;
		//�ΏۏW���J�����ڕW�ʒu
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
	//	�C�x���g�J����
	//--------------------------------------------------------------------------------------
	///<name>�쐬�ҁF�����B��</name>
	class EventCamera :public Camera
	{
	public:
		EventCamera();

		virtual~EventCamera();

		virtual void OnUpdate()override;
	};


}

/*!
@breif �ėp�I�u�W�F�N�g����
@name�@�쐬�ҁF�����B��
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	///<breif>�ėp�Œ�I�u�W�F�N�g</breif>
	///<name>�쐬�ҁF�����B��</name>
	class FixedObj :public ObjectBase
	{
	public:
		FixedObj(const shared_ptr<Stage>&StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
				const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr,Position,Rotation,Scale,TexKey,MeshKey){}

		FixedObj(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr,pNode){}

		virtual ~FixedObj(){}

		void OnCreate()override;

		void OnEvent(const shared_ptr<Event>&event)override;
	};

	///<breif>�ėp�Œ�I�u�W�F�N�g<breif/>
	///<name>�쐬�ҁF�����B��<name/>
	class LoopTexObj :public ObjectBase
	{
	public:
		LoopTexObj(const shared_ptr<Stage>&StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey) {}

		LoopTexObj(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}

		virtual ~LoopTexObj() {}

		void OnCreate()override;

		void OnEvent(const shared_ptr<Event>&event)override;
	};


	//�쐬�F�ɓ��ˌ�
	//�O�ʂ̓����ȕ�
	class FrontWallObj :public ObjectBase
	{
	public:
		FrontWallObj(const shared_ptr<Stage>&StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey) {}

		FrontWallObj(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}

		virtual ~FrontWallObj() {}

		void OnCreate()override;
	};

	//----------------------------------------------------------------------------
	//��{���̓N���X
	//----------------------------------------------------------------------------
	class DefaultInput :public ObjectBase, public PawnBase<DefaultInput>
	{
	public:
		DefaultInput(const shared_ptr<Stage>&StagePtr, const wstring& StageStr)
			:ObjectBase(StagePtr), PawnBase(), m_StageStr(StageStr), m_IsSend(false)
		{}
		virtual ~DefaultInput() {}

		void OnCreate()override {}

		void OnUpdate()override;

		void OnPushA()override;

	private:
		wstring m_StageStr;

		bool m_IsSend;
	};


	//----------------------------------------------------------------------------
	//�J�����}���N���X:�J�����̎��͎̂����Ȃ�
	//----------------------------------------------------------------------------
	class CameraMan :public GameObject
	{
	public:
		//�\�z�Ɣj��
		CameraMan(const shared_ptr<Stage>& StagePtr,const Vec3& StartPos,const Vec3& AtPos);
		virtual ~CameraMan();
		//������
		virtual void OnCreate()override;
		//����
		virtual void OnUpdate()override;
		void OnEvent(const shared_ptr<Event>&event)override;
		//�A�N�Z�T
		const unique_ptr<StateMachine<CameraMan>>& GetStateMachine()
		{
			return m_StateMachine;
		}

		Vec3 GetAtPos() const
		{
			return m_AtPos;
		}

		void ToGoalEnterBehavior();
		void ToStartEnterBehavior();
		bool ExcuteBehavior(float TotalTime);
		void EndStateEnterBehavior();
		// -- ��������N���A���� --
		void ClearStateEnterBehavior();
		void ToClearMoveEnterBehavior();

		void EventStart();
	private:
		// -- �ړ��ϐ� --
		Vec3 m_StartEye;
		Vec3 m_EndEye;

		Vec3 m_CreatePosEye;
		Vec3 m_CreatePosAt;

		Vec3 m_AtStartPos;
		Vec3 m_AtEndPos;
		Vec3 m_AtPos;
		float m_TotalTime;
		//�X�e�[�g�}�V�[��
		unique_ptr< StateMachine<CameraMan> >  m_StateMachine;

		wstring m_MsgEvent;

	};

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanToGoalState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	class CameramanToGoalState : public ObjState<CameraMan>
	{
		CameramanToGoalState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(CameramanToGoalState)
		virtual void Enter(const shared_ptr<CameraMan>&Obj)override;
		virtual void Execute(const shared_ptr<CameraMan>& Obj)override;
		virtual void Exit(const shared_ptr<CameraMan>& Obj)override;

	};

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanToStartState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	class CameramanToStartState : public ObjState<CameraMan>
	{
		CameramanToStartState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(CameramanToStartState)
		virtual void Enter(const shared_ptr<CameraMan>&Obj)override;
		virtual void Execute(const shared_ptr<CameraMan>& Obj)override;
		virtual void Exit(const shared_ptr<CameraMan>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanEndState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	class CameramanEndState : public ObjState<CameraMan>
	{
		CameramanEndState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(CameramanEndState)
		virtual void Enter(const shared_ptr<CameraMan>&Obj)override;
		virtual void Execute(const shared_ptr<CameraMan>& Obj)override;
		virtual void Exit(const shared_ptr<CameraMan>& Obj)override;

	};

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanEndState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	class CameramanClearState : public ObjState<CameraMan>
	{
		CameramanClearState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(CameramanClearState)
		virtual void Enter(const shared_ptr<CameraMan>&Obj)override;
		virtual void Execute(const shared_ptr<CameraMan>& Obj)override;
		virtual void Exit(const shared_ptr<CameraMan>& Obj)override;

	};

	//--------------------------------------------------------------------------------------
	//	�J�����ҋ@�X�e�[�g�F��{�I�ɉ������Ȃ�
	//--------------------------------------------------------------------------------------
	class CameraNoneState : public ObjState<CameraMan>
	{
		CameraNoneState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(CameraNoneState)
		virtual void Enter(const shared_ptr<CameraMan>&Obj)override {}
		virtual void Execute(const shared_ptr<CameraMan>& Obj)override {}
		virtual void Exit(const shared_ptr<CameraMan>& Obj)override {}

	};

}
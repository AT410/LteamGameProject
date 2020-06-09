/*!
@breif �ėp�I�u�W�F�N�g����
@name�@�쐬�ҁF�����B��
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	///<breif>�ėp�Œ�I�u�W�F�N�g<breif/>
	///<name>�쐬�ҁF�����B��<name/>
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

	///<breif>�ėp�ړ��I�u�W�F�N�g<breif/>
	///<name>�쐬�ҁF�����B��<name/>
	enum class MovingType
	{
		Position,
		Rotation,
		Scaling
	};

	class MoveObj :public ObjectBase
	{
	public:
		MoveObj(const shared_ptr<Stage>&StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey,const Vec3 Start,const Vec3 End,const float Speed,const float TotalTime);

		MoveObj(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		virtual ~MoveObj();

		//�����ݒ�
		void OnCreate()override;

		//�X�V
		void OnUpdate()override;

	private:
		void PosMove();
		bool LerpMove(Vec3 Start,Vec3 end);

		void RotMove();

		MovingType m_Type;
		float m_speed;
		float m_Movetime;
		float m_CurrntTime;
		Vec3 m_Start;
		Vec3 m_End;
	};

	//OpeningCameraman
	class OpeningCameraman :public GameObject
	{
	public:
		//�\�z�Ɣj��
		OpeningCameraman(const shared_ptr<Stage>& StagePtr,const Vec3& StartPos,const Vec3& AtPos);
		virtual ~OpeningCameraman();
		//������
		virtual void OnCreate()override;
		//����
		virtual void OnUpdate()override;
		void OnEvent(const shared_ptr<Event>&event)override;
		//�A�N�Z�T
		const unique_ptr<StateMachine<OpeningCameraman>>& GetStateMachine()
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
		unique_ptr< StateMachine<OpeningCameraman> >  m_StateMachine;

		wstring m_MsgEvent;

	};

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanToGoalState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	class OpeningCameramanToGoalState : public ObjState<OpeningCameraman>
	{
		OpeningCameramanToGoalState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(OpeningCameramanToGoalState)
		virtual void Enter(const shared_ptr<OpeningCameraman>&Obj)override;
		virtual void Execute(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Exit(const shared_ptr<OpeningCameraman>& Obj)override;

	};

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanToStartState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	class OpeningCameramanToStartState : public ObjState<OpeningCameraman>
	{
		OpeningCameramanToStartState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(OpeningCameramanToStartState)
		virtual void Enter(const shared_ptr<OpeningCameraman>&Obj)override;
		virtual void Execute(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Exit(const shared_ptr<OpeningCameraman>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanEndState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	class OpeningCameramanEndState : public ObjState<OpeningCameraman>
	{
		OpeningCameramanEndState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(OpeningCameramanEndState)
		virtual void Enter(const shared_ptr<OpeningCameraman>&Obj)override;
		virtual void Execute(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Exit(const shared_ptr<OpeningCameraman>& Obj)override;

	};

	//--------------------------------------------------------------------------------------
	//	class OpeningCameramanEndState : public ObjState<OpeningCameraman>;
	//--------------------------------------------------------------------------------------
	class CameramanClearState : public ObjState<OpeningCameraman>
	{
		CameramanClearState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(CameramanClearState)
		virtual void Enter(const shared_ptr<OpeningCameraman>&Obj)override;
		virtual void Execute(const shared_ptr<OpeningCameraman>& Obj)override;
		virtual void Exit(const shared_ptr<OpeningCameraman>& Obj)override;

	};

	//--------------------------------------------------------------------------------------
	//	�J�����ҋ@�X�e�[�g�F��{�I�ɉ������Ȃ�
	//--------------------------------------------------------------------------------------
	class CameraNoneState : public ObjState<OpeningCameraman>
	{
		CameraNoneState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(CameraNoneState)
		virtual void Enter(const shared_ptr<OpeningCameraman>&Obj)override {}
		virtual void Execute(const shared_ptr<OpeningCameraman>& Obj)override {}
		virtual void Exit(const shared_ptr<OpeningCameraman>& Obj)override {}

	};

}
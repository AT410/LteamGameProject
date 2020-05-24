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
	class PullBoxObj :public ObjectBase
	{
	public:
		PullBoxObj(const shared_ptr<Stage>&StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey) {}

		PullBoxObj(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}

		virtual ~PullBoxObj() {}

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

	private:
		Vec3 m_StartPos;
		Vec3 m_EndPos;
		Vec3 m_AtStartPos;
		Vec3 m_AtEndPos;
		Vec3 m_AtPos;
		float m_TotalTime;
		//�X�e�[�g�}�V�[��
		unique_ptr< StateMachine<OpeningCameraman> >  m_StateMachine;

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

}
/*!
@breif 汎用オブジェクト実体
@name　作成者：阿部達哉
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	///<breif>汎用固定オブジェクト<breif/>
	///<name>作成者：阿部達哉<name/>
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

	///<breif>汎用固定オブジェクト<breif/>
	///<name>作成者：阿部達哉<name/>
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


	///<breif>汎用移動オブジェクト<breif/>
	///<name>作成者：阿部達哉<name/>
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

		//初期設定
		void OnCreate()override;

		//更新
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
		//構築と破棄
		OpeningCameraman(const shared_ptr<Stage>& StagePtr,const Vec3& StartPos,const Vec3& AtPos);
		virtual ~OpeningCameraman();
		//初期化
		virtual void OnCreate()override;
		//操作
		virtual void OnUpdate()override;
		//アクセサ
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
		//ステートマシーン
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
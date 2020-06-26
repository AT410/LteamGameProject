/*!
@breif 汎用オブジェクト実体
@name　作成者：阿部達哉
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	///<breif>汎用固定オブジェクト</breif>
	///<name>作成者：阿部達哉</name>
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


	//作成：伊東祥吾
	//前面の透明な壁
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
	//基本入力クラス
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
	//カメラマンクラス:カメラの実体は持たない
	//----------------------------------------------------------------------------
	class CameraMan :public GameObject
	{
	public:
		//構築と破棄
		CameraMan(const shared_ptr<Stage>& StagePtr,const Vec3& StartPos,const Vec3& AtPos);
		virtual ~CameraMan();
		//初期化
		virtual void OnCreate()override;
		//操作
		virtual void OnUpdate()override;
		void OnEvent(const shared_ptr<Event>&event)override;
		//アクセサ
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
		// -- ここからクリア処理 --
		void ClearStateEnterBehavior();
		void ToClearMoveEnterBehavior();

		void EventStart();
	private:
		// -- 移動変数 --
		Vec3 m_StartEye;
		Vec3 m_EndEye;

		Vec3 m_CreatePosEye;
		Vec3 m_CreatePosAt;

		Vec3 m_AtStartPos;
		Vec3 m_AtEndPos;
		Vec3 m_AtPos;
		float m_TotalTime;
		//ステートマシーン
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
	//	カメラ待機ステート：基本的に何もしない
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
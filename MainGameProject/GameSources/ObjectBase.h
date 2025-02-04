/*!
@breif	オブジェクトの基底クラス
@name	作成者：阿部達哉
*/
#pragma once
#include "stdafx.h"
#include "ProjectUtility.h"

namespace basecross
{
	//---------------------------------------------------
	//配置オブジェクトの基底クラス
	//---------------------------------------------------
	class ObjectBase :public GameObject
	{
	public:
		ObjectBase(const shared_ptr<Stage>&StagePtr)
			:GameObject(StagePtr) {}

		ObjectBase(const shared_ptr<Stage>&StagePtr,const Vec3 Position,const Vec3 Rotation,const Vec3 Scale,
					const wstring TexKey,const wstring MeshKey)
			:GameObject(StagePtr),m_pos(Position),m_rot(Rotation,1.0f),m_scal(Scale),m_texKey(TexKey),m_meshKey(MeshKey) {}

		ObjectBase(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		virtual ~ObjectBase() {}

		//初期化
		virtual void OnCreate()override = 0;
		//更新処理
		virtual void OnUpdate()override {};

		// -- イベント処理 --
		virtual void OnEvent(const shared_ptr<Event>& event)override;

	protected:
		// -- 基本設定関数 --
		void DefaultSettings();

		// -- アクションの追加 --
		void SetActions();

		Vec3 m_pos;
		Vec4 m_rot;
		Vec3 m_scal;
		wstring m_texKey;
		wstring m_meshKey;

		vector<wstring> m_tag;

		bool m_CollisionActive = false;
		bool m_CollisionSetFixed = false;

		bool m_SharedActive = false;
		wstring m_SharedName;
		// -- イベント設定 --
		bool m_EventActive = false;
		wstring m_ReceiverKey;

		// -- アニメーション設定 --
		IXMLDOMNodeListPtr m_AnimationNodes;
		bool m_AnimationActive = false;
		bool m_StartActionActive = false;
		bool m_IsStartActionLoop = false;

		bool m_EventActionActive = false;
		bool m_IsEventActionLoop = false;

		bool m_EndActionActive = false;
		bool m_IsEndActionLoop = false;

	};

	//---------------------------------------------------
	//入力操作可能クラスの基底クラス
	//---------------------------------------------------
	template <typename T>
	class PawnBase
	{
	public:
		PawnBase(){}

		virtual ~PawnBase() {}

		virtual void OnPushA() {};
		virtual void OnPushB() {};
		virtual void OnPushX() {};
		virtual void OnPushY() {};
		virtual void OnPushR3() {};
		virtual void OnPushL3() {};
		virtual void OnPushLB() {};
		virtual void OnRemoveLB() {};
		virtual void OnPushRB() {};
	protected:
		InputHandler<T> m_handler;
	};
}
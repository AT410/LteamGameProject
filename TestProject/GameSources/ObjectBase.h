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
		virtual ~ObjectBase() {}

		//初期化
		virtual void OnCreate()override = 0;
		//更新処理
		virtual void OnUpdate()override = 0;
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
		virtual void OnPushLB() {};
		virtual void OnRemoveLB() {};
		virtual void OnPushRB() {};
	protected:
		InputHandler2<T> m_Handler;
	};
}
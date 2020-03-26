/*!
@breif	オブジェクトの基底クラス
@name	作成者：阿部達哉
*/
#pragma once
#include "stdafx.h"

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
	class PawnBase :public ObjectBase
	{
	public:
		PawnBase(const shared_ptr<Stage>&StagePtr)
			:ObjectBase(StagePtr) {}
		virtual ~PawnBase() {}

		virtual void OnPushA() = 0;
		virtual void OnPushB() = 0;
		virtual void OnPushX() = 0;
		virtual void OnPushY() = 0;
		
		//初期化
		virtual void OnCreate()override =0;
		//更新処理
		virtual void OnUpdate()override =0;
	protected:
		
	};
}
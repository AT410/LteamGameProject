#pragma once
#include "stdafx.h"

namespace basecross {
	class Uictr : public GameObject // Stageに追加したいオブジェクトは、GameObjectクラスの継承でなければならない
	{
	public:
		Uictr(const std::shared_ptr<Stage>& stage) // 必ずStageを受け取るコンストラクタを作らなければならない
			: GameObject(stage) // 必ず親クラスのコンストラクタにStageを渡して初期化しなければならない
		{
		}

		void OnCreate() override; // オブジェクトへのコンポーネントの追加はOnCreate関数をオーバーライドして、その中で行う
	};
}
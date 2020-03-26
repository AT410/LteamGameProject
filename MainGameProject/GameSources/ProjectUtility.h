/*!
@breif	プロジェクト定義の追加関数・機能
@name	作成者：阿部達哉
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	//---------------------------------
	//コントローラー操作用ハンドラ
	//---------------------------------
	template<typename T>
	struct InputHandller
	{
		void PushHandler(const shared_ptr<T>& Obj)
		{
			auto Input = App::GetApp()->GetInputDevice().GetControlerVec()[0];
			if (Input.bConnected)
			{
				if (Input.wPressedButtons == XINPUT_GAMEPAD_A)
				{
					Obj->OnPushA();
				}
			}
		}
	};
}
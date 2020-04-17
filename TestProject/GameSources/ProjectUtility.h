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
	struct InputHandler2
	{
		//Vec2 m_LStickVol;//Lスティック
		//Vec2 m_RStickVol;//Rスティック

		//bool m_ConvertStickToDbadActoive;

		void PushHandler(const shared_ptr<T>& Obj)
		{
			auto InputDevice = App::GetApp()->GetInputDevice();
			auto ContInput = InputDevice.GetControlerVec()[0];
			auto KeyInput = InputDevice.GetKeyState();
			if (ContInput.bConnected)
			{
				/*!
				@breif　決定ボタンorジャンプ
				*/
				if (ContInput.wPressedButtons == XINPUT_GAMEPAD_A)
				{
					Obj->OnPushA();
				}
				if (ContInput.wButtons == XINPUT_GAMEPAD_LEFT_SHOULDER) 
				{
					Obj->OnPushLB();
				}
				else {
					Obj->OnRemoveLB();
				}
			}
		}
	};
}
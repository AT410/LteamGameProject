/*!
@breif	�v���W�F�N�g��`�̒ǉ��֐��E�@�\
@name	�쐬�ҁF�����B��
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	//---------------------------------
	//�R���g���[���[����p�n���h��
	//---------------------------------
	template<typename T>
	struct InputHandler2
	{
		//Vec2 m_LStickVol;//L�X�e�B�b�N
		//Vec2 m_RStickVol;//R�X�e�B�b�N

		//bool m_ConvertStickToDbadActoive;

		void PushHandler(const shared_ptr<T>& Obj)
		{
			auto InputDevice = App::GetApp()->GetInputDevice();
			auto ContInput = InputDevice.GetControlerVec()[0];
			auto KeyInput = InputDevice.GetKeyState();
			if (ContInput.bConnected)
			{
				/*!
				@breif�@����{�^��or�W�����v
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
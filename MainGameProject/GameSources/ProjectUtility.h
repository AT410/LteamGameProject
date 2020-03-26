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
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
	struct InputHandler
	{
		//Vec2 m_LStickVol;//L�X�e�B�b�N
		//Vec2 m_RStickVol;//R�X�e�B�b�N

		WORD wDpad = 0;
		WORD wPressedDpad = 0;
		WORD wNowStick;
		WORD wLastStick = 0;
		bool m_ConvertStickToDbadActoive= false;
		bool m_ConvertStickToDbadActoiveDown = false;
		bool m_ConvertStickToDbadActoiveLeft = false;
		bool m_ConvertStickToDbadActoiveRight = false;

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

				//�X�e�B�b�N������
				if (ContInput.wPressedButtons == XINPUT_GAMEPAD_RIGHT_THUMB)
				{
					Obj->OnPushR3();
				}

				if (ContInput.wButtons == XINPUT_GAMEPAD_LEFT_SHOULDER)
				{
					Obj->OnPushLB();
				}
				else {
					Obj->OnRemoveLB();
				}
				if (ContInput.wPressedButtons == XINPUT_GAMEPAD_RIGHT_SHOULDER) {
					Obj->OnPushRB();
				}
			}
		}

		WORD CovertToDPAD(const Vec2 Stick)
		{
			// -- ��t���[���O --
			wLastStick = wDpad;

			wDpad = 0;
			if (Stick.x > 0.5f)
			{
				wDpad |= XINPUT_GAMEPAD_DPAD_RIGHT;
			}
			else if (Stick.x < -0.5f)
			{
				wDpad |= XINPUT_GAMEPAD_DPAD_LEFT;
			}

			if (Stick.y > 0.5f)
			{
				wDpad |= XINPUT_GAMEPAD_DPAD_UP;
			}
			else if (Stick.y < -0.5f)
			{
				wDpad |= XINPUT_GAMEPAD_DPAD_DOWN;
			}

			if (wDpad != wLastStick && m_ConvertStickToDbadActoive)
				m_ConvertStickToDbadActoive = false;

			if (m_ConvertStickToDbadActoive)
			{
				return 0;
			}
			else
			{
				m_ConvertStickToDbadActoive = true;
				wLastStick = wDpad;
				return wDpad;
			}
		}

		bool GetUpArrow(const WORD DPAD)
		{
			auto InputDevice = App::GetApp()->GetInputDevice();
			auto ContInput = InputDevice.GetControlerVec()[0];

			if (ContInput.wPressedButtons == XINPUT_GAMEPAD_DPAD_UP || DPAD == XINPUT_GAMEPAD_DPAD_UP)
			{
				return true;
			}

			return false;
		}

		bool GetDownArrow(const WORD DPAD)
		{
			auto InputDevice = App::GetApp()->GetInputDevice();
			auto ContInput = InputDevice.GetControlerVec()[0];

			if (ContInput.wPressedButtons == XINPUT_GAMEPAD_DPAD_DOWN || DPAD == XINPUT_GAMEPAD_DPAD_DOWN)
			{
				return true;
			}

			return false;
		}

		bool GetRightArrow(const WORD DPAD)
		{
			auto InputDevice = App::GetApp()->GetInputDevice();
			auto ContInput = InputDevice.GetControlerVec()[0];

			if (ContInput.wPressedButtons == XINPUT_GAMEPAD_DPAD_RIGHT || DPAD == XINPUT_GAMEPAD_DPAD_RIGHT)
			{
				return true;
			}

			return false;
		}

		bool GetLeftArrow(const WORD DPAD)
		{
			auto InputDevice = App::GetApp()->GetInputDevice();
			auto ContInput = InputDevice.GetControlerVec()[0];

			if (ContInput.wPressedButtons == XINPUT_GAMEPAD_DPAD_LEFT || DPAD & XINPUT_GAMEPAD_DPAD_LEFT)
			{
				return true;
			}

			return false;
		}
	};

	class StageBase;

	//---------------------------------
	//�X�e�[�W�r���_�[�̍쐬
	//---------------------------------
	class StageBulider
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		StageBulider();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~StageBulider();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�N���G�[�^�[��o�^����
		@tparam	T	�o�^����^�i�Q�[���I�u�W�F�N�g�̔h���j
		@param[in]	ClsName	�^�̎��ʖ��i�ʏ�͌^���𕶎���ɂ������́j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void Register(const wstring& ClsName) {
			GetCreatorMap()[ClsName] = shared_ptr<GameObjectCreatorXML<T>>(new GameObjectCreatorXML<T>());
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�^�̎��ʖ����g���ăQ�[���I�u�W�F�N�g���\�z����
		@param[in]	ClsName	�^�̎��ʖ��i�ʏ�͌^���𕶎���ɂ������́j
		@param[in]	StagePtr	��������X�e�[�W
		@param[in]	pNode	XML�m�[�h
		@return	�쐬�����Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> CreateFromXML(const wstring& ClsName, const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XML����Q�[���X�e�[�W���\�z����
		@param[in]	StagePtr	��������X�e�[�W
		@param[in]	XMLFileName	XML�t�@�C����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void StageBuild(const shared_ptr<StageBase>& StagePtr, const wstring& XMLFileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XML����UI�Z�b�g���\�z����
		@param[in]	StagePtr	��������X�e�[�W
		@param[in]	XMLFileName	XML�t�@�C����
		@param[in]	DefaultDrawActive�@�������`�悷�邩�ǂ����B
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UISetBuild(const shared_ptr<StageBase>& StagePtr, const wstring& XMLFileName, const bool DefaultDrawActive);
	private:
		map<wstring, shared_ptr<GameObjectCreatorBaseXML>>& GetCreatorMap() const;

		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};
}
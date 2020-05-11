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
	};

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
		void StageBuild(const shared_ptr<Stage>& StagePtr, const wstring& XMLFileName);
	private:
		map<wstring, shared_ptr<GameObjectCreatorBaseXML>>& GetCreatorMap() const;

		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};
}
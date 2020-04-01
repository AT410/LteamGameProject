/*!
@breif �Q�[���S�̂̊Ǘ���`
@name�@�쐬�ҁF�����B��
*/
#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@berif �Q�[���}�l�[�W���[�N���X
	@info �Q�[���S�̂̊Ǘ��E�X�e�[�W�I�u�W�F�N�g�̐���
	*/
	class GameManager
	{
	private:
		struct GMDeleter
		{
			void operator()(GameManager *ptr) { delete ptr; }
		};

		static unique_ptr<GameManager, GMDeleter> m_ins;	///<-�}�l�[�W���[�̃C���X�^���X

		GameManager();
		~GameManager();
	public:
		static void CreateManager();
		
		static unique_ptr<GameManager, GMDeleter>& GetManager();

		static bool CheckManager();

		static void DeleteManager();
	};
}
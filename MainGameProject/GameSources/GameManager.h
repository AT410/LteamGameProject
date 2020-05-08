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

		pair<int, int > m_SelectStage;						///<-�X�e�[�W�I�������l

		GameManager();
		~GameManager();
	public:
		static void CreateManager();
		
		static unique_ptr<GameManager, GMDeleter>& GetManager();

		static bool CheckManager();

		static void DeleteManager();

		//�X�e�[�W�̐���
		//�X�e�[�W����Ă΂��
		void CreateStage(const shared_ptr<StageBase>&StagePtr,const wstring& FileName, const bool MenuActive);

		//�Q�b�^�[
		pair<int, int> GetStagePair()const {return m_SelectStage; }
		//�Z�b�^�[
		void SetStagePair(int AreaNum, int StageNum) { m_SelectStage = make_pair(AreaNum, StageNum); }
		void SetStagePair(const pair<int, int> StagePair) { m_SelectStage = StagePair; }
	private:
		//�R�s�[�֎~
		GameManager(const GameManager&) = delete;
		GameManager& operator=(const GameManager&) = delete;
		//���[�u�֎~
		GameManager(const GameManager&&) = delete;
		GameManager& operator=(const GameManager&&) = delete;

	};
}
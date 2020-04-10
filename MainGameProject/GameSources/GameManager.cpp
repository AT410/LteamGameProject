/*!
@breif �Q�[���S�̂̊Ǘ�����
@name�@�쐬��:�����B��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//static�ϐ��̎���
	unique_ptr<GameManager,GameManager::GMDeleter> GameManager::m_ins;

	GameManager::GameManager()
		:m_SelectStage(0,0)
	{

	}

	GameManager::~GameManager()
	{

	}

	//static�֐�
	void GameManager::CreateManager()
	{
		try
		{
			if (m_ins.get() == 0)
			{
				m_ins.reset(new GameManager());
			}
		}
		catch (...)
		{
			throw;
		}
	}

	//Get
	unique_ptr<GameManager, GameManager::GMDeleter>& GameManager::GetManager()
	{
		try
		{
			if (m_ins.get() == 0)
			{
				throw(BaseException(
					L"GameManager����������Ă��܂���",
					L"if(m_ins.get()==0)",
					L"GameManager::GetManager"
				));
			}
			else
				return m_ins;
		}
		catch (...)
		{
			throw;
		}
	}


	bool GameManager::CheckManager()
	{
		if (m_ins == 0)
			return false;
		else
			return true;
	}

	void GameManager::DeleteManager()
	{
		if (m_ins.get() == 0)
		{
			//�p������
			m_ins.reset();
		}
	}

	//�X�e�[�W����
	void GameManager::CreateStage(const shared_ptr<StageBase>&StagePtr,const wstring& FileName)
	{
		StageBulider Builder;
		Builder.Register<StageTest>(L"Test");

		Builder.StageBuild(StagePtr, FileName);
	}
}
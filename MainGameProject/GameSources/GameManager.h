/*!
@breif �Q�[���S�̂̊Ǘ���`
@name�@�쐬�ҁF�����B��
*/
#pragma once
#include "stdafx.h"

namespace basecross
{
	//----------------------------------------------------------------------------
	//�f�[�^�N���X�ǉ�
	//----------------------------------------------------------------------------
	class SaveData
	{
	public:
		SaveData(const wstring& FilePath);
		~SaveData();

		void Save();
		void Load(const wstring& FileName);

		void Clear();

		bool IsAreaClear(int AreaNumber);

		bool IsStageClear(int StageNumber);

		void Update();

		bool DataToStr(const wstring& FileName, wstring& result);
	private:
		struct Impl;
		unique_ptr<Impl> m_pImpl;
	};

	//----------------------------------------------------------------------------
	//�Q�[���}�l�[�W���[
	//----------------------------------------------------------------------------
	class GameManager
	{
	private:
		struct GMDeleter
		{
			void operator()(GameManager *ptr) { delete ptr; }
		};

		static unique_ptr<GameManager, GMDeleter> m_ins;	///<-�}�l�[�W���[�̃C���X�^���X

		pair<int, int > m_SelectStage;						///<-�X�e�[�W�I�������l

		pair<int, int> m_MaxStageCount;						///<-�ő�X�e�[�W�o�^��

		wstring m_MapFile;									///<-�}�b�v�f�[�^�t�@�C��

		wstring m_ResFile;									///<-���\�[�X���X�g�t�@�C��

		wstring m_UISetFile;								///<-UI�}�b�v�f�[�^�t�@�C��

		shared_ptr<SaveData> m_Data;						///<-�Z�[�u�f�[�^�N���X

		std::mutex mutex;

		bool m_ResorceLoad;									///<-���\�[�X�Ǎ��������ǂ���

		bool m_UpdateActive;								///<-�X�V�\���ǂ���

		bool m_StageReloadActive;							///<-�X�e�[�W���ēǂݍ��݂��邩�ǂ���

		bool m_StartCameraEnd;								///<-�J�n�C�x���g�J�����I��

		void ResorceLoadFunc();								///<-���\�[�X�̓Ǎ����s��

		GameManager();
		~GameManager();
	public:
		static void CreateManager();
		
		static unique_ptr<GameManager, GMDeleter>& GetManager();

		static bool CheckManager();

		static void DeleteManager();

		void LoadResources();

		//�X�e�[�W�̐���
		//�X�e�[�W����Ă΂��
		void CreateGameStage(const shared_ptr<StageBase>&StagePtr);

		void CreateUISet(const shared_ptr<StageBase>&StagePtr,const bool DefaultDrawActive =true);

		void UpdateManager();

		//�Q�b�^�[
		pair<int, int> GetStagePair()const {return m_SelectStage; }
		pair<int, int> GetMaxStagePair()const { return m_MaxStageCount; }
		bool GetLoadFlag()const { return m_ResorceLoad; }
		bool GetUpdateActive()const { return m_UpdateActive; }
		bool GetStageReloadActive()const { return m_StageReloadActive; }
		bool GetStartCameraActive()const { return m_StartCameraEnd; }
		shared_ptr<SaveData> GetSaveData()const { return m_Data; }

		//�Z�b�^�[
		void SetStagePair(int AreaNum, int StageNum) { m_SelectStage = make_pair(AreaNum, StageNum); }
		void SetStagePair(const pair<int, int> StagePair) { m_SelectStage = StagePair; }
		void SetAreaNumber(int AreaNum) { m_SelectStage.first = AreaNum; }
		void SetStageNumber(int StageNum) { m_SelectStage.second = StageNum; }

		void SetMaxAreaCount(int AreaCount) { m_MaxStageCount.first = AreaCount; }
		void SetMaxStageCount(int StageCount) { m_MaxStageCount.second = StageCount; }
		void SetUpdateActive(const bool Active) { m_UpdateActive = Active; }
		void SetStageReloadActive(const bool Active) { m_StageReloadActive = Active; }
		void SetStartCameraActive(const bool Active) { m_StartCameraEnd = Active; }
	private:
		//�R�s�[�֎~
		GameManager(const GameManager&) = delete;
		GameManager& operator=(const GameManager&) = delete;
		//���[�u�֎~
		GameManager(const GameManager&&) = delete;
		GameManager& operator=(const GameManager&&) = delete;

	};
}
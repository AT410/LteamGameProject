/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	enum class StageType
	{
		TitleStage,
		LoadStage,
		DataSelectStage,
		AreaSelectStage,
		StageSelectStage,
		GameStage,
		EndingStage
	};

	//--------------------------------------------------------------------------------------
	//	�X�e�[�W�x�[�X�N���X
	//--------------------------------------------------------------------------------------
	class StageBase : public Stage 
	{
	public:
		//�\�z�Ɣj��
		StageBase(const StageType Type) 
			:Stage(),m_StageType(Type) {}
		virtual ~StageBase() {}
		//������
		virtual void OnCreate()override = 0;
		virtual void OnUpdate()override {};
		virtual void OnDestroy()override;
		virtual void OnDraw()override {};

		StageType GetStageType()const { return m_StageType; }

		wstring GetStageTypeStr()const;

		shared_ptr<SingleView> GetOpeningView() { return m_OpeningView; }
		shared_ptr<SingleView> GetMainView() { return m_MainView; }
	protected:
		//�r���[�̍쐬
		virtual void CreateViewLight();

		void SetBGM(const wstring& BGMKey);

		shared_ptr<SoundItem> m_BGMPtr = nullptr;

		StageType m_StageType;

		//�J����
		shared_ptr<SingleView> m_MainView;
		shared_ptr<SingleView> m_OpeningView;
	};

	//--------------------------------------------------------------------------------------
	//	�^�C�g���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class TitleStage :public StageBase
	{
	public:
		//�\�z�Ɣj��
		TitleStage():StageBase(StageType::TitleStage){}
		virtual ~TitleStage(){}
		//������
		void OnCreate()override;
		void OnUpdate()override;
	private:


	};
	//--------------------------------------------------------------------------------------
	//	�Z�[�u�f�[�^�Z���N�g�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class DataSelectStage :public StageBase
	{
	public:
		DataSelectStage() :StageBase(StageType::DataSelectStage) {}
		virtual ~DataSelectStage(){}
		//������
		void OnCreate()override;
		void OnUpdate()override {};

	};
	//--------------------------------------------------------------------------------------
	//	�G���A�Z���N�g�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class AreaSelectStage :public StageBase
	{
	public:
		AreaSelectStage() :StageBase(StageType::AreaSelectStage) {}
		virtual ~AreaSelectStage() {}
		//������
		void OnCreate()override;
		void OnUpdate()override {};
	};

	//--------------------------------------------------------------------------------------
	// �X�e�[�W�Z���N�g�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class StageSelectStage :public StageBase
	{
	public:
		StageSelectStage() :StageBase(StageType::StageSelectStage) {}
		virtual ~StageSelectStage() {}
		//������
		void OnCreate()override;
		void OnUpdate()override {};
	};

	//--------------------------------------------------------------------------------------
	//���[�h�ҋ@�X�e�[�W(���[�h�I���������őJ�ځj
	//--------------------------------------------------------------------------------------
	class LoadStage :public StageBase
	{
	public:
		LoadStage() :StageBase(StageType::LoadStage){}
		virtual ~LoadStage(){}

		//������
		void OnCreate()override;
		//�X�V
		void OnUpdate()override;
	};
	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage :public StageBase
	{
		shared_ptr<EfkPlay> m_EfkPlay[50];
		int m_EfkCount = 0;

	public:
		GameStage() :StageBase(StageType::GameStage) {}
		virtual ~GameStage() {}

		//�X�e�[�W�̔z�u
		void GenerateStage();
		//������
		void OnCreate()override;
		void OnUpdate()override;

		void OnDraw()override;

		void Effectplay(wstring Key, Vec3 hitpoint) {
			//�G�t�F�N�g�̃v���C********************************
			//auto TransformPtr = &tr;
			//auto ShEfkInterface = GetTypeStage<GameStage>()->GetEfkInterface();

			m_EfkPlay[m_EfkCount] = ObjectFactory::Create<EfkPlay>(Key, hitpoint);
			if (m_EfkCount == 19) {
				m_EfkCount = 0;
			}
			else {
				m_EfkCount++;
			}
		}


		void ToMyCamera();
		//�Đ��G�t�F�N�g��ǉ�����
		//void AddEffect();

	private:
	};

	//--------------------------------------------------------------------------------------
	//	�G���f�B���O�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class EndingStage :public StageBase
	{
	public:
		EndingStage() :StageBase(StageType::EndingStage) {}
		virtual ~EndingStage() {}
		//������
		void OnCreate()override;
		void OnUpdate()override {};
	};

}
//end basecross


/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�X�e�[�W�x�[�X�N���X
	//--------------------------------------------------------------------------------------
	class StageBase : public Stage 
	{
	public:
		//�\�z�Ɣj��
		StageBase() :Stage() {}
		virtual ~StageBase() {}
		//������
		virtual void OnCreate()override = 0;
		virtual void OnUpdate()override = 0;
		virtual void OnDestroy()override;
	protected:
		//�r���[�̍쐬
		virtual void CreateViewLight();

		void SetBGM(const wstring& BGMKey);

		shared_ptr<SoundItem> m_BGMPtr;

	};

	//--------------------------------------------------------------------------------------
	//	�^�C�g���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class TitleStage :public StageBase
	{
	public:
		//�\�z�Ɣj��
		TitleStage():StageBase(){}
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
		DataSelectStage():StageBase(){}
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
		AreaSelectStage() :StageBase() {}
		virtual ~AreaSelectStage() {}
		//������
		void OnCreate()override;
		void OnUpdate()override {};
	};

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage :public StageBase
	{
	public:
		GameStage() :StageBase() {}
		virtual ~GameStage() {}
		//������
		void OnCreate()override;
		void OnUpdate()override {};
	};

	//--------------------------------------------------------------------------------------
	//	�G���f�B���O�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class EndingStage :public StageBase
	{
	public:
		EndingStage() :StageBase() {}
		virtual ~EndingStage() {}
		//������
		void OnCreate()override;
		void OnUpdate()override {};
	};

}
//end basecross


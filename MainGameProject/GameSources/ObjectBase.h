/*!
@breif	�I�u�W�F�N�g�̊��N���X
@name	�쐬�ҁF�����B��
*/
#pragma once
#include "stdafx.h"

namespace basecross
{
	//---------------------------------------------------
	//�z�u�I�u�W�F�N�g�̊��N���X
	//---------------------------------------------------
	class ObjectBase :public GameObject
	{
	public:
		ObjectBase(const shared_ptr<Stage>&StagePtr)
			:GameObject(StagePtr) {}
		virtual ~ObjectBase() {}

		//������
		virtual void OnCreate()override = 0;
		//�X�V����
		virtual void OnUpdate()override = 0;

	};

	//---------------------------------------------------
	//���͑���\�N���X�̊��N���X
	//---------------------------------------------------
	class PawnBase :public ObjectBase
	{
	public:
		PawnBase(const shared_ptr<Stage>&StagePtr)
			:ObjectBase(StagePtr) {}
		virtual ~PawnBase() {}

		virtual void OnPushA() = 0;
		virtual void OnPushB() = 0;
		virtual void OnPushX() = 0;
		virtual void OnPushY() = 0;
		
		//������
		virtual void OnCreate()override =0;
		//�X�V����
		virtual void OnUpdate()override =0;
	protected:
		
	};
}
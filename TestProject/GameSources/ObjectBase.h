/*!
@breif	�I�u�W�F�N�g�̊��N���X
@name	�쐬�ҁF�����B��
*/
#pragma once
#include "stdafx.h"
#include "ProjectUtility.h"

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
	template <typename T>
	class PawnBase
	{
	public:
		PawnBase(){}

		virtual ~PawnBase() {}

		virtual void OnPushA() {};
		virtual void OnPushB() {};
		virtual void OnPushX() {};
		virtual void OnPushY() {};
		virtual void OnPushLB() {};
		virtual void OnRemoveLB() {};
		virtual void OnPushRB() {};
	protected:
		InputHandler2<T> m_Handler;
	};
}
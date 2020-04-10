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

		ObjectBase(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		virtual ~ObjectBase() {}

		//������
		virtual void OnCreate()override = 0;
		//�X�V����
		virtual void OnUpdate()override {};

	protected:
		Vec3 m_pos;
		Vec4 m_rot;
		Vec3 m_scal;
		wstring m_texKey;
		wstring m_meshKey;
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
	protected:
		InputHandler<T> m_handler;
	};
}
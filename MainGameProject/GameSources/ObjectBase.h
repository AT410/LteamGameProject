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

		ObjectBase(const shared_ptr<Stage>&StagePtr,const Vec3 Position,const Vec3 Rotation,const Vec3 Scale,
					const wstring TexKey,const wstring MeshKey)
			:GameObject(StagePtr),m_pos(Position),m_rot(Rotation,1.0f),m_scal(Scale),m_texKey(TexKey),m_meshKey(MeshKey) {}

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
		vector<wstring> m_tag;
		bool m_SharedActive = false;
		wstring m_SharedName;
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
		virtual void OnPushR3() {};
		virtual void OnPushL3() {};
		virtual void OnPushLB() {};
		virtual void OnRemoveLB() {};
		virtual void OnPushRB() {};
	protected:
		InputHandler<T> m_handler;
	};
}
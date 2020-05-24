/*!
@breif	Gimmick�֘A
@name	�쐬�ҁF�����B��
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	///<breif>�X�C�b�`�I�u�W�F�N�g<breif/>
	///<name>�쐬�ҁF�����B��<name/>
	class SwitchObj : public ObjectBase
	{
	public:
		SwitchObj(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey,const bool IsKeep = false)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey), m_IsKeep(IsKeep), m_Active(false)
		{
		}

		SwitchObj(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		void OnCreate() override;
		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		void OnCollisionExit(shared_ptr<GameObject>& Other)	override;

		//�Q�b�^�[
		bool GetSwitchActiveI()const { return m_Active; }
	private:
		bool m_Active;//ON�EOFF����
		//����������K�v�����邩�B
		bool m_IsKeep;

		//�C�x���g�ݒ�
		wstring m_RecipientKey;//<-��M��ݒ�L�[
		wstring m_EventMsg; //<-�C�x���g���b�Z�[�W

		//�G�t�F�N�g
		shared_ptr<EfkPlay> m_ActiveEfk;
	};

	//----------------------------------------------------------------------------
	//���ΐ�
	//----------------------------------------------------------------------------
	class FireLine : public ObjectBase
	{
	public:
		//-- �\�z --
		FireLine(const shared_ptr<Stage>& StagePtr)
			:ObjectBase(StagePtr)
		{
		}

		FireLine(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);


		//-- �j�� --
		virtual ~FireLine() {}

		//-- ������ --
		void OnCreate()override;

		//-- �X�V���� --
		void OnUpdate()override;

		void OnEvent(const shared_ptr<Event>&event);

	private:
		float m_Time = 0;
		bool m_Active = false;

		//�C�x���g�ݒ�
		wstring m_RecipientKey;//<-��M��ݒ�L�[
		wstring m_EventMsg; //<-�C�x���g���b�Z�[�W
	};

	///<breif>�M��`����_<breif/>
	///<name> �쐬�ҁF�ɓ��ˌ�<name/>
	class HeatStick : public ObjectBase
	{
	private:
		AABB m_HeatArea;

		shared_ptr<Player> m_player;
		shared_ptr<ObjectBase> m_himo;

		//�f�B���C�J�E���g
		float m_count;
	public:
		HeatStick(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey), m_count(0)
		{
		}
		HeatStick(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}

		void OnCreate() override;
		void OnUpdate() override;
	};

	//������
	class Omori : public ObjectBase
	{
	public:
		Omori(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		{
		}

		Omori(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}

		void OnCreate() override;

		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};

	//������
	class MoveFloor : public ObjectBase
	{
	private:
		float m_Time;
		bool m_Active; //flg
	public:
		MoveFloor(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		{
		}

		MoveFloor(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}


		void OnCreate() override;
		void OnUpdate() override;

		void SetActive(bool active)
		{
			m_Active = active;
		}
	};
	//-----------------------------------------------------------------------------
	//���̃I�u�W�F�N�g
	//-----------------------------------------------------------------------------
	class Door :public ObjectBase
	{
	public:
		Door(const shared_ptr<Stage>&StagePtr)
			:ObjectBase(StagePtr){}

		// -- Xml�t�@�C���Ǎ��p --
		Door(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		virtual ~Door(){}

		// -- ������ --
		void OnCreate()override;

		// -- �X�V���� --
		void OnUpdate()override;

		void OnEvent(const shared_ptr<Event>& event)override;

		// -- �Z�b�^�[ --
		void SetOpenActive(bool Active) { m_OpenActive = Active; }

	private:
		// -- �J�又�� --
		void OpenProcess();
		// -- �又�� --
		void CloseProcess();

		bool MoveBehavior(Vec3 Start,Vec3 End, const float TotalTime);

		float m_TotalTime;
		bool m_OpenActive;
		bool m_MoveEnd;
	};

	//----------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------
	class Fountain :public ObjectBase
	{
	private:
		bool m_Active; //flg
		float m_Time = 0;

		AABB m_FountainArea; //��ɗL��
		AABB m_FountainArea2; //����Active�̎������ɗL��

		shared_ptr<Player> m_TestPlayer;

		shared_ptr<EfkPlay> m_WaterEfk;

	public:
		Fountain(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		{
		}

		Fountain(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}

		void OnCreate() override;
		void OnUpdate() override;

		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

		void SetActive(bool active)
		{
			m_Active = active;
		}
	};


	//-----------------------------------------------------------------------------
	//��N���X
	//-----------------------------------------------------------------------------
	class Waterfall :public ObjectBase
	{
	public:
		Waterfall(const shared_ptr<Stage>&StagePtr,const Vec3& Start,const Vec3& End,const float Width,const float Speed)
			:ObjectBase(StagePtr),m_StartPoint(Start),m_EndPoint(End),m_Width(Width),m_FallSpeed(Speed){}

		Waterfall(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		{
		}

		void OnCreate()override;

		void OnUpdate()override;

	private:
		Vec3 m_StartPoint;
		Vec3 m_EndPoint;

		float m_Width;
		float m_FallSpeed;

		float m_TotalTime = 0;

		shared_ptr<EfkPlay> m_EfkPlay;
	};
}
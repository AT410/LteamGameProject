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

		SwitchObj(const shared_ptr<Stage>&StagePtr,IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr,pNode){}

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


	//����
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
}
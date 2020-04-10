/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Himo;
	//�M�_
	class HeatStick : public GameObject
	{
	private:
		Vec3 m_Pos;
		Vec3 m_Rotation;
		Vec3 m_Scale;
		AABB m_HeatArea; //AABB

		shared_ptr<Himo> m_himo;

		//�f�B���C�J�E���g
		int count = 1;
	public:
		HeatStick(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale)
			:GameObject(StagePtr), m_Pos(Position), m_Rotation(Rotation), m_Scale(Scale)
		{
		}
		void OnCreate() override;
		void OnUpdate() override;
	};

	//�i���j�v���C���[
	class TestPlayer : public GameObject
	{
	private:
		Vec3 m_Pos;
		Vec3 m_Rotation;
		Vec3 m_Scale;
	public:
		TestPlayer(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale)
			:GameObject(StagePtr), m_Pos(Position), m_Rotation(Rotation), m_Scale(Scale)
		{
		}

		void OnCreate() override;
		void OnUpdate() override;
	};
	//�Ђ�
	class Himo : public GameObject
	{
	private:
		Vec3 m_Pos;
		Vec3 m_Rotation;
		Vec3 m_Scale;
	public:
		Himo(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale)
			:GameObject(StagePtr), m_Pos(Position), m_Rotation(Rotation), m_Scale(Scale)
		{
		}

		void OnCreate() override;
	};

	//������
	class Omori : public GameObject
	{
	private:
		Vec3 m_Pos;
		Vec3 m_Rotation;
		Vec3 m_Scale;
	public:
		Omori(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale)
			:GameObject(StagePtr), m_Pos(Position), m_Rotation(Rotation), m_Scale(Scale)
		{
		}

		void OnCreate() override;
	};
	//�{�^��
	class Button : public GameObject
	{
	private:
		Vec3 m_Pos;
		Vec3 m_Rotation;
		Vec3 m_Scale;
	public:
		Button(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale)
			:GameObject(StagePtr), m_Pos(Position), m_Rotation(Rotation), m_Scale(Scale)
		{
		}
		void OnCreate() override;
		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		void OnCollisionExcute(shared_ptr<GameObject>& Other);
	};
	//��
	class Floor : public GameObject
	{
	private:
		Vec3 m_Pos;
		Vec3 m_Rotation;
		Vec3 m_Scale;
	public:
		Floor(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale)
			:GameObject(StagePtr), m_Pos(Position), m_Rotation(Rotation), m_Scale(Scale)
		{
		}

		void OnCreate() override;
	};
	//������
	class MoveFloor : public GameObject
	{
	private:
		Vec3 m_Pos;
		Vec3 m_Rotation;
		Vec3 m_Scale;
		float m_Time;
		bool m_Active; //flg
	public:
		MoveFloor(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale)
			:GameObject(StagePtr), m_Pos(Position), m_Rotation(Rotation), m_Scale(Scale), m_Time(0.0f), m_Active(false)
		{
		}

		void OnCreate() override;
		void OnUpdate() override;

		void SetActive(bool active)
		{
			m_Active = active;
		}
	};


	//����
	class Fountain :public GameObject
	{
	private:
		Vec3 m_Pos;
		Vec3 m_Scale;
		bool m_Active; //flg
		float m_Time = 0;

		AABB m_FountainArea; //��ɗL��
		AABB m_FountainArea2; //����Active�̎������ɗL��

		shared_ptr<TestPlayer> m_TestPlayer;

	public:
		Fountain(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Scale)
			:GameObject(StagePtr), m_Pos(Position), m_Scale(Scale)
		{
		}

		void OnCreate() override;
		void OnUpdate() override;

		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

		void SetActive(bool active)
		{
			m_Active = active;
		}
	};
}
//end basecross

/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Himo;
	//熱棒
	class HeatStick : public GameObject
	{
	private:
		Vec3 m_Pos;
		Vec3 m_Rotation;
		Vec3 m_Scale;
		AABB m_HeatArea;

		shared_ptr<Himo> m_himo;

		//ディレイカウント
		int count = 1;
	public:
		HeatStick(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale)
			:GameObject(StagePtr), m_Pos(Position), m_Rotation(Rotation), m_Scale(Scale)
		{
		}
		void OnCreate() override;
		void OnUpdate() override;
	};

	//（仮）プレイヤー
	class Player : public GameObject
	{
	private:
		Vec3 m_Pos;
		Vec3 m_Rotation;
		Vec3 m_Scale;
	public:
		Player(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale)
			:GameObject(StagePtr), m_Pos(Position), m_Rotation(Rotation), m_Scale(Scale)
		{
		}

		void OnCreate() override;
		void OnUpdate() override;
	};
	//ひも
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

	//おもり
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
	//ボタン
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
	//床
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
	//動く床
	class MoveFloor : public GameObject
	{
	private:
		Vec3 m_Pos;
		Vec3 m_Rotation;
		Vec3 m_Scale;
		float m_Time;
		bool m_Active;
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

}
//end basecross

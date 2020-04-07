/*!
@file Character.h
@brief ÉMÉ~ÉbÉNä÷åWÇ»Ç«
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class Gimmick : public GameObject {
		shared_ptr<StateMachine<Gimmick>> m_StateMachine;
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		int MoveSpeed=1;
		bool Moveflg = false;
	public:
		Gimmick(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~Gimmick();
		//èâä˙âª
		void Move();
		virtual void OnCreate() override;
		virtual void OnUpdate()override;
		shared_ptr<StateMachine<Gimmick>>&GetStateMachine()
		{
			return m_StateMachine;
		}
		void setmoveflg(bool active)
		{
			Moveflg = active;
		}
	};

	class UpDown :public ObjState<Gimmick>
	{
		UpDown() {}
	public:
		DECLARE_SINGLETON_INSTANCE(UpDown);
		virtual void Enter(const shared_ptr<Gimmick>& obj)override;
		virtual void Execute(const shared_ptr<Gimmick>& obj)override;
		virtual void Exit(const shared_ptr<Gimmick>& obj)override;
	};

	class Nomal :public ObjState<Gimmick>
	{
		Nomal() {}
	public:
		DECLARE_SINGLETON_INSTANCE(Nomal);
		virtual void Enter(const shared_ptr<Gimmick>& obj)override;
		virtual void Execute(const shared_ptr<Gimmick>& obj)override;
		virtual void Exit(const shared_ptr<Gimmick>& obj)override;
	};

	
}

/*!
@file Character.cpp
@brief ギミック関係など
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Gimmick::Gimmick(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}

	void Gimmick::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
		//タグをつける
		AddTag(L"Gimmick");
		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//ptrDraw->SetTextureResource(L"SKY_TX");
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetOwnShadowActive(true);

		PsBoxParam param(ptrTransform->GetWorldMatrix(), 0.0f, true,
			PsMotionType::MotionTypeFixed);
		auto PsPtr = AddComponent<RigidbodyBox>(param);		
		PsPtr->SetAutoTransform(false);
		AddComponent<CollisionObb>();

		m_StateMachine.reset(new StateMachine<Gimmick>(GetThis<Gimmick>()));
		//m_StateMachine->ChangeState(Nomal::Instance());
	}
	Gimmick::~Gimmick() {}

	void Gimmick::Move()
	{
		auto deltatime = App::GetApp()->GetElapsedTime();
		auto Pos = GetComponent<Transform>()->GetPosition();


		if (Pos.y > 3)
		{
			MoveSpeed = -1;
		}
		if (Pos.y<1)
		{
			MoveSpeed = +1;
		}
		Pos.y += MoveSpeed * deltatime;
		GetComponent<Transform>()->SetPosition(Pos);
	}
	void Gimmick::OnUpdate()
	{
		//m_StateMachine->Update();
		if (Moveflg)
		{
			Move();
		}
	};

	
	IMPLEMENT_SINGLETON_INSTANCE(UpDown)
	void UpDown::Enter(const shared_ptr<Gimmick>& obj)
	{

	}
	void UpDown::Execute(const shared_ptr<Gimmick>& obj)
	{

	}
	void UpDown::Exit(const shared_ptr<Gimmick>& obj)
	{

	}

	IMPLEMENT_SINGLETON_INSTANCE(Nomal)
	void Nomal::Enter(const shared_ptr<Gimmick>& obj)
	{

	}
	void Nomal::Execute(const shared_ptr<Gimmick>& obj)
	{

	}
	void Nomal::Exit(const shared_ptr<Gimmick>& obj)
	{

	}
}

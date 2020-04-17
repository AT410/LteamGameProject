/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	FixBox::FixBox(const shared_ptr<Stage>& Stageptr, Vec3 Scale, Vec3 Rotation, Vec3 Position):
		GameObject(Stageptr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{}

	void FixBox::OnCreate(){
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);

		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		AddTag(L"Floor");
		
	}

	ReturnCube::ReturnCube(const shared_ptr<Stage>& Stageptr, Vec3 Scale, Vec3 Rotation, Vec3 Position):
		GameObject(Stageptr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{}

	void ReturnCube::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);

		auto ptrColl = AddComponent<CollisionSphere>();
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");

		AddTag(L"Goal");
	}

	PushPullObj::PushPullObj(const shared_ptr<Stage>& Stageptr, Vec3 Scale, Vec3 Rotation, Vec3 Position):
		GameObject(Stageptr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{}

	void PushPullObj::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);

		auto ptrGrav = AddComponent<Gravity>();
		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

		auto ptrColl = AddComponent<CollisionObb>();
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");

		AddTag(L"PushPullObj");
	}
}
//end basecross

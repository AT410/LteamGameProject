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

	WaterLV::WaterLV(const shared_ptr<Stage>& Stageptr) :
		ObjectBase(Stageptr),
		m_WaterLVMode(false),
		m_AABBX(1.0f),
		m_AABBY(1.0f),
		m_AABBZ(1.0f)
	{}

	void WaterLV::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(-5.0f, 1.0f, 0.0f);
		ptrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTransform->SetScale(1.0f, 1.0f, 1.0f);
		auto ptrScale = ptrTransform->GetScale();
		m_WaterOldPos = ptrTransform->GetPosition();
		m_WaterCurrentPos = ptrTransform->GetPosition();
		m_AABBX = ptrScale.x;
		m_AABBY = ptrScale.y;
		m_AABBZ = ptrScale.z;
		m_PlayerAABBX = ptrScale.x / 2;
		m_PlayerAABBY = ptrScale.y / 2;
		m_PlayerAABBZ = ptrScale.z / 2;
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
	}
	//＠ウォーターレベルモードセット関数
	//＠松崎　洸樹
	//bool型のウォーターレベルモードのセット関数
	void WaterLV::SetWaterLVMode(const bool LVMode) {
		m_WaterLVMode = LVMode;
	}
	//＠ウォーターレベル変更関数
	//＠松崎　洸樹
	//＠ウォーターレベルの水位状態変化を行う関数
	void WaterLV::WaterLVChange() {
		auto Elapsedtime = App::GetApp()->GetElapsedTime();
		auto ptrTransform = GetComponent<Transform>();
		auto ptrObjPos = ptrTransform->GetPosition();
		if (m_WaterLVMode) {
			if (m_WaterCurrentPos.y > m_WaterOldPos.y - 2.0f) {
				m_WaterCurrentPos.y += -0.5f * Elapsedtime;
			}
			else if (m_WaterCurrentPos.y < m_WaterOldPos.y - 2.0f) {
				m_WaterTime += Elapsedtime;
			}
			if (m_WaterTime > 6.0f) {
				m_WaterCurrentPos.y += 1.0f * Elapsedtime;
			}
			if (m_WaterCurrentPos.y > m_WaterOldPos.y) {
				m_WaterTime = 0.0f;
				m_WaterCurrentPos.y = m_WaterOldPos.y;
				m_WaterLVMode = false;
			}
		}
		ptrTransform->SetPosition(m_WaterCurrentPos);
	}
	//＠ウォーターレベル判定関数
	//＠松崎　洸樹
	//＠ウォーターレベルとプレイヤーのボリューム境界における接触判定を行う関数
	void WaterLV::WaterLVJudgement() {
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = ptrPlayer->GetComponent<Transform>()->GetPosition();
		auto ptrTransform = GetComponent<Transform>();
		auto ptrPos = ptrTransform->GetPosition();
		m_WaterLVAABB = AABB(ptrPos, m_AABBX, m_AABBY, m_AABBZ);
		auto PlayerAABB = AABB(Vec3(PlayerPos.x - m_PlayerAABBX, PlayerPos.y, PlayerPos.z - m_PlayerAABBZ),
			Vec3(PlayerPos.x + m_PlayerAABBX, PlayerPos.y + m_PlayerAABBY, PlayerPos.z + m_PlayerAABBZ));
		if (HitTest::AABB_AABB(m_WaterLVAABB, PlayerAABB)) {
			ptrPlayer->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 0.0f);
		}
	}

	void WaterLV::OnUpdate() {
		WaterLVChange();
		WaterLVJudgement();
	}

}
//end basecross

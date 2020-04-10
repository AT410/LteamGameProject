
#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	WaterWheel::WaterWheel(const shared_ptr<Stage>& StagePtr,
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
	WaterWheel::~WaterWheel() {}
	void WaterWheel::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
		//タグをつける
		AddTag(L"WaterWheel");
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
	}
	void WaterWheel::OnUpdate()
	{
	  //      Time += App::GetApp()->GetElapsedTime();
			//auto ptrTrans = GetComponent<Transform>();
			//m_Rotation = ptrTrans->GetRotation();
			//Easing<Vec3> easing;
			//auto Force = easing.Linear(m_Rotation, Vec3(0, 0, 0), Time, 5.0f);
			//const float LerpFact = 1.0f;
			//bsm::Quat FQuat;
			//FQuat.rotationRollPitchYawFromVector(Force);
			//FQuat.normalize();
			//ptrTrans->SetQuaternion(FQuat);

		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= 4.0f)
		{
			m_TotalTime = 0.0f;
			m_Swap = 1 - m_Swap;
		}
		auto PtrTrans = GetComponent<Transform>();
		auto StartPos = PtrTrans->GetPosition();
		StartPos.x = m_Position.x;
		auto EndPos = StartPos;
		EndPos.x += 4.0f;
		Vec3 StartRot(0, 0, 0);
		Vec3 EndRot(0, 0, 180);
		Vec3 TgtPos, TgtRot;
		Easing<Vec3> easing;
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (KeyState.m_bPushKeyTbl['R'])
		{
			
			TgtRot = easing.EaseInOut(EasingType::Exponential, EndRot, StartRot, m_TotalTime, 4.0f);

		}
		if (KeyState.m_bPushKeyTbl['L'])
		{
			
			TgtRot = easing.EaseInOut(EasingType::Exponential, StartRot, EndRot, m_TotalTime, 4.0f);

		}

		bsm::Quat FQuat;
		FQuat.rotationRollPitchYawFromVector(TgtRot);
		FQuat.normalize();
		PtrTrans->SetQuaternion(FQuat);
		PtrTrans->SetQuaternion(FQuat);


	}
}
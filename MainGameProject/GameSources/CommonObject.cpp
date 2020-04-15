/*!
@breif 汎用オブジェクト実装
@name　作成者：阿部達哉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	
	void FixedObj::OnCreate()
	{
		//描画設定
		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetMeshResource(m_meshKey);
		DrawComp->SetTextureResource(m_texKey);

		//配置設定
		auto TransComp = GetComponent<Transform>();
		TransComp->SetPosition(m_pos);
		TransComp->SetQuaternion(Quat(m_rot));
		TransComp->SetScale(m_scal);

		//物理判定
		auto CollComp = AddComponent<CollisionObb>();
		CollComp->SetFixed(true);

		for (auto tag : m_tag)
		{
			if (tag == L"")
				continue;
			AddTag(tag);
		}

		if (m_SharedActive)
		{
			GetStage()->SetSharedGameObject(m_SharedName, GetThis<FixedObj>());
		}

	}

	///<breif>汎用移動オブジェクト<breif/>
	MoveObj::MoveObj(const shared_ptr<Stage>&StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
		const wstring TexKey, const wstring MeshKey, const Vec3 Start, const Vec3 End, const float Speed, const float TotalTime)
		:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		,m_Start(Start),m_End(End),m_speed(Speed),m_Movetime(TotalTime), m_CurrntTime(0.0f),m_Type(MovingType::Rotation)
	{
	
	}

	MoveObj::MoveObj(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr,pNode)
	{
		
	}

	MoveObj::~MoveObj()
	{

	}

	void MoveObj::OnCreate()
	{
		//描画設定
		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetMeshResource(m_meshKey);
		DrawComp->SetTextureResource(m_texKey);

		//配置設定
		auto TransComp = GetComponent<Transform>();
		TransComp->SetPosition(m_pos);
		TransComp->SetScale(m_scal);
		TransComp->SetQuaternion(Quat(m_rot));

		//物理判定
		auto CollComp = AddComponent<CollisionObb>();
		CollComp->SetFixed(true);

		for (auto tag : m_tag)
		{
			AddTag(tag);
		}

		if (m_SharedActive)
		{
			GetStage()->SetSharedGameObject(m_SharedName, GetThis<MoveObj>());
		}
	}

	void MoveObj::OnUpdate()
	{
		//移動処理
		switch (m_Type)
		{
		case basecross::MovingType::Position:
			PosMove();
			break;
		case basecross::MovingType::Rotation:
			RotMove();
			break;
		case basecross::MovingType::Scaling:
			break;
		default:
			break;
		}
	}

	void MoveObj::PosMove()
	{
		if (LerpMove(m_Start, m_End))
		{
			Swap<Vec3>(m_Start,m_End);
		}
		else
		{

		}
	}

	bool MoveObj::LerpMove( Vec3 Start, Vec3 End)
	{
		m_CurrntTime += m_speed * App::GetApp()->GetElapsedTime();
		if (m_Movetime < m_CurrntTime)
		{
			m_CurrntTime = 0.0f;
			return true;
		}

		Easing<Vec3> easing;
		auto Force = easing.EaseInOut(EasingType::Cubic, Start, End, m_CurrntTime, m_Movetime);
		
		auto TransComp = GetComponent<Transform>();
		TransComp->SetPosition(Force);
		return false;
	}

	void MoveObj::RotMove()
	{
		auto ptrTrans = GetComponent<Transform>();
		auto Rotation = ptrTrans->GetRotation();
		Vec3 Force = Vec3(0, 0, 10)*m_speed*App::GetApp()->GetElapsedTime();
		bsm::Quat FQuat;
		FQuat.rotationRollPitchYawFromVector(Force);
		FQuat.normalize();
		ptrTrans->SetQuaternion(FQuat);
	}
}
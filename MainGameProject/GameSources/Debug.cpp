/*!
@breif	デバック用クラス実体
@name	作成者：阿部達哉
*/
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void DebugSprite::OnCreate()
	{
		auto DrawComp = AddComponent<PCTSpriteDraw>();
		DrawComp->CreateMesh<VertexPositionColorTexture>(m_vertices, m_indices);
		DrawComp->SetTextureResource(m_TexKey);
		DrawComp->SetDiffuse(Col4(1, 1, 1, 1));
		SetAlphaActive(true);


	}

	void ShaderTest::OnCreate()
	{

		Vec3 color(1, 1, 1);
		Vec2 tipSize = Vec2(1.0f, 1.0f);

		vector<VertexPositionNormalTexture>vertices =
		{
			{Vec3(-1.0f,+1.0f,0.0f), Vec3(1),Vec2(0		,0)},
			{Vec3(+1.0f,+1.0f,0.0f), Vec3(1),Vec2(tipSize.x,0)},
			{Vec3(-1.0f,-1.0f,0.0f), Vec3(1),Vec2(0		,tipSize.y)},
			{Vec3(+1.0f,-1.0f,0.0f), Vec3(1),Vec2(tipSize.x,tipSize.y)},
		};

		vector<uint16_t> indices =
		{
			0,1,2,
			2,1,3,
		};


		auto DrawComp = AddComponent<PNTWaterDraw>();
		DrawComp->CreateOriginalMesh<VertexPositionNormalTexture>(vertices, indices);
		DrawComp->SetOriginalMeshUse(true);
		DrawComp->SetTextureResource(m_TexKey);
		DrawComp->SetDiffuse(Col4(1, 1, 1, 1));
		SetAlphaActive(true);
	}

	void ShaderTest::OnUpdate()
	{
		auto DrawComp = GetComponent<PNTWaterDraw>();
		m_TotalTime += App::GetApp()->GetElapsedTime();

		DrawComp->UpdateUV(m_TotalTime,0.0f);
	}

	void AnimSpriteTest::OnCreate()
	{
		auto DrawComp = AddComponent<PCTSpriteDraw>();
		DrawComp->CreateMesh<VertexPositionColorTexture>(m_vertices, m_indices);
		DrawComp->SetTextureResource(m_TexKey,false);
		DrawComp->SetDiffuse(Col4(1, 1, 1, 0));
		DrawComp->SetEmissive(Col4(1, 0, 0, 0));
		SetAlphaActive(true);
	}

	void AnimSpriteTest::OnUpdate()
	{
		if (!m_IsLoop) 
		{
			auto DrawComp = GetComponent<PCTSpriteDraw>();
			auto Diffuse = DrawComp->GetDiffuse();
			if (!m_IsActived)
			{
				if (Diffuse.w >= 1.0f)
				{
					m_vol = -1.0f;
				}
				if (Diffuse.w < 0.0f) 
				{
					m_IsActived = true;
					GetStage()->GetSharedGameObject<Player>(L"Player")->SetState(PlayerState::Excute);
				}
				Diffuse.w += m_vol * App::GetApp()->GetElapsedTime();
				DrawComp->SetDiffuse(Diffuse);
			}
		}
		else
		{
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			m_TotalTime += ElapsedTime * 5.0f;
			if (m_TotalTime >= XM_2PI) {
				m_TotalTime = 0;
			}
			auto PtrDraw = GetComponent<PCTSpriteDraw>();
			Col4 col(1.0, 1.0, 1.0, 1.0);
			col.w = sin(m_TotalTime) * 0.5f + 0.5f;
			PtrDraw->SetDiffuse(col);

		}
	}

	//操作テスト
	void ContTest::OnCreate()
	{

	}

	void ContTest::OnUpdate()
	{
		m_handler.PushHandler(GetThis<ContTest>());
	}

	void ContTest::OnPushA()
	{
		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), m_StageStr);
	}


	//ステージ配置テスト
	StageTest::StageTest(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr,pNode)
	{

	}

	void StageTest::OnCreate()
	{
		DefaultSettings();
		SetActions();
	}

	ActionTest::ActionTest(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr, pNode)
	{

	}

	void ActionTest::OnCreate()
	{
		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetMeshResource(L"DEFAULT_CUBE");

		//配置設定
		//auto TransComp = GetComponent<Transform>();
		//TransComp->SetPosition(m_pos);
		//TransComp->SetQuaternion(Quat(m_rot));
		//TransComp->SetScale(m_scal);



		auto ActionComp = AddComponent<Actions>();
		ActionComp->AddMoveBy(L"Test", 2.0f, Vec3(0, 0, 5.0f));
		ActionComp->AddMoveBy(L"Test", 2.0f, Vec3(0, 0, -5.0f));
		ActionComp->AddMoveTo(L"Test2", 2.0f, Vec3(2.0f, 0, 5.0f));
		ActionComp->AddMoveTo(L"Test2", 2.0f, Vec3(-2.0f, 0, -5.0f));

		ActionComp->AddRotateTo(L"Test2", 2.0f, Quat(Vec4(0.5f)));

		ActionComp->Run(L"Test2");
	}

	//ステージ配置テスト
	GoalTest::GoalTest(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr, pNode),m_IsGoal(false),m_count(0.0f)
	{

	}

	void GoalTest::OnCreate()
	{
		DefaultSettings();
		//描画設定
		//auto DrawComp = AddComponent<PNTStaticDraw>();
		//マップに登録されてない（DEFAULT_CUBE, TEST_TXになってる）↓
		//DrawComp->SetMeshResource(L"GOAL_MD");
		//DrawComp->SetTextureResource(L"GOAL_TX");

		//配置設定
		auto TransComp = GetComponent<Transform>();
		m_pos.y += -0.5f;

		auto CollComp = AddComponent<CollisionObb>();
		CollComp->SetAfterCollision(AfterCollision::None);
		//マップにアクティブ化してない↓
		GetStage()->SetSharedGameObject(L"Goal", GetThis<GoalTest>());

		
	}

	void GoalTest::OnUpdate()
	{
		if (m_IsGoal)
		{
			m_count += App::GetApp()->GetElapsedTime();
			if (m_count > 2.0f)
			{
				auto  StageSelect = GameManager::GetManager()->GetStagePair();
				auto MaxStageCount = GameManager::GetManager()->GetMaxStagePair();
				if (StageSelect.second != MaxStageCount.second-1)
				{
					StageSelect.second += 1;
					GameManager::GetManager()->SetStagePair(StageSelect);
					PostEvent(0.0, GetThis<ObjectInterface>(), L"Camera", L"Clear",L"ToGameStage");
					GameManager::GetManager()->SetStartCameraActive(false);
				}
				else
				{
					if (StageSelect.first != MaxStageCount.first-1)
					{
						StageSelect.first += 1;
						StageSelect.second = 0;
						GameManager::GetManager()->SetStagePair(StageSelect);
						PostEvent(0.0, GetThis<ObjectInterface>(), L"Camera", L"Clear", L"ToGameStage");
						GameManager::GetManager()->SetStartCameraActive(false);
					}
					else 
					{
						PostEvent(0.0, GetThis<ObjectInterface>(), L"Camera", L"Clear", L"ToEndingStage");
					}
				}
				m_IsGoal = false;
			}
		}
	}
}
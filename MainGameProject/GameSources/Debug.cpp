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

		vector<VertexPositionTexture>vertices =
		{
			{Vec3(-1.0f,+1.0f,0.0f),Vec2(0		,0)},
			{Vec3(+1.0f,+1.0f,0.0f),Vec2(tipSize.x,0)},
			{Vec3(-1.0f,-1.0f,0.0f),Vec2(0		,tipSize.y)},
			{Vec3(+1.0f,-1.0f,0.0f),Vec2(tipSize.x,tipSize.y)},
		};

		vector<uint16_t> indices =
		{
			0,1,2,
			2,1,3,
		};


		auto DrawComp = AddComponent<PTWaterDraw>();
		DrawComp->CreateOriginalMesh<VertexPositionTexture>(vertices, indices);
		DrawComp->SetOriginalMeshUse(true);
		DrawComp->SetTextureResource(m_TexKey);
		DrawComp->SetDiffuse(Col4(1, 1, 1, 1));
		SetAlphaActive(true);
	}

	void ShaderTest::OnUpdate()
	{
		auto DrawComp = GetComponent<PTWaterDraw>();
		m_TotalTime += App::GetApp()->GetElapsedTime();

		DrawComp->UpdateUV(m_TotalTime,0.0f);
	}

	void AnimSpriteTest::OnCreate()
	{
		auto DrawComp = AddComponent<PCTSpriteDraw>();
		DrawComp->CreateMesh<VertexPositionColorTexture>(m_vertices, m_indices);
		DrawComp->SetTextureResource(m_TexKey);
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
					m_IsActived = true;
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
	}

	//--------------------------------------------------------------------------------------
	//	数字のスクエア
	//--------------------------------------------------------------------------------------
	//構築と破棄
	NumberSquare::NumberSquare(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<RockTest>& SeekObjectPtr, size_t Number) :
		GameObject(StagePtr),
		m_SeekObject(SeekObjectPtr),
		m_Number(Number)
	{}
	NumberSquare::~NumberSquare() {}

	//初期化
	void NumberSquare::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();
		if (!m_SeekObject.expired()) {
			auto SeekPtr = m_SeekObject.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += 0.75f;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
			PtrTransform->SetQuaternion(SeekTransPtr->GetQuaternion());
			//変更できるスクエアリソースを作成

			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Squareの作成(ヘルパー関数を利用)
			MeshUtill::CreateSquare(1.0f, vertices, indices);
			//UV値の変更
			float from = ((float)m_Number) / 10.0f;
			float to = from + (1.0f / 10.0f);
			//左上頂点
			vertices[0].textureCoordinate = Vec2(from, 0);
			//右上頂点
			vertices[1].textureCoordinate = Vec2(to, 0);
			//左下頂点
			vertices[2].textureCoordinate = Vec2(from, 1.0f);
			//右下頂点
			vertices[3].textureCoordinate = Vec2(to, 1.0f);
			//頂点の型を変えた新しい頂点を作成
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//新しい頂点を使ってメッシュリソースの作成
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, indices, true);

			auto DrawComp = AddComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(L"Number_TX");
			SetAlphaActive(true);
			SetDrawLayer(1);
		}

	}


	//変化
	void NumberSquare::OnUpdate() {

		if (!m_SeekObject.expired()) {
			auto SeekPtr = m_SeekObject.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();

			auto PtrTransform = GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += 1.0f;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(1.0f, 1.0f, 1.0f);

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quat Qt;
			//向きをビルボードにする
			Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			PtrTransform->SetQuaternion(Qt);

		}

	}


	//ステージ配置テスト
	RockTest::RockTest(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr, pNode)
	{
		auto NumberStr = XmlDocReader::GetAttribute(pNode, L"Number");

		num = (int)_wtoi(NumberStr.c_str());
	}

	void RockTest::OnCreate()
	{
		//描画設定
		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetMeshResource(m_meshKey);
		//DrawComp->SetTextureResource(m_texKey);

		//配置設定
		auto TransComp = GetComponent<Transform>();
		TransComp->SetPosition(m_pos);
		TransComp->SetQuaternion(Quat(m_rot));
		TransComp->SetScale(m_scal);

		auto CollComp = AddComponent<CollisionObb>();
		CollComp->SetAfterCollision(AfterCollision::None);

		//数字追加
		GetStage()->AddGameObject<NumberSquare>(GetThis<RockTest>(), num);

		GetStage()->GetSharedObjectGroup(L"Rock")->IntoGroup(GetThis<RockTest>());
	}

	//ステージ配置テスト
	GoalTest::GoalTest(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:ObjectBase(StagePtr, pNode),m_IsGoal(false),m_count(0.0f)
	{

	}

	void GoalTest::OnCreate()
	{
		//描画設定
		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetMeshResource(L"GOAL_MD");
		DrawComp->SetTextureResource(L"GOAL_TX");

		//配置設定
		auto TransComp = GetComponent<Transform>();
		m_pos.y += -0.5f;
		TransComp->SetPosition(m_pos);
		TransComp->SetQuaternion(Quat(m_rot));
		TransComp->SetScale(m_scal);

		auto CollComp = AddComponent<CollisionObb>();
		CollComp->SetAfterCollision(AfterCollision::None);
	}

	void GoalTest::OnUpdate()
	{
		if (m_IsGoal)
		{
			m_count += App::GetApp()->GetElapsedTime();
			if (m_count > 2.0f)
			{
				auto  StageSelect = GameManager::GetManager()->GetStagePair();
				if (StageSelect.second != 4)
				{
					StageSelect.second += 1;
					GameManager::GetManager()->SetStagePair(StageSelect);
					PostEvent(0.0, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
				}
				else
				{
					PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToEndingStage");
				}
			}
		}
	}
}
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
		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToDataSelectStage");
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
	}
}
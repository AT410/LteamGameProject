/*!
@breif	�f�o�b�N�p�N���X����
@name	�쐬�ҁF�����B��
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

	//����e�X�g
	void ContTest::OnCreate()
	{

	}

	void ContTest::OnUpdate()
	{
		m_Handler.PushHandler(GetThis<ContTest>());
	}

	void ContTest::OnPushA()
	{
		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToDataSelectStage");
	}
}
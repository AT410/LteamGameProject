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
}
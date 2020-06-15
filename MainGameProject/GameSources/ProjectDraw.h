/*!
@breif	�v���W�F�N�g��`�̕`��N���X
@name	�쐬�ҁF�����B��
*/
#pragma once
#include "stdafx.h"

namespace basecross
{
	//----------------------------------------------------------------------------
	//���`��N���X
	//----------------------------------------------------------------------------
	class PNTWaterDraw :public SmBaseDraw
	{
	public:
		explicit PNTWaterDraw(const shared_ptr<GameObject>&GameObejctPtr)
			:SmBaseDraw(GameObejctPtr), m_Cont() {}

		virtual ~PNTWaterDraw() {}

		void OnCreate()override {};

		void OnUpdate()override {};

		void OnDraw()override;

		void UpdateUV(float u, float v)
		{
			m_Cont.BehaviorVal.x = u;
			m_Cont.BehaviorVal.y = v;
		}

		void SetSubTexResource(const wstring& TexKey)
		{
			m_SubTex = App::GetApp()->GetResource<TextureResource>(TexKey);
		}

		void SetMaskTexResource(const wstring& TexKey)
		{
			m_MaskTex = App::GetApp()->GetResource<TextureResource>(TexKey);
		}

	private:
		ShaderControl m_Cont;
		shared_ptr<TextureResource> m_SubTex, m_MaskTex;
	};

	//-----------------------------------------------------------------------------
	//�_�������C�e�B���O
	//-----------------------------------------------------------------------------
	class PNTPointDraw :public SmBaseDraw
	{
	public:
		explicit PNTPointDraw(const shared_ptr<GameObject>&GameObejctPtr)
			:SmBaseDraw(GameObejctPtr) {}

		virtual ~PNTPointDraw() {}

		void OnCreate()override {};

		void OnUpdate()override {};

		void OnDraw()override;
	};
}
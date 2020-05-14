/*!
@breif	プロジェクト定義の描画クラス
@name	作成者：阿部達哉
*/
#pragma once
#include "stdafx.h"

namespace basecross
{
	class PTWaterDraw :public SmBaseDraw
	{
	public:
		explicit PTWaterDraw(const shared_ptr<GameObject>&GameObejctPtr)
			:SmBaseDraw(GameObejctPtr), m_Cont() {}

		virtual ~PTWaterDraw() {}

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

}
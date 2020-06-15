/*!
@file ProjectShader.h
@brief プロジェクトで使用するシェーダー
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//----------------------------------------------------------------------------
	//水用コンスタントバッファ構造体
	//----------------------------------------------------------------------------
	struct WaterBuffer
	{
		Mat4x4 World;
		Mat4x4 View;
		Mat4x4 Proj;

		Vec4 AmbientLight;

		bsm::Col4 Diffuse;
		bsm::Col4 Emissive;

		XMUINT4 ActiveFlag;

		WaterBuffer()
		{
			memset(this, 0, sizeof(WaterBuffer));
			ActiveFlag = XMUINT4(0, 0, 0, 0);
		}
	};

	//----------------------------------------------------------------------------
	//シェーダコントロール用バッファ構造体
	//----------------------------------------------------------------------------
	struct ShaderControl
	{
		XMUINT4 TexFlag;
		Vec4 BehaviorVal;

		ShaderControl()
		{
			memset(this, 0, sizeof(ShaderControl));
			TexFlag = XMUINT4(0, 0, 0, 0);
			BehaviorVal = Vec4(0, 0, 0.5f, 0);
		}
	};

	DECLARE_DX11_CONSTANT_BUFFER(WBuffer, WaterBuffer)
	DECLARE_DX11_CONSTANT_BUFFER(ControlBuffer, ShaderControl);

	DECLARE_DX11_VERTEX_SHADER(VSWater, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSWater)

	//-----------------------------------------------------------------------------
	//点光源
	//-----------------------------------------------------------------------------
	struct PointBuffer
	{
		Mat4x4 World;
		Mat4x4 View;
		Mat4x4 Proj;

		Vec4 LightPos;
		Vec4 Attenuation;

		bsm::Col4 Diffuse;
		bsm::Col4 Emissive;

		XMUINT4 ActiveFlag;

		PointBuffer()
		{
			memset(this, 0, sizeof(PointBuffer));
			ActiveFlag = XMUINT4(0, 0, 0, 0);
		}

	};

	DECLARE_DX11_CONSTANT_BUFFER(PBuffer, PointBuffer)

	DECLARE_DX11_VERTEX_SHADER(VSPoint, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPoint)

}
//end basecross


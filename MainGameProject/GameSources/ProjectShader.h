/*!
@file ProjectShader.h
@brief プロジェクトで使用するシェーダー
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	struct BufferTest
	{
		Mat4x4 World;
		Mat4x4 View;
		Mat4x4 Proj;

		bsm::Col4 Diffuse;
		bsm::Col4 Emissive;

		XMUINT4 ActiveFlag;

		BufferTest()
		{
			memset(this, 0, sizeof(BufferTest));
			ActiveFlag = XMUINT4(0, 0, 0, 0);
		}
	};

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

	DECLARE_DX11_CONSTANT_BUFFER(TestBuffer, BufferTest)
	DECLARE_DX11_CONSTANT_BUFFER(ControlBuffer, ShaderControl);

	DECLARE_DX11_VERTEX_SHADER(VSWater, VertexPositionTexture)
	DECLARE_DX11_PIXEL_SHADER(PSWater)

}
//end basecross


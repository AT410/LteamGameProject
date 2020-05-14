/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	IMPLEMENT_DX11_CONSTANT_BUFFER(TestBuffer)

		IMPLEMENT_DX11_CONSTANT_BUFFER(ControlBuffer)

		IMPLEMENT_DX11_VERTEX_SHADER(VSWater, App::GetApp()->GetShadersPath() + L"WaterVS.cso")

		IMPLEMENT_DX11_PIXEL_SHADER(PSWater, App::GetApp()->GetShadersPath() + L"WaterPS.cso")


}
//end basecross



/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	IMPLEMENT_DX11_CONSTANT_BUFFER(WBuffer)

	IMPLEMENT_DX11_CONSTANT_BUFFER(ControlBuffer)

	IMPLEMENT_DX11_VERTEX_SHADER(VSWater, App::GetApp()->GetShadersPath() + L"WaterVS.cso")

	IMPLEMENT_DX11_PIXEL_SHADER(PSWater, App::GetApp()->GetShadersPath() + L"WaterPS.cso")


	IMPLEMENT_DX11_CONSTANT_BUFFER(PBuffer)

	IMPLEMENT_DX11_VERTEX_SHADER(VSPoint, App::GetApp()->GetShadersPath() + L"PointVS.cso")

	IMPLEMENT_DX11_PIXEL_SHADER(PSPoint, App::GetApp()->GetShadersPath() + L"PointPS.cso")

}
//end basecross



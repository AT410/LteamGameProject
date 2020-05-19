/*!
@breif	�v���W�F�N�g��`�̕`��N���X����
@name	�쐬�ҁF�����B��
*/

#include"stdafx.h"
#include "Project.h"

namespace basecross
{
	void PNTWaterDraw::OnDraw()
	{
		auto MeshRes = GetMeshResource();
		if (!MeshRes)
		{
			throw(BaseException(L"", L"", L""));
		}

		//�f�o�C�X�̏���
		auto DeviceRes = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = DeviceRes->GetD3DDeviceContext();
		auto RenderState = DeviceRes->GetRenderState();


		//NULL�̃V�F�[�_���\�[�X�̏���
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };

		auto PtrGameObject = GetGameObject();

		auto PtrT = PtrGameObject->GetComponent<Transform>();
		//�J�����𓾂�
		auto PtrCamera = PtrGameObject->OnGetDrawCamera();
		//�J�����̎擾
		Mat4x4 View, Proj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();

		//�R���X�^���g�o�b�t�@�̐ݒ�
		BufferTest cb1;
		//�s��̐ݒ�(�]�u����)
		cb1.World = transpose(PtrT->GetWorldMatrix());
		cb1.View = transpose(View);
		cb1.Proj = transpose(Proj);

		//����
		auto Light = PtrGameObject->OnGetDrawLight();
		cb1.AmbientLight = Light.m_Directional;
		cb1.AmbientLight.w = 1.0f;

		cb1.Diffuse = GetDiffuse();
		cb1.Emissive = GetEmissive();

		//�R���X�^���g�o�b�t�@�̍X�V
		pID3D11DeviceContext->UpdateSubresource(TestBuffer::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//���_�o�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�X�e�[�^�X�̃|�C���^
		//����������
		float blendfact[4] = { 0.0f,0.0f,0.0f,0.0f };
		pID3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), blendfact, D3D11_DEFAULT_SAMPLE_MASK);
		//�f�v�X�X�e���V���͎g�p����
		RenderState->SetBlendState(pID3D11DeviceContext, GetBlendState());
		pID3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(VSWater::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSWater::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(VSWater::GetPtr()->GetInputLayout());

		//�e�N�X�`��
		auto shTex = GetTextureResource();
		if (shTex) {
			//�e�N�X�`��������
			m_Cont.TexFlag.x = 1;
			pID3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
			//�T���v���[��ݒ�
			RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 0);

			//SubTex
			if (m_SubTex)
			{
				//�e�N�X�`��������
				m_Cont.TexFlag.y = 1;
				pID3D11DeviceContext->PSSetShaderResources(1, 1, m_SubTex->GetShaderResourceView().GetAddressOf());
				//�T���v���[��ݒ�
				RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 1);

				if (m_MaskTex)
				{
					//�e�N�X�`��������
					m_Cont.TexFlag.z = 1;
					pID3D11DeviceContext->PSSetShaderResources(2, 1, m_MaskTex->GetShaderResourceView().GetAddressOf());
					//�T���v���[��ݒ�
					RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 2);

				}
			}

		}
		else {
			//�`��R���|�[�l���g�ɂ̓e�N�X�`�����Ȃ�
			m_Cont.TexFlag.x = 0;
			//�V�F�[�_�[���\�[�X���N���A
			pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
			//�T���v���[���N���A
			RenderState->SetSamplerAllClear(pID3D11DeviceContext);
		}

		//�R���g���[���o�b�t�@�̍X�V
		pID3D11DeviceContext->UpdateSubresource(ControlBuffer::GetPtr()->GetBuffer(), 0, nullptr, &m_Cont, 0, 0);

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = TestBuffer::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//�R���g���[���o�b�t�@�̐ݒ�
		ID3D11Buffer* pControlBuffer = ControlBuffer::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(1, 1, &pControlBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(1, 1, &pControlBuffer);

		//
		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderState->GetCullFront());
		//�`��
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//�`��
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//��n��
		DeviceRes->InitializeStates();
	}
}
/*!
@breif	プロジェクト定義の描画クラス実体
@name	作成者：阿部達哉
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

		//デバイスの準備
		auto DeviceRes = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = DeviceRes->GetD3DDeviceContext();
		auto RenderState = DeviceRes->GetRenderState();


		//NULLのシェーダリソースの準備
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };

		auto PtrGameObject = GetGameObject();

		auto PtrT = PtrGameObject->GetComponent<Transform>();
		//カメラを得る
		auto PtrCamera = PtrGameObject->OnGetDrawCamera();
		//カメラの取得
		Mat4x4 View, Proj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();

		//コンスタントバッファの設定
		BufferTest cb1;
		//行列の設定(転置する)
		cb1.World = transpose(PtrT->GetWorldMatrix());
		cb1.View = transpose(View);
		cb1.Proj = transpose(Proj);

		//環境光
		auto Light = PtrGameObject->OnGetDrawLight();
		cb1.AmbientLight = Light.m_Directional;
		cb1.AmbientLight.w = 1.0f;

		cb1.Diffuse = GetDiffuse();
		cb1.Emissive = GetEmissive();

		//コンスタントバッファの更新
		pID3D11DeviceContext->UpdateSubresource(TestBuffer::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//
		//ストライドとオフセット
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//ステータスのポインタ
		//半透明処理
		float blendfact[4] = { 0.0f,0.0f,0.0f,0.0f };
		pID3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), blendfact, D3D11_DEFAULT_SAMPLE_MASK);
		//デプスステンシルは使用する
		RenderState->SetBlendState(pID3D11DeviceContext, GetBlendState());
		pID3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSWater::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSWater::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSWater::GetPtr()->GetInputLayout());

		//テクスチャ
		auto shTex = GetTextureResource();
		if (shTex) {
			//テクスチャがある
			m_Cont.TexFlag.x = 1;
			pID3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
			//サンプラーを設定
			RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 0);

			//SubTex
			if (m_SubTex)
			{
				//テクスチャがある
				m_Cont.TexFlag.y = 1;
				pID3D11DeviceContext->PSSetShaderResources(1, 1, m_SubTex->GetShaderResourceView().GetAddressOf());
				//サンプラーを設定
				RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 1);

				if (m_MaskTex)
				{
					//テクスチャがある
					m_Cont.TexFlag.z = 1;
					pID3D11DeviceContext->PSSetShaderResources(2, 1, m_MaskTex->GetShaderResourceView().GetAddressOf());
					//サンプラーを設定
					RenderState->SetSamplerState(pID3D11DeviceContext, GetSamplerState(), 2);

				}
			}

		}
		else {
			//描画コンポーネントにはテクスチャがない
			m_Cont.TexFlag.x = 0;
			//シェーダーリソースもクリア
			pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
			//サンプラーもクリア
			RenderState->SetSamplerAllClear(pID3D11DeviceContext);
		}

		//コントロールバッファの更新
		pID3D11DeviceContext->UpdateSubresource(ControlBuffer::GetPtr()->GetBuffer(), 0, nullptr, &m_Cont, 0, 0);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = TestBuffer::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//コントロールバッファの設定
		ID3D11Buffer* pControlBuffer = ControlBuffer::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(1, 1, &pControlBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(1, 1, &pControlBuffer);

		//
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderState->GetCullFront());
		//描画
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//描画
		pID3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//後始末
		DeviceRes->InitializeStates();
	}
}
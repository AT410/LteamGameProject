/*
@breif プロジェクト定義のシェーダーファイル
@name 作成者：阿部達哉
*/

//コンスタントバッファ
cbuffer ConstantBuffer : register(b0)
{
    float4x4 WorldMatrix : packoffset(c0);
    float4x4 ViewMatrix : packoffset(c4);
    float4x4 ProjectionMatrix : packoffset(c8);
    
    float4 Diffuse : packoffset(c9);
    float4 Emissive : packoffset(c10);
}

//シェーダー制御用バッファ
cbuffer ControlBuffer :register(b1)
{
    
}
/*!
@breif  �V�F�[�_�[�w�b�_
@name   �쐬�ҁF�����B��
*/

cbuffer ConstantBuffer : register(b0)
{
    float4x4 World : packoffset(c0);
    float4x4 View : packoffset(c4);
    float4x4 Proj : packoffset(c8);
    
    float4 AmbientLight : packoffset(c12);
    
    float4 DiffuseCol : packoffset(c13);
    float4 EmissveCol : packoffset(c14);
    
    uint4 ActiveFlag : packoffset(c15);//Active
}

cbuffer ControlBuffer : register(b1)
{
    uint4 TexFlag : packoffset(c0);
    float4 BehaviorVal : packoffset(c1);
}

//VS
struct VSPNTInput
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

struct PSPNTInput
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

//�ėp�֐�
//�O���[�X�P�[���ϊ�
float4 ConvertToGray(float4 col)
{
    const float3 lumcoeff = { 0.3, 0.59, 0.11 };
    float4 result = col;
    result.rgb = dot(col.rgb, lumcoeff);
    return result;
}
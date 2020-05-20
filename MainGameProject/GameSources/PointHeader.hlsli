/*!
@breif “_ŒõŒ¹ƒwƒbƒ_
*/

struct VSPNTInput
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
};

struct PSPNTInput
{
    float4 pos : SV_POSITION;
    float4 posw : POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD;
};

cbuffer PointBuffer : register(b0)
{
    float4x4 World : packoffset(c0);
    float4x4 View : packoffset(c4);
    float4x4 Proj : packoffset(c8);
    
    float4 Light : packoffset(c12);
    float4 Attenuation : packoffset(c13);
    
    float4 DiffuseCol : packoffset(c14);
    float4 EmissveCol : packoffset(c15);
    
    uint4 ActiveFlag : packoffset(c16);
}
#include "PointHeader.hlsli"

Texture2D g_Texture : register(t0);

SamplerState g_Samp : register(s0);

float4 main(PSPNTInput input) : SV_TARGET
{
    //•ûŒü
    float3 dir = Light.xyz - input.posw.xyz;
    
    float len = length(dir);
    
    dir = dir / len;
    
    float colD = saturate(dot(input.norm.xyz, dir));
    
    float colA = saturate(1.0f / (Attenuation.x + Attenuation.y * len + Attenuation.z * len * len));
    
    float col = colD * colA;
    
    float4 Light = float4(col, col, col, 1.0f) * DiffuseCol + EmissveCol;
    
    float4 Tex = float4(1, 1, 1, 1);
    
    if (ActiveFlag.x)
    {
        Tex = g_Texture.Sample(g_Samp, input.tex);
    }    
    
    float4 result = Tex * Light;
    result.a = 1.0f;
    return result;
}
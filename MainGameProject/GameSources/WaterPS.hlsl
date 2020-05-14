#include "Header.hlsli"

Texture2D g_Texture : register(t0);
Texture2D g_SubTex : register(t1);
Texture2D g_MaskTex : register(t2);

SamplerState g_Samp : register(s0);
SamplerState s_SubSamp : register(s1);
SamplerState s_MaskSamp : register(s2);

float4 main(PSPTInput input) : SV_Target
{
    float4 Light = DiffuseCol + EmissveCol;
    
    Light.a = DiffuseCol.a;
    
    float4 tex = g_Texture.Sample(g_Samp, input.tex - BehaviorVal.xy);
    
    float4 Sc = g_SubTex.Sample(s_SubSamp, input.tex - BehaviorVal.xy * 1.5f);
    float4 Mkc = g_MaskTex.Sample(s_MaskSamp, input.tex-BehaviorVal.xy*1.25f);
    
    float4 Gray = ConvertToGray(Mkc);
    
    tex.a = BehaviorVal.z;
    Sc.a = BehaviorVal.z;
    
    float4 result = lerp(tex, Sc, Gray);
    
    return result;
}
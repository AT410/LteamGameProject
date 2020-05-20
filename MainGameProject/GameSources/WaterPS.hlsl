#include "Header.hlsli"

Texture2D g_Texture : register(t0);
Texture2D g_SubTex : register(t1);
Texture2D g_MaskTex : register(t2);

SamplerState g_Samp : register(s0);
SamplerState s_SubSamp : register(s1);
SamplerState s_MaskSamp : register(s2);

float4 main(PSPNTInput input) : SV_Target
{
    //ライトの計算
	//法線ライティング
    float3 lightdir = normalize(AmbientLight.xyz);
    float3 N1 = normalize(input.normal);
    float4 Light = (saturate(dot(N1, lightdir)) * DiffuseCol) + EmissveCol;
    Light.a = DiffuseCol.a;
    
    if (TexFlag.x==1 && TexFlag.y==1 && TexFlag.z==1)
    {
        float4 MainTex = g_Texture.Sample(g_Samp, input.tex - BehaviorVal.xy);
        float4 SubTex = g_SubTex.Sample(s_SubSamp, input.tex - BehaviorVal.xy * 1.5f);
        float4 MaskTex = g_MaskTex.Sample(s_MaskSamp, input.tex - BehaviorVal.xy * 1.25f);
    
        float4 Gray = ConvertToGray(MaskTex);
    
    
        float4 result = lerp(MainTex, SubTex, Gray) * Light;
    
        return result;

    }
    return Light;
}
#include "Header.hlsli"

PSPNTInput main(VSPNTInput input)
{
    PSPNTInput result;
    float4 pos = float4(input.pos.xyz, 1.0f);
    pos = mul(pos, World);
    pos = mul(pos, View);
    pos = mul(pos, Proj);
    
    result.pos = pos;
    
    //NORMAL
    result.normal = mul(input.normal, (float3x3)World);
    result.normal = normalize(result.normal);
    
    result.tex = input.tex;
    return result;
}
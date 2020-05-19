#include "PointHeader.hlsli"

PSPNTInput main(VSPNTInput input)
{
    PSPNTInput result;
    
    float4 pos = float4(input.pos.xyz,1.0f);
    
    pos = mul(pos, World);
    result.posw = pos;
    pos = mul(pos, View);
    pos = mul(pos, Proj);
    
    //Normal
    result.norm = mul(input.norm, (float3x3)World);
    result.norm = normalize(result.norm);
    
    result.pos = pos;
    
    result.tex = input.tex;
    return result;

}
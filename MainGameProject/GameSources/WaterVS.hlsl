#include "Header.hlsli"

PSPTInput main(VSPTInput input)
{
    PSPTInput result;
    float4 pos = float4(input.pos.xyz, 1.0f);
    pos = mul(pos, World);
    pos = mul(pos, View);
    pos = mul(pos, Proj);
    
    result.pos = pos;
    result.tex = input.tex;
    return result;
}
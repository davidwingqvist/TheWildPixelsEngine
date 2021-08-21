Texture2D tex : register(t0);
sampler samp : register(s0);

cbuffer colorBuffer : register(b0)
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
}

struct VSOut
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(VSOut input) : SV_TARGET
{
    // If color buffer should be used.
    if(color.w > 0)
        return color;
    
    return tex.Sample(samp, input.uv);
}
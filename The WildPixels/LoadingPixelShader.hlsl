SamplerState samp : register(s0);
Texture2D tex : register(t0);

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(VSOut input) : SV_TARGET
{
    return tex.Sample(samp, input.uv);
}
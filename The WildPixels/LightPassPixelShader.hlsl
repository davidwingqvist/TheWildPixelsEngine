Texture2D ambientTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState Asampler : register(s0);

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(VSOut input) : SV_TARGET
{
    return ambientTexture.Sample(Asampler, input.uv);
}
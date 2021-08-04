SamplerState Ssampler : register(s0);
Texture2D Ttexture : register(t0);

struct VSOut
{
    float4 position : SV_POSITION;
    float4 positionW : POS_W;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct PSOut
{
    float4 ambientTex : SV_Target0;
    float4 normal : SV_Target1;
};

PSOut main(VSOut input)
{
    PSOut output;
    
    output.normal = float4(input.normal, 0.0f);
    output.ambientTex = Ttexture.Sample(Ssampler, input.uv);
    
    return output;
}
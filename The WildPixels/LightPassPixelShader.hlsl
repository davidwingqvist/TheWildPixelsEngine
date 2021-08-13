Texture2D ambientTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState Asampler : register(s0);


/*
    Light caluclations
*/
void doDirectionalLight(inout float4 color, int arrSpot);
struct Light
{
    float4 position;
    float4 direction;
    float4 attenuation;
};

struct LightProp
{
    float4 ambient;
    float4 specular;
    float4 diffuse;
};

StructuredBuffer<Light> LightStructs : register(t10);
StructuredBuffer<LightProp> LightProperties : register(t11);
cbuffer LightRules : register(b10)
{
    float amount = 0;
    float range;
    float unknown;
    float unknown2;
};

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

struct GBuffers
{
    float4 ambientTexture;
    float4 normalTexture;
};

// Sample from the buffers.
GBuffers CreateBuffers(float2 uv)
{
    GBuffers buffs;
    buffs.ambientTexture = ambientTexture.Sample(Asampler, uv);
    buffs.normalTexture = normalTexture.Sample(Asampler, uv);
    
    return buffs;
}



float4 main(VSOut input) : SV_TARGET
{
    GBuffers buffers = CreateBuffers(input.uv);
    // If no lights exist then render uneditted ambient texture.
    if(amount <= 0)
    {
        return buffers.ambientTexture;
    }
    else // Calculate lights.
    {
        float4 color = buffers.ambientTexture;
        for (int i = 0; i < amount; i++)
        {
            switch (LightStructs[i].position.w)
            {
                case 0:
                    doDirectionalLight(color, i);
                    break;
                case 1:
                    break;
                case 2:
                    break;
                default:
                // default is just returning normal ambient color.
                    color = buffers.ambientTexture;
                    return color;
                    break;
            }
        }

        return color;
    }
    
    // Failed somewhere so we just return 1.0fs;
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

void doDirectionalLight(inout float4 color, int arrSpot)
{
    
}
cbuffer CameraMatrices : register(b1)
{
    matrix viewMatrix;
    matrix projectionMatrix;
    
    float3 position;
    float pad;
    float3 rotation;
    float pad2;
}

struct VSIn
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};


VSOut main( VSIn input )
{
    VSOut output;
    
    output.pos = float4(input.pos, 1.0f);
    output.uv = input.uv;
    
	return output;
}
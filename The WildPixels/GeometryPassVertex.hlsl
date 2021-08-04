cbuffer Matrices : register(b0)
{
    matrix modelMatrix;
}

cbuffer CameraMatrices : register(b1)
{
    matrix viewMatrix;
    matrix projectionMatrix;
    
    float3 position;
}

struct VSIn
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float4 positionW : POS_W;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

VSOut main( VSIn input)
{
    VSOut output;
    
    output.position = mul(float4(input.position, 1.0f), modelMatrix);
    output.positionW = mul(float4(input.position, 1.0f), modelMatrix);
    
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) modelMatrix);
    
    return output;
}
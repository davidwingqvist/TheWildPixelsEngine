#include "Canvas.h"
#include "ResourceManager.h"
#include "Graphics.h"

bool Canvas::CreateVertex()
{
    ScreenVertex screenQuad[4] =
    {
        { { position.x, position.y - (float)this->size.y, 0.0f }, { 0.0f, 1.0f } }, // BOTTOM LEFT
        { { position.x, position.y, 0.0f }, { 0.0f, 0.0f } },   // TOP LEFT
        { { position.x + (float)this->size.x, position.y, 0.0f }, { 1.0f, 0.0f } },   // TOP RIGHT
        { { position.x + (float)this->size.x, position.y - (float)this->size.y, 0.0f }, { 1.0f, 1.0f } }    // BOTTOM RIGHT
    };

    UINT indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    HRESULT hr;
    D3D11_BUFFER_DESC bDesc;
    bDesc.ByteWidth = (UINT)4 * sizeof(ScreenVertex);
    bDesc.Usage = D3D11_USAGE_DYNAMIC;
    bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = &(screenQuad[0]);

    hr = DEVICE->CreateBuffer(&bDesc, &data, &this->vertexBuffer);
    if (FAILED(hr))
        return false;

    bDesc.ByteWidth = sizeof(UINT) * 6;
    bDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bDesc.CPUAccessFlags = 0;
    bDesc.MiscFlags = 0;

    data.pSysMem = &(indices[0]);
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    hr = DEVICE->CreateBuffer(&bDesc, &data, &this->indexBuffer);
    if (FAILED(hr))
        return false;

    bDesc.ByteWidth = sizeof(DirectX::XMFLOAT4);
    bDesc.Usage = D3D11_USAGE_DYNAMIC;
    bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bDesc.MiscFlags = 0;

    // w == 1 means color should be used.
    DirectX::XMFLOAT4 color = { this->color.x, this->color.y, this->color.z, 1.0f };
    data.pSysMem = &color;

    hr = DEVICE->CreateBuffer(&bDesc, &data, &this->colorBuffer);
    if (FAILED(hr))
        return false;

    return true;
}

bool Canvas::CreateTexture(const std::string& filePath)
{
    this->texture = RESOURCE(Texture, std::string(filePath));
    if (!this->texture)
    {
        this->texture = new Texture(std::string(filePath));
        ADD_RESOURCE(this->texture, filePath);
    }

    if (this->texture == nullptr)
        return false;
    else
        return true;
}

void Canvas::UpdateBuffer(const bool&& useColor)
{
    DirectX::XMFLOAT4 color = {};
    if (useColor)
    {
        if (this->transparency <= 0.0f)
            this->transparency = 0.000001f;

        color = { this->color.x, this->color.y, this->color.z, this->transparency };
    }
    else
    {
        color = { 0.0f, 0.0f, 0.0f, -1.0f };
    }

    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    Graphics::GetContext()->Map(this->colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &color, sizeof(DirectX::XMFLOAT4));
    Graphics::GetContext()->Unmap(this->colorBuffer, 0);
}

void Canvas::Unbind()
{
}

Canvas::Canvas(const float&& x, const float&& y, const float&& width, const float height, const float&& red, const float&& blue, const float&& green)
{
    this->vertexBuffer = nullptr;
    this->indexBuffer = nullptr;
    this->blendState = nullptr;
    this->texture = nullptr;

    this->position.x = x;
    this->position.y = y;

    this->size.x = width;
    this->size.y = height;

    this->color.x = red;
    this->color.y = green;
    this->color.z = blue;

    this->CreateVertex();
}

Canvas::Canvas(Vector2D& position, Vector2D& size, Vector3D& color)
{
    this->vertexBuffer = nullptr;
    this->indexBuffer = nullptr;
    this->blendState = nullptr;
    this->texture = nullptr;

    this->position = position;
    this->size = size;
    this->color = color;

    this->CreateVertex();
}

const bool Canvas::ApplyTexture(std::string&& filePath)
{
    this->UpdateBuffer(false);
    return this->CreateTexture(filePath);
}

void Canvas::ApplyTransparency(float degree)
{
    this->transparency = degree;

    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = true;
    blendDesc.IndependentBlendEnable = true;
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;

    HRESULT hr = DEVICE->CreateBlendState(&blendDesc, &this->blendState);

    this->UpdateBuffer(true);
}

void Canvas::ApplyColor(Vector3D& color)
{
    this->color = color;

    this->UpdateBuffer(true);
}

void Canvas::ApplyColor(float red, float green, float blue)
{
    this->color.x = red;
    this->color.y = green;
    this->color.z = blue;

    this->UpdateBuffer(true);
}

void Canvas::ApplyDecal(Decal* decal, int pos)
{
    if (pos >= decal_amount || pos < 0)
        return;
    else
        this->decals[pos] = decal;
}

void Canvas::ApplyButton(Button* button, int pos)
{
    if (pos >= button_amount || pos < 0)
        return;
    else
        this->buttons[pos] = button;
}

void Canvas::RePosition(float* x, float* y)
{
    this->position.x = *x;
    this->position.y = *y;
}

void Canvas::ReSize(float* width, float* height)
{
    this->size.x = *width;
    this->size.y = *height;
}

Canvas::~Canvas()
{
    if (this->vertexBuffer)
        this->vertexBuffer->Release();
    if (this->indexBuffer)
        this->indexBuffer->Release();
    if (this->blendState)
        this->blendState->Release();
    if (this->colorBuffer)
        this->colorBuffer->Release();
}

void Canvas::Render()
{
    UINT stride = sizeof(ScreenVertex);
    UINT offset = 0;
    CONTEXT->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
    CONTEXT->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    CONTEXT->PSSetConstantBuffers(0, 1, &this->colorBuffer);

    if(this->texture)
        CONTEXT->PSSetShaderResources(0, 1, &this->texture->GetShaderView());

    ID3D11BlendState* nullState = nullptr;
    if (this->blendState)
        CONTEXT->OMSetBlendState(this->blendState, NULL, 0xffffffff);
    else
        CONTEXT->OMSetBlendState(nullState, NULL, 0xffffffff);

    /*
        Draw canvas!
    */
    CONTEXT->DrawIndexed(6, 0, 0);

    // So that other drawables wont use this canvas color
    ID3D11Buffer* nullBuff = nullptr;
    CONTEXT->PSSetConstantBuffers(0, 1, &nullBuff);
    CONTEXT->OMSetBlendState(nullState, NULL, 0xffffffff);

    for (int i = 0; i < button_amount; i++)
    {
        if (this->buttons[i])
            this->buttons[i]->Render();
    }

    for (int i = 0; i < decal_amount; i++)
    {
        if (this->decals[i])
            this->decals[i]->Render();
    }
}

const bool Canvas::Colliding(float* x, float* y)
{
    float x_pos = *(x);
    float y_pos = *(y);
    return false;
}

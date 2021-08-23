#include "Button.h"
#include "ResourceManager.h"
#include "Graphics.h"
#include <omp.h>

void Button::UpdateBuffer(const bool&& useColor)
{
    DirectX::XMFLOAT4 color = {};
    if (useColor)
    {
        color = { this->color.x, this->color.y, this->color.z, 1.0f };
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

bool Button::CreateVertex()
{
    ScreenVertex screenQuad[4] =
    {
        { { position.x, position.y - (float)this->height, 0.0f }, { 0.0f, 1.0f } }, // BOTTOM LEFT
        { { position.x, position.y, 0.0f }, { 0.0f, 0.0f } },   // TOP LEFT
        { { position.x + (float)this->width, position.y, 0.0f }, { 1.0f, 0.0f } },   // TOP RIGHT
        { { position.x + (float)this->width, position.y - (float)this->height, 0.0f }, { 1.0f, 1.0f } }    // BOTTOM RIGHT
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

Button::Button()
{
    this->position.x = -1.0f;
    this->position.y = 1.0f;
    this->CreateVertex();
}

Button::Button(float x, float y)
{
    this->position.x = x;
    this->position.y = y;
    this->CreateVertex();
}

Button::~Button()
{
    if (this->vertexBuffer)
        this->vertexBuffer->Release();
    if (this->indexBuffer)
        this->indexBuffer->Release();
    if (this->colorBuffer)
        this->colorBuffer->Release();
}

void Button::SetAsToggle()
{
    this->isToggle = true;
}

void Button::SetAsPress()
{
    this->isToggle = false;
}

void Button::Toggle()
{
    // Easy toggled between a bool.
    this->isToggledPressed = !this->isToggledPressed;
}

void Button::ApplyTexture(const std::string&& filePath)
{
    this->texture = RESOURCE(Texture, std::string(filePath));
    if (!this->texture)
    {
        this->texture = new Texture(std::string(filePath));
        ADD_RESOURCE(this->texture, filePath);
    }

    this->UpdateBuffer(false);
}

void Button::ApplyColor(Vector3D& color)
{
    this->color = color;
    this->UpdateBuffer(true);
}

void Button::ApplyColor(float red, float green, float blue)
{
    this->color.x = red;
    this->color.y = green;
    this->color.z = blue;
    this->UpdateBuffer(true);
}

void Button::Render()
{
    /*
        Decide which texture that should be rendered.
    */
    if (this->isToggle)
    {
        if (!this->isToggledPressed)
        {
            if (this->texture)
                CONTEXT->PSSetShaderResources(0, 1, &this->texture->GetShaderView());
        }
        else
        {
            if (this->toggledTexture)
                CONTEXT->PSSetShaderResources(0, 1, &this->toggledTexture->GetShaderView());
            else
                CONTEXT->PSSetShaderResources(0, 1, &this->texture->GetShaderView());
        }
    }

    UINT stride = 20U;
    UINT offset = 0;
    CONTEXT->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
    CONTEXT->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    CONTEXT->PSSetConstantBuffers(0, 1, &this->colorBuffer);

    /*
        Draw Button!
    */
    CONTEXT->DrawIndexed(6, 0, 0);

    // So that other drawables wont use this button color
    ID3D11Buffer* nullBuff = nullptr;
    CONTEXT->PSSetConstantBuffers(0, 1, &nullBuff);

    if (this->text.length() > 0)
    {
        // Convert the positions to range [winHeight, 0] : [winWidth, 0]
        float y = ((-position.y - (-1)) * (Graphics::GetHeight())) * 0.5f;
        float x = ((position.x - (-1)) * (Graphics::GetWidth())) * 0.5f;

        float w = ((width - (-1)) * (Graphics::GetWidth())) * 0.5f;
        float h = ((height - (-1)) * (Graphics::GetHeight())) * 0.5f;
        Graphics2D::Draw(text, x, y, (x / w) * Graphics::GetWidth(),  (y / h) * Graphics::GetHeight() - y * 0.3f);
    }
}

const bool Button::Colliding(float* x, float* y)
{
    // In this case, x and y is the position of the mouse.
    float x_pos = *(x);
    float y_pos = *(y);

    /*
    std::cout << "X: " << x_pos << " Y: " << y_pos << "\n" << "X2: " << position.x
        << " Y2: " << position.y << "\n" << "Width: " << position.x + width << " Height: " <<
        position.y + height << "\n";
        */

    if (x_pos >= position.x && x_pos <= position.x + width &&
        y_pos <= position.y && y_pos >= position.y - height)
        return true;

    return false;
}

void Button::ToggledTexture(const std::string&& filePath)
{
    this->toggledTexture = RESOURCE(Texture, std::string(filePath));
    if (!this->toggledTexture)
    {
        this->toggledTexture = new Texture(std::string(filePath));
        ADD_RESOURCE(this->toggledTexture, filePath);
    }

    this->UpdateBuffer(false);
}

void Button::ToggledColor(Vector3D& color)
{
    this->toggledColor = color;
    //this->UpdateBuffer(true);
}

void Button::ToggledColor(float red, float green, float blue)
{
    this->toggledColor.x = red;
    this->toggledColor.y = green;
    this->toggledColor.z = blue;
    //this->UpdateBuffer(true);
}

void Button::SetText(const std::string&& text)
{
    this->text = text;
}

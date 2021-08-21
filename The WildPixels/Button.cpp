#include "Button.h"

Button::Button()
{

}

Button::~Button()
{
    if (this->vertexBuffer)
        this->vertexBuffer->Release();
    if (this->indexBuffer)
        this->indexBuffer->Release();
}

void Button::Press()
{
    this->activate = true;
}

void Button::End()
{
    this->activate = false;
}

void Button::Render()
{
}

const bool Button::Colliding(float* x, float* y)
{
    return false;
}

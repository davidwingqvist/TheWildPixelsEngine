#include "Text2D.h"
#include "Graphics.h"

Text2D::Text2D(const std::string&& text)
{
    this->text = text;
}

Text2D::Text2D(const std::string& text)
{
    this->text = text;
}

void Text2D::SetPosition(float x, float y)
{
    this->position = { x, y };
    this->hasPosition = true;
}

void Text2D::SetText(const std::string&& text)
{
    this->text = text;
}

void Text2D::SetText(const std::string& text)
{
    this->text = text;
}

void Text2D::Render()
{
    if (this->text.length() > 0)
    {
        if(!hasPosition)
            Graphics2D::Draw(this->text);
        else
            Graphics2D::Draw(this->text, this->position.x, this->position.y);
    }
}

const bool Text2D::Colliding(float* x, float* y)
{
    return false;
}

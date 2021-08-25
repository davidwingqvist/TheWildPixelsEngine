#pragma once
#include <d3d11.h>
#include "GUnknown.h"

bool test;

class GTexture : public GUnkown
{
public:

	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* shader = nullptr;

public:

};


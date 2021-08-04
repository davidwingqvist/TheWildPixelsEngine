#pragma once
#include "Resource.h"
#include <string>

class Mesh : public Resource
{
private:

	ID3D11Buffer* vertexBuffer;
	int nrOfVertices;

public:

	Mesh(std::string&& filePath);
	~Mesh();

	ID3D11Buffer*& GetMesh();

	// Get amount of vertices in mesh.
	int GetVertCount();
};
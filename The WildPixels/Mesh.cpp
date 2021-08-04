#include "Mesh.h"
#include "ReadFromObj.h"
#include "Graphics.h"

Mesh::Mesh(std::string&& filePath)
	:Resource(ResourceType::MESH)
{
	this->vertexBuffer = nullptr;
	this->nrOfVertices = 0;

	std::vector<Vertex> vertices = ReadFromObj(filePath, this->nrOfVertices);

	// Mesh wasn't found. Break.
	if ((int)vertices.size() < 0)
		return;

	D3D11_BUFFER_DESC bDesc;
	bDesc.ByteWidth = sizeof(Vertex) * (int)vertices.size();
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bDesc.CPUAccessFlags = 0;
	bDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &(vertices[0]);
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	DEVICE->CreateBuffer(&bDesc, &data, &this->vertexBuffer);
}

Mesh::~Mesh()
{
	if (this->vertexBuffer)
		this->vertexBuffer->Release();
}

ID3D11Buffer*& Mesh::GetMesh()
{
	return this->vertexBuffer;
}

int Mesh::GetVertCount()
{
	return this->nrOfVertices;
}

#ifndef VERTEX_H
#define VERTEX_H

#include "D3D11.h"
#include <DirectXMath.h>

struct Vertex
{
	Vertex() : Pos(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f), Tex(0.0f, 0.0f) {}
	Vertex(float px, float py, float pz, float nx, float ny, float nz, float u, float v)
		: Pos(px, py, pz), Normal(nx, ny, nz), Tex(u, v) {}

	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 Tex;
};

class InputLayouts
{
public:
	static const D3D11_INPUT_ELEMENT_DESC BasicVertexLayout[3];
};

#endif // VERTEX_H
#ifndef GVERTEXSHADER_H
#define GVERTEXSHADER_H

#include "D3D11.h"
#include "GShader.h"

#include "malloc.h"
#include <vector>

__declspec(align(16))
class GVertexShader : public GShader
{
public:
	GVertexShader();
	~GVertexShader();

	inline ID3D11VertexShader* GetShader() { return mShader; }
	inline void SetShader(ID3D11VertexShader* vs) { mShader = vs; }

private:
	ID3D11VertexShader* mShader;
};

#endif // GVERTEXSHADER_H
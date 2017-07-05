#ifndef GGEOMETRYSHADER_H
#define GGEOMETRYSHADER_H

#include "D3D11.h"
#include "GShader.h"

#include "malloc.h"
#include <vector>

__declspec(align(16))
class GGeometryShader : public GShader
{
public:
	GGeometryShader();
	~GGeometryShader();

	inline ID3D11GeometryShader* GetShader() { return mShader; }
	inline void SetShader(ID3D11GeometryShader* gs) { mShader = gs; }

private:
	ID3D11GeometryShader* mShader;
};

#endif // GGEOMETRYSHADER_H
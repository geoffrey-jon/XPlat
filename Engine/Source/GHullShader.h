#ifndef GHULLSHADER_H
#define GHULLSHADER_H

#include "D3D11.h"
#include "GShader.h"

#include "malloc.h"
#include <vector>

__declspec(align(16))
class GHullShader : public GShader
{
public:
	GHullShader();
	~GHullShader();

	inline ID3D11HullShader* GetShader() { return mShader; }
	inline void SetShader(ID3D11HullShader* hs) { mShader = hs; }

private:
	ID3D11HullShader* mShader;
};

#endif // GHULLSHADER_H
#ifndef GPIXELSHADER_H
#define GPIXELSHADER_H

#include "D3D11.h"
#include "GShader.h"

#include "malloc.h"
#include <vector>

__declspec(align(16))
class GPixelShader : public GShader
{
public:
	GPixelShader();
	~GPixelShader();

	inline ID3D11PixelShader* GetShader() { return mShader; }
	inline void SetShader(ID3D11PixelShader* ps) { mShader = ps; }

private:
	ID3D11PixelShader* mShader;
};

#endif // GPIXELSHADER_H
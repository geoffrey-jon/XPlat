#ifndef GDOMAINSHADER_H
#define GDOMAINSHADER_H

#include "D3D11.h"
#include "GShader.h"

#include "malloc.h"
#include <vector>

__declspec(align(16))
class GDomainShader : public GShader
{
public:
	GDomainShader();
	~GDomainShader();

	inline ID3D11DomainShader* GetShader() { return mShader; }
	inline void SetShader(ID3D11DomainShader* ds) { mShader = ds; }

private:
	ID3D11DomainShader* mShader;
};

#endif // GDOMAINSHADER_H
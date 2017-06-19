#ifndef GSHADER_H
#define GSHADER_H

#include "D3D11.h"
#include "malloc.h"
#include <vector>

__declspec(align(16))
class GShader
{
public:
	GShader();
	~GShader();

	void* operator new(size_t i){ return _mm_malloc(i,16); }
	void operator delete(void* p) { _mm_free(p); }

	void AddConstantBuffer(ID3D11Buffer* buffer);
	void AddSampler(ID3D11SamplerState* sampler);
	void AddShaderResource(ID3D11ShaderResourceView* srv);

	ID3D11Buffer* GetConstantBufferAt(UINT i);
	ID3D11SamplerState* GetSamplerAt(UINT i);
	ID3D11ShaderResourceView* GetShaderResourceAt(UINT i);

	UINT GetNumConstantBuffers();
	UINT GetNumSamplers();
	UINT GetNumShaderResources();

	inline LPCWSTR GetFilename() { return mFilename; }
	inline LPCSTR GetEntryPoint() { return mEntryPoint; }

	void SetFilename(LPCWSTR filename);
	void SetEntryPoint(LPCSTR entryPoint);

private:
	LPCWSTR mFilename; 
	LPCSTR mEntryPoint;

	std::vector<ID3D11Buffer*> mConstantBuffers;
	std::vector<ID3D11SamplerState*> mSamplerStates;
	std::vector<ID3D11ShaderResourceView*> mShaderResources;
};

#endif // GSHADER_H
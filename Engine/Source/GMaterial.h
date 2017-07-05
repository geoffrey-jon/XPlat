#ifndef GMATERIAL_H
#define GMATERIAL_H

#include <map>

#include "D3D11.h"

#include "GVertexShader.h"

#include "GPixelShader.h"
#include "GHullShader.h"
#include "GDomainShader.h"
#include "GGeometryShader.h"
#include "GConstantBuffer.h"

#include "GObject.h"

class GObject;
class GConstantBuffer;

__declspec(align(16))
class GMaterial
{
public:
	GMaterial();
	~GMaterial();

	void* operator new(size_t i){ return _mm_malloc(i,16); }
	void operator delete(void* p) { _mm_free(p); }

	// Unique ID
	inline UINT GetID() { return mID; }

	// Shaders
	inline GVertexShader* GetVertexShader() { return mVS; }
	inline GHullShader* GetHullShader() { return mHS; }
	inline GDomainShader* GetDomainShader() { return mDS; }
	inline GGeometryShader* GetGeometryShader() { return mGS; }
	inline GPixelShader* GetPixelShader() { return mPS; }

	void SetVertexShader(LPCWSTR filename, LPCSTR entryPoint);
	void SetHullShader(LPCWSTR filename, LPCSTR entryPoint);
	void SetDomainShader(LPCWSTR filename, LPCSTR entryPoint);
	void SetGeometryShader(LPCWSTR filename, LPCSTR entryPoint);
	void SetPixelShader(LPCWSTR filename, LPCSTR entryPoint);

	// Input Layout and Description
	inline ID3D11InputLayout* GetInputLayout() { return mInputLayout; }
	const D3D11_INPUT_ELEMENT_DESC* GetInputLayoutDesc();
	inline UINT GetNumInputElements() { return mNumInputElements; }

	void SetInputLayout(const D3D11_INPUT_ELEMENT_DESC layout[], UINT numElements);
	inline void SetInputLayout(ID3D11InputLayout* layout) { mInputLayout = layout; }

	// Topology
	inline D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return mTopology; }
	inline void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { mTopology = topology; }

	// Constant Buffers
	const std::map<UINT, GConstantBuffer*>& GetConstantBuffers() const;

	void AddConstantBuffer(GConstantBuffer* buffer, UINT registerID);

	// Rasterizer State
	inline ID3D11RasterizerState* GetRasterizerState() { return mRasterizerState; }
	inline void SetRasterizerState(ID3D11RasterizerState* state) { mRasterizerState = state; }

	// Depth Stencil State
	inline ID3D11DepthStencilState* GetDepthStencilState() { return mDepthStencilState; }
	inline void SetDepthStencilState(ID3D11DepthStencilState* state) { mDepthStencilState = state; }

	inline UINT GetStencilRef() { return mStencilRef; }
	inline void SetStencilRef(UINT ref) { mStencilRef = ref; }

	// Blend State
	inline ID3D11BlendState* GetBlendState() { return mBlendState; }
	inline void SetBlendState(ID3D11BlendState* state) { mBlendState = state; }

	inline FLOAT* GetBlendFactor() { return mBlendFactor; }
	void SetBlendFactor(FLOAT factor[4]);

	inline UINT GetSampleMask() { return mSampleMask; }
	inline void SetSampleMask(UINT mask) { mSampleMask = mask; }

private:
	// Unique ID
	UINT mID;

	// Shader Objects
	GVertexShader*   mVS;
	GHullShader*     mHS;
	GDomainShader*   mDS;
	GGeometryShader* mGS;
	GPixelShader*    mPS;

	// Input Layout and Description
	ID3D11InputLayout* mInputLayout;
	const D3D11_INPUT_ELEMENT_DESC* mInputLayoutDesc;
	UINT mNumInputElements;

	D3D11_PRIMITIVE_TOPOLOGY mTopology;

	// Constant Buffers
	std::map<UINT, GConstantBuffer*> mConstantBuffers;	

	// Samplers
//	std::map<UINT, GSampler*> mSamplers;

	// State Objects
	ID3D11RasterizerState* mRasterizerState;

	ID3D11DepthStencilState* mDepthStencilState;
	UINT mStencilRef;
	
	ID3D11BlendState* mBlendState;
	FLOAT mBlendFactor[4];
	UINT mSampleMask;
};

#endif // GMATERIAL_H

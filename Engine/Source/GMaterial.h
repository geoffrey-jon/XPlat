#ifndef GMATERIAL_H
#define GMATERIAL_H

#include <map>

#include "D3D11.h"

#include "GConstantBuffer.h"
#include "GVertexShader.h"
#include "GPixelShader.h"

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

	void RegisterObject(GObject* obj);

	std::vector<GObject*> GetObjects();

	void SetVertexShader(ID3D11VertexShader* shader);
	void SetHullShader(ID3D11HullShader* shader);
	void SetDomainShader(ID3D11DomainShader* shader);
	void SetGeometryShader(ID3D11GeometryShader* shader);
	void SetPixelShader(ID3D11PixelShader* shader);

	void SetVertexShader(LPCWSTR filename, LPCSTR entryPoint);
	void SetPixelShader(LPCWSTR filename, LPCSTR entryPoint);

	inline void SetInputLayout(ID3D11InputLayout* layout) { mInputLayout = layout; }

	void SetInputLayout(const D3D11_INPUT_ELEMENT_DESC layout[], UINT numElements);

	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology);

	void SetConstantBuffer(GConstantBuffer* buffer);

	void SetRasterizerState(ID3D11RasterizerState* state);

	void SetDepthStencilState(ID3D11DepthStencilState* state);
	void SetStencilRef(UINT ref);

	void SetBlendState(ID3D11BlendState* state);
	void SetBlendFactor(FLOAT factor[4]);
	void SetSampleMask(UINT mask);

//	ID3D11VertexShader*   GetVertexShader();
	ID3D11HullShader*     GetHullShader();
	ID3D11DomainShader*   GetDomainShader();
	ID3D11GeometryShader* GetGeometryShader();
//	ID3D11PixelShader*    GetPixelShader();

	inline GVertexShader* GetVertexShader() { return mVS; }
	inline GPixelShader* GetPixelShader() { return mPS; }

	inline ID3D11InputLayout* GetInputLayout() { return mInputLayout; }
	inline D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return mTopology; }

	inline ID3D11RasterizerState* GetRasterizerState() { return mRasterizerState; }

	inline ID3D11DepthStencilState* GetDepthStencilState() { return mDepthStencilState;	}
	inline UINT GetStencilRef() { return mStencilRef; }

	inline ID3D11BlendState* GetBlendState() { return mBlendState; }
	inline FLOAT* GetBlendFactor() { return mBlendFactor; }
	inline UINT GetSampleMask() { return mSampleMask; }

	void UpdateConstantBuffers();

	void AddConstantBuffer(GConstantBuffer* buffer, UINT registerID);

	std::map<UINT, GConstantBuffer*>::iterator GetConstantBuffers();
	UINT GetNumConstantBuffers();

	inline UINT GetID() { return mID; }

	const D3D11_INPUT_ELEMENT_DESC* GetInputLayoutDesc();
	inline UINT GetNumInputElements() { return mNumInputElements; }

private:
	UINT mID;

	std::vector<GObject*> mObjects;

//	ID3D11VertexShader*   mVS;
	ID3D11HullShader*     mHS;
	ID3D11DomainShader*   mDS;
	ID3D11GeometryShader* mGS;
//	ID3D11PixelShader*    mPS;

	GVertexShader* mVS;
	GPixelShader* mPS;

	ID3D11InputLayout* mInputLayout;
	D3D11_PRIMITIVE_TOPOLOGY mTopology;

	// Constant Buffers
	std::map<UINT, GConstantBuffer*> mConstantBuffers;

	// Samplers

	ID3D11RasterizerState* mRasterizerState;

	ID3D11DepthStencilState* mDepthStencilState;
	UINT mStencilRef;
	
	ID3D11BlendState* mBlendState;
	FLOAT mBlendFactor[4];
	UINT mSampleMask;

	const D3D11_INPUT_ELEMENT_DESC* mInputLayoutDesc;
	UINT mNumInputElements;
};

#endif // GMATERIAL_H

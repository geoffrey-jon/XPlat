#include "GMaterial.h"
#include "D3DUtil.h"

GMaterial::GMaterial() : 
	mVS(0), 
	mHS(0), 
	mDS(0), 
	mGS(0), 
	mPS(0),
	mInputLayout(0),
	mRasterizerState(0),
	mDepthStencilState(0),
	mBlendState(0)
{
	// TODO: Create MaterialManager to assign new mID to each new Material
	mID = 0;

	mStencilRef = 0;
	mBlendFactor[0] = 0.0f;
	mBlendFactor[1] = 0.0f;
	mBlendFactor[2] = 0.0f;
	mBlendFactor[3] = 0.0f;
	mSampleMask = 0;
}

GMaterial::~GMaterial()
{
	ReleaseCOM(mInputLayout);
	ReleaseCOM(mRasterizerState);
	ReleaseCOM(mDepthStencilState);
	ReleaseCOM(mBlendState);
}

void GMaterial::SetVertexShader(LPCWSTR filename, LPCSTR entryPoint)
{
	if (!mVS) {	mVS = new GVertexShader(); }

	mVS->SetFilename(filename);
	mVS->SetEntryPoint(entryPoint);
}


void GMaterial::SetHullShader(LPCWSTR filename, LPCSTR entryPoint)
{
	if (!mHS) { mHS = new GHullShader(); }

	mHS->SetFilename(filename);
	mHS->SetEntryPoint(entryPoint);
}

void GMaterial::SetDomainShader(LPCWSTR filename, LPCSTR entryPoint)
{
	if (!mDS) { mDS = new GDomainShader(); }

	mDS->SetFilename(filename);
	mDS->SetEntryPoint(entryPoint);
}

void GMaterial::SetGeometryShader(LPCWSTR filename, LPCSTR entryPoint)
{
	if (!mGS) { mGS = new GGeometryShader(); }

	mGS->SetFilename(filename);
	mGS->SetEntryPoint(entryPoint);
}


void GMaterial::SetPixelShader(LPCWSTR filename, LPCSTR entryPoint)
{
	if (!mPS) {	mPS = new GPixelShader(); }

	mPS->SetFilename(filename);
	mPS->SetEntryPoint(entryPoint);
}

void GMaterial::AddConstantBuffer(GConstantBuffer* buffer, UINT registerID)
{
	mConstantBuffers.insert(std::pair<UINT, GConstantBuffer*>(registerID, buffer));
}

void GMaterial::SetInputLayout(const D3D11_INPUT_ELEMENT_DESC layout[], UINT numElements)
{
	mInputLayoutDesc = layout;
	mNumInputElements = numElements;
}

const D3D11_INPUT_ELEMENT_DESC* GMaterial::GetInputLayoutDesc()
{
	return mInputLayoutDesc;
}

const std::map<UINT, GConstantBuffer*>& GMaterial::GetConstantBuffers() const
{
	return mConstantBuffers;
}

void GMaterial::SetBlendFactor(FLOAT factor[4])
{
	mBlendFactor[0] = factor[0];
	mBlendFactor[1] = factor[1];
	mBlendFactor[2] = factor[2];
	mBlendFactor[3] = factor[3];
}
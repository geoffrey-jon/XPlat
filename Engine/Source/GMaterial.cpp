#include "GMaterial.h"
#include "D3DUtil.h"

GMaterial::GMaterial() : 
	mVS(0), mHS(0), mDS(0), mGS(0), mPS(0),
	mInputLayout(0),
	mRasterizerState(0),
	mDepthStencilState(0),
	mBlendState(0)
{
	// TODO: Create MaterialManager to assign new mID to each new Material
	mID = 0;
	mObjects.resize(0);

	mStencilRef = 0;
	mBlendFactor[0] = 0.0f;
	mBlendFactor[1] = 0.0f;
	mBlendFactor[2] = 0.0f;
	mBlendFactor[3] = 0.0f;
	mSampleMask = 0;
}

GMaterial::~GMaterial()
{
	mObjects.resize(0);

//	ReleaseCOM(mVS);
	ReleaseCOM(mHS);
	ReleaseCOM(mDS);
	ReleaseCOM(mGS);
//	ReleaseCOM(mPS);

	ReleaseCOM(mInputLayout);
	ReleaseCOM(mRasterizerState);
	ReleaseCOM(mDepthStencilState);
	ReleaseCOM(mBlendState);
}

void GMaterial::RegisterObject(GObject* obj)
{
	// TODO: Reserve space for objects to prevent resizing the vector on every push_back
	mObjects.push_back(obj);
}

std::vector<GObject*> GMaterial::GetObjects()
{
	return mObjects;
}

void GMaterial::UpdateConstantBuffers()
{
	auto it = mConstantBuffers.begin();
	for (auto it = mConstantBuffers.begin(); it != mConstantBuffers.end(); ++it)
	{
		GConstantBuffer* cb = it->second;
//		cb->Update();
	}
}

void GMaterial::AddConstantBuffer(GConstantBuffer* buffer, UINT registerID)
{
	mConstantBuffers.insert(std::pair<UINT, GConstantBuffer*>(registerID, buffer));
}

void GMaterial::SetVertexShader(LPCWSTR filename, LPCSTR entryPoint)
{
	if (!mVS)
	{
		mVS = new GVertexShader();
	}

	mVS->SetFilename(filename);
	mVS->SetEntryPoint(entryPoint);
}

void GMaterial::SetPixelShader(LPCWSTR filename, LPCSTR entryPoint)
{
	if (!mPS)
	{
		mPS = new GPixelShader();
	}

	mPS->SetFilename(filename);
	mPS->SetEntryPoint(entryPoint);
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

void GMaterial::SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	mTopology = topology;
}

std::map<UINT, GConstantBuffer*>::iterator GMaterial::GetConstantBuffers()
{
	return mConstantBuffers.begin();
}

UINT GMaterial::GetNumConstantBuffers()
{
	return mConstantBuffers.size();
}
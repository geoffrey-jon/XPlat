#include "GShader.h"
#include "D3DUtil.h"

GShader::GShader()
{
	mConstantBuffers.resize(0);
	mSamplerStates.resize(0);
	mShaderResources.resize(0);
}

GShader::~GShader()
{
	// TODO: Do I need to release the COM pointers to the objects in my vectors?
	mConstantBuffers.resize(0);
	mSamplerStates.resize(0);
	mShaderResources.resize(0);
}

void GShader::SetFilename(LPCWSTR filename)
{
	mFilename = filename;
}

void GShader::SetEntryPoint(LPCSTR entryPoint)
{
	mEntryPoint = entryPoint;
}
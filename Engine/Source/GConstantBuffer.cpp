#include "GConstantBuffer.h"
#include "D3DUtil.h"

GConstantBuffer::GConstantBuffer() :
	mBuffer(0)
{
}

GConstantBuffer::~GConstantBuffer()
{
	ReleaseCOM(mBuffer);
}

void GConstantBuffer::SetShaderStages(BYTE stages)
{
	mShaderStages = stages;
}

bool GConstantBuffer::IsBoundToVS()
{
	return ((mShaderStages & ShaderStage::VS) != 0);
}

bool GConstantBuffer::IsBoundToHS()
{
	return ((mShaderStages & ShaderStage::HS) != 0);
}

bool GConstantBuffer::IsBoundToDS()
{
	return ((mShaderStages & ShaderStage::DS) != 0);
}

bool GConstantBuffer::IsBoundToGS()
{
	return ((mShaderStages & ShaderStage::GS) != 0);
}

bool GConstantBuffer::IsBoundToPS()
{
	return ((mShaderStages & ShaderStage::PS) != 0);
}

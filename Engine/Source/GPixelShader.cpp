#include "GPixelShader.h"
#include "D3DUtil.h"

GPixelShader::GPixelShader() : GShader(), mShader(0)
{
}

GPixelShader::~GPixelShader()
{
	ReleaseCOM(mShader);
}

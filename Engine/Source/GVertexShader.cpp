#include "GVertexShader.h"
#include "D3DUtil.h"

GVertexShader::GVertexShader() : GShader(), mShader(0)
{
}

GVertexShader::~GVertexShader()
{
	ReleaseCOM(mShader);
}

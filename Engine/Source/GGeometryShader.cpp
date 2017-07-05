#include "GGeometryShader.h"
#include "D3DUtil.h"

GGeometryShader::GGeometryShader() : GShader(), mShader(0)
{
}

GGeometryShader::~GGeometryShader()
{
	ReleaseCOM(mShader);
}

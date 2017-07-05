#include "GHullShader.h"
#include "D3DUtil.h"

GHullShader::GHullShader() : GShader(), mShader(0)
{
}

GHullShader::~GHullShader()
{
	ReleaseCOM(mShader);
}

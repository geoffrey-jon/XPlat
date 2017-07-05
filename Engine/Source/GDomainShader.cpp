#include "GDomainShader.h"
#include "D3DUtil.h"

GDomainShader::GDomainShader() : GShader(), mShader(0)
{
}

GDomainShader::~GDomainShader()
{
	ReleaseCOM(mShader);
}

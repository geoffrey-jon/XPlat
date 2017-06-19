#include "GBasicCB.h"

GBasicCB::GBasicCB() : GConstantBuffer()
{
}

GBasicCB::~GBasicCB()
{
}

void GBasicCB::Init()
{
}

void GBasicCB::Update()
{
	MBuffer* cb = (MBuffer*)mResource.pData;
	cb->eyePosW = mCamera->GetPosition();
	cb->light = *mLight;
}

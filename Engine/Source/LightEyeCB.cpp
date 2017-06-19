#include "LightEyeCB.h"

LightEyeCB::LightEyeCB()
{
	mByteWidth = sizeof(CBuffer_LightEye);
}

LightEyeCB::~LightEyeCB()
{

}

void LightEyeCB::Init()
{

}

void LightEyeCB::Update(D3D11_MAPPED_SUBRESOURCE resource)
{
	mCBuffer = (CBuffer_LightEye*)resource.pData;
	mCBuffer->eyePosW = mCamera->GetPosition();
	mCBuffer->light = mLight;
}

void LightEyeCB::Update(D3D11_MAPPED_SUBRESOURCE resource, GObject* obj)
{
}

void LightEyeCB::SetDirectionalLight(DirectionalLight light)
{
	mLight = light;
}

void LightEyeCB::SetSurfaceProperties(SurfaceProperties surface)
{
	mSurface = surface;
}

void LightEyeCB::SetCamera(GFirstPersonCamera* camera)
{
	mCamera = camera;
}
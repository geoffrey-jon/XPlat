#include "ObjectTransformsCB.h"

ObjectTransformsCB::ObjectTransformsCB()
{
	mByteWidth = sizeof(CBuffer_ObjectTransforms);
}

ObjectTransformsCB::~ObjectTransformsCB()
{

}

void ObjectTransformsCB::Init()
{

}

void ObjectTransformsCB::Update(D3D11_MAPPED_SUBRESOURCE resource)
{
}

void ObjectTransformsCB::Update(D3D11_MAPPED_SUBRESOURCE resource, GObject* obj)
{
	mCBuffer = (CBuffer_ObjectTransforms*)resource.pData;

	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&obj->GetWorldTransform());

	mCBuffer->gWorld = DirectX::XMMatrixTranspose(world);
	mCBuffer->gWorldViewProj = DirectX::XMMatrixTranspose(world*mCamera->ViewProj());
	mCBuffer->gWorldInvTranspose = MathHelper::InverseTranspose(world);
	mCBuffer->gTexTransform = DirectX::XMLoadFloat4x4(&obj->GetTexTransform());
	mCBuffer->gSurface = obj->GetSurfaceProperies();
}

void ObjectTransformsCB::SetCamera(GFirstPersonCamera* camera)
{
	mCamera = camera;
}

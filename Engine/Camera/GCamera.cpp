#include "GCamera.h"

GCamera::GCamera() : 
	mPosition(0.0f, 0.0f, 0.0f),
	mRight(1.0f, 0.0f, 0.0f),
	mUp(0.0f, 1.0f, 0.0f),
	mLook(0.0f, 0.0f, 1.0f)
{
	SetLens(0.25f*MathHelper::Pi, 1.0f, 1.0f, 1000.0f);
}

GCamera::~GCamera()
{
}

DirectX::XMVECTOR GCamera::GetPosition()const
{
	return XMLoadFloat3(&mPosition);
}

DirectX::XMVECTOR GCamera::GetPositionVector()const
{
	return XMLoadFloat3(&mPosition);
}

DirectX::XMFLOAT3 GCamera::GetPositionFloat3()const
{
	return mPosition;
}

void GCamera::SetPosition(float x, float y, float z)
{
	mPosition = DirectX::XMFLOAT3(x, y, z);
}

void GCamera::SetPosition(const DirectX::XMFLOAT3& v)
{
	mPosition = v;
}

DirectX::XMVECTOR GCamera::GetRightVector()const
{
	return XMLoadFloat3(&mRight);
}

DirectX::XMFLOAT3 GCamera::GetRightFloat3()const
{
	return mRight;
}

DirectX::XMVECTOR GCamera::GetUpVector()const
{
	return XMLoadFloat3(&mUp);
}

DirectX::XMFLOAT3 GCamera::GetUpFloat3()const
{
	return mUp;
}

DirectX::XMVECTOR GCamera::GetLookVector()const
{
	return XMLoadFloat3(&mLook);
}

DirectX::XMFLOAT3 GCamera::GetLookFloat3()const
{
	return mLook;
}

float GCamera::GetNearZ()const
{
	return mNearZ;
}

float GCamera::GetFarZ()const
{
	return mFarZ;
}

float GCamera::GetAspect()const
{
	return mAspect;
}

float GCamera::GetFovY()const
{
	return mFovY;
}

float GCamera::GetFovX()const
{
	float halfWidth = 0.5f*GetNearWindowWidth();
	return 2.0f*atanf(halfWidth / mNearZ);
}

float GCamera::GetNearWindowWidth()const
{
	return mAspect * mNearWindowHeight;
}

float GCamera::GetNearWindowHeight()const
{
	return mNearWindowHeight;
}

float GCamera::GetFarWindowWidth()const
{
	return mAspect * mFarWindowHeight;
}

float GCamera::GetFarWindowHeight()const
{
	return mFarWindowHeight;
}

void GCamera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f*mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f*mFovY);

	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mProj, P);
}

void GCamera::LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp)
{
	DirectX::XMVECTOR L = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(target, pos));
	DirectX::XMVECTOR R = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(worldUp, L));
	DirectX::XMVECTOR U = DirectX::XMVector3Cross(L, R);

	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
}

void GCamera::LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
{
	DirectX::XMVECTOR P = XMLoadFloat3(&pos);
	DirectX::XMVECTOR T = XMLoadFloat3(&target);
	DirectX::XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);
}

DirectX::XMMATRIX GCamera::View()const
{
	return XMLoadFloat4x4(&mView);
}

DirectX::XMMATRIX GCamera::Proj()const
{
	return XMLoadFloat4x4(&mProj);
}

DirectX::XMMATRIX GCamera::ViewProj()const
{
	return XMMatrixMultiply(View(), Proj());
}

void GCamera::UpdateViewMatrix()
{
	DirectX::XMVECTOR R = XMLoadFloat3(&mRight);
	DirectX::XMVECTOR U = XMLoadFloat3(&mUp);
	DirectX::XMVECTOR L = XMLoadFloat3(&mLook);
	DirectX::XMVECTOR P = XMLoadFloat3(&mPosition);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = DirectX::XMVector3Normalize(L);
	U = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = DirectX::XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
	float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
	float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, L));

	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
	XMStoreFloat3(&mLook, L);

	mView(0, 0) = mRight.x;
	mView(1, 0) = mRight.y;
	mView(2, 0) = mRight.z;
	mView(3, 0) = x;

	mView(0, 1) = mUp.x;
	mView(1, 1) = mUp.y;
	mView(2, 1) = mUp.z;
	mView(3, 1) = y;

	mView(0, 2) = mLook.x;
	mView(1, 2) = mLook.y;
	mView(2, 2) = mLook.z;
	mView(3, 2) = z;

	mView(0, 3) = 0.0f;
	mView(1, 3) = 0.0f;
	mView(2, 3) = 0.0f;
	mView(3, 3) = 1.0f;
}

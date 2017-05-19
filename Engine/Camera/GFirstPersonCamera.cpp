#include "GFirstPersonCamera.h"

GFirstPersonCamera::GFirstPersonCamera() : 
	mPosition(0.0f, 0.0f, 0.0f),
	mRight(1.0f, 0.0f, 0.0f),
	mUp(0.0f, 1.0f, 0.0f),
	mLook(0.0f, 0.0f, 1.0f)
{
	SetLens(0.25f*MathHelper::Pi, 1280.0f / 720.0f, 1.0f, 1000.0f);
}

GFirstPersonCamera::~GFirstPersonCamera()
{
}

DirectX::XMVECTOR GFirstPersonCamera::GetPositionXM()const
{
	return XMLoadFloat3(&mPosition);
}

DirectX::XMFLOAT3 GFirstPersonCamera::GetPosition()const
{
	return mPosition;
}

void GFirstPersonCamera::SetPosition(float x, float y, float z)
{
	mPosition = DirectX::XMFLOAT3(x, y, z);
}

void GFirstPersonCamera::SetPosition(const DirectX::XMFLOAT3& v)
{
	mPosition = v;
}

DirectX::XMVECTOR GFirstPersonCamera::GetRightXM()const
{
	return XMLoadFloat3(&mRight);
}

DirectX::XMFLOAT3 GFirstPersonCamera::GetRight()const
{
	return mRight;
}

DirectX::XMVECTOR GFirstPersonCamera::GetUpXM()const
{
	return XMLoadFloat3(&mUp);
}

DirectX::XMFLOAT3 GFirstPersonCamera::GetUp()const
{
	return mUp;
}

DirectX::XMVECTOR GFirstPersonCamera::GetLookXM()const
{
	return XMLoadFloat3(&mLook);
}

DirectX::XMFLOAT3 GFirstPersonCamera::GetLook()const
{
	return mLook;
}

float GFirstPersonCamera::GetNearZ()const
{
	return mNearZ;
}

float GFirstPersonCamera::GetFarZ()const
{
	return mFarZ;
}

float GFirstPersonCamera::GetAspect()const
{
	return mAspect;
}

float GFirstPersonCamera::GetFovY()const
{
	return mFovY;
}

float GFirstPersonCamera::GetFovX()const
{
	float halfWidth = 0.5f*GetNearWindowWidth();
	return 2.0f*atanf(halfWidth / mNearZ);
}

float GFirstPersonCamera::GetNearWindowWidth()const
{
	return mAspect * mNearWindowHeight;
}

float GFirstPersonCamera::GetNearWindowHeight()const
{
	return mNearWindowHeight;
}

float GFirstPersonCamera::GetFarWindowWidth()const
{
	return mAspect * mFarWindowHeight;
}

float GFirstPersonCamera::GetFarWindowHeight()const
{
	return mFarWindowHeight;
}

void GFirstPersonCamera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f*mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f*mFovY);

//	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicOffCenterLH(-80.0f, 80.0f, -45.0f, 45.0f, 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void GFirstPersonCamera::LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp)
{
	DirectX::XMVECTOR L = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(target, pos));
	DirectX::XMVECTOR R = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(worldUp, L));
	DirectX::XMVECTOR U = DirectX::XMVector3Cross(L, R);

	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
}

void GFirstPersonCamera::LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
{
	DirectX::XMVECTOR P = XMLoadFloat3(&pos);
	DirectX::XMVECTOR T = XMLoadFloat3(&target);
	DirectX::XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);
}

DirectX::XMMATRIX GFirstPersonCamera::View()const
{
	return XMLoadFloat4x4(&mView);
}

DirectX::XMMATRIX GFirstPersonCamera::Proj()const
{
	return XMLoadFloat4x4(&mProj);
}

DirectX::XMMATRIX GFirstPersonCamera::ViewProj()const
{
	return XMMatrixMultiply(View(), Proj());
}

void GFirstPersonCamera::Strafe(float d)
{
	// mPosition += d*mRight
	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
	DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&mRight);
	DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, DirectX::XMVectorMultiplyAdd(s, r, p));
}

void GFirstPersonCamera::Walk(float d)
{
	// mPosition += d*mLook
	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
	DirectX::XMVECTOR l = DirectX::XMLoadFloat3(&mLook);
	DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, DirectX::XMVectorMultiplyAdd(s, l, p));
}

void GFirstPersonCamera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	DirectX::XMMATRIX R = DirectX::XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void GFirstPersonCamera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	DirectX::XMMATRIX R = DirectX::XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void GFirstPersonCamera::UpdateViewMatrix()
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

#ifndef GCAMERA_H
#define GCAMERA_H

#include "MathHelper.h"

__declspec(align(16))
class GCamera
{
public:
	GCamera();
	~GCamera();

	void* operator new(size_t i) { return _mm_malloc(i,16);	}
	void operator delete(void* p) { _mm_free(p); }

	// Get/Set world camera position.
	DirectX::XMVECTOR GetPosition() const;
	DirectX::XMVECTOR GetPositionVector() const;
	DirectX::XMFLOAT3 GetPositionFloat3() const;

	void SetPosition(float x, float y, float z);
	void SetPosition(const DirectX::XMFLOAT3& v);

	// Get camera basis vectors.
	DirectX::XMVECTOR GetRightVector()const;
	DirectX::XMFLOAT3 GetRightFloat3()const;

	DirectX::XMVECTOR GetUpVector()const;
	DirectX::XMFLOAT3 GetUpFloat3()const;

	DirectX::XMVECTOR GetLookVector()const;
	DirectX::XMFLOAT3 GetLookFloat3()const;

	// Get frustum properties.
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;
	float GetFovY()const;
	float GetFovX()const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;

	// Set frustum.
	void SetLens(float fovY, float aspect, float zn, float zf);

	// Define camera space via LookAt parameters.
	void LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
	void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

	// Get View/Proj matrices.
	DirectX::XMMATRIX View()const;
	DirectX::XMMATRIX Proj()const;
	DirectX::XMMATRIX ViewProj()const;

	// After modifying camera position/orientation, call to rebuild the view matrix.
	void UpdateViewMatrix();

protected:
	// Camera coordinate system with coordinates relative to world space.
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRight;
	DirectX::XMFLOAT3 mUp;
	DirectX::XMFLOAT3 mLook;

	// Cache frustum properties.
	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;

	// Cache View/Proj matrices.
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;
};

#endif // GCAMERA_H
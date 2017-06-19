#ifndef LIGHTEYECB_H
#define LIGHTEYECB_H

#include "GConstantBuffer.h"
#include "LightHelper.h"
#include "GFirstPersonCamera.h"
#include "malloc.h"

struct CBuffer_LightEye
{
	DirectionalLight light;
	DirectX::XMFLOAT3 eyePosW;
	float pad;
};

__declspec(align(16))
class LightEyeCB : public GConstantBuffer
{
public:
	LightEyeCB();
	~LightEyeCB();

	void Init() override;
	void Update(D3D11_MAPPED_SUBRESOURCE resource) override;
	void Update(D3D11_MAPPED_SUBRESOURCE resource, GObject* obj) override;

	void SetDirectionalLight(DirectionalLight light);
	void SetSurfaceProperties(SurfaceProperties surface);
	void SetCamera(GFirstPersonCamera* camera);

private:
	CBuffer_LightEye* mCBuffer;

	DirectionalLight mLight;
	SurfaceProperties mSurface;
	GFirstPersonCamera* mCamera;
};

#endif // LIGHTEYECB_H
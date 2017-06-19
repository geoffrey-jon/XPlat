#ifndef GBASICCB_H
#define GBASICCB_H

#include "GConstantBuffer.h"
#include "LightHelper.h"
#include "GFirstPersonCamera.h"
#include "malloc.h"

struct MBuffer
{
	DirectionalLight light;
	DirectX::XMFLOAT3 eyePosW;
};

__declspec(align(16))
class GBasicCB : public GConstantBuffer
{
public:
	GBasicCB();
	~GBasicCB();

	void Init();
	void Update();

	void* operator new(size_t i){ return _mm_malloc(i,16); }
	void operator delete(void* p) { _mm_free(p); }

	void SetDirectionalLight(DirectionalLight light);
	void SetCamera(GFirstPersonCamera* camera);

private:
	DirectionalLight* mLight;
	GFirstPersonCamera* mCamera;

	DirectX::XMFLOAT3 mEyePosW;

};

#endif // GBASICCB_H
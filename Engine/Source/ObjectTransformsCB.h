#ifndef OBJECTTRANFORMSCB_H
#define OBJECTTRANFORMSCB_H

#include "GConstantBuffer.h"
#include "GObject.h"
#include "GFirstPersonCamera.h"
#include "MathHelper.h"
#include "malloc.h"

struct CBuffer_ObjectTransforms
{
	DirectX::XMMATRIX gWorld;
	DirectX::XMMATRIX gWorldViewProj;
	DirectX::XMMATRIX gWorldInvTranspose;
	DirectX::XMMATRIX gTexTransform;
	SurfaceProperties gSurface;
};

__declspec(align(16))
class ObjectTransformsCB : public GConstantBuffer
{
public:
	ObjectTransformsCB();
	~ObjectTransformsCB();

	void Init() override;
	void Update(D3D11_MAPPED_SUBRESOURCE resource) override;
	void Update(D3D11_MAPPED_SUBRESOURCE resource, GObject* obj) override;

	void SetObject(GObject* object);
	void SetCamera(GFirstPersonCamera* camera);

private:
	CBuffer_ObjectTransforms* mCBuffer;

	GFirstPersonCamera* mCamera;
};

#endif // OBJECTTRANFORMSCB_H
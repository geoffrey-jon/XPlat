#ifndef GRENDERER_H
#define GRENDERER_H

#include "D3DApp.h"
#include "D3DUtil.h"
#include "D3DCompiler.h"
#include "GObject.h"
#include "GMaterial.h"
#include "GFirstPersonCamera.h"
#include "RenderStates.h"

#include "malloc.h"

#include <Windows.h>
#include <WindowsX.h>
#include <assert.h>
#include <string>
#include <map>

__declspec(align(16))
class GRenderer
{
public:
	GRenderer(D3DApp* app);
	~GRenderer();

	void* operator new(size_t i){ return _mm_malloc(i,16); }
	void operator delete(void* p) { _mm_free(p); }

	bool Init(HWND appWindow);

	void DrawScene();

	bool DeviceExists();
	void OnResize();

	void RegisterObject(GObject* obj);

	void CreateGeometryBuffers(GObject* obj, bool dynamic = false);

	void CreateConstantBuffers(GMaterial* material);

	void CreateVertexShader(GMaterial* material);
	void CreatePixelShader(GMaterial* material);

	void LoadTextureToSRV(ID3D11ShaderResourceView** srv, LPCWSTR filename);

	void Draw(GObject* object, const GFirstPersonCamera* camera, DirectX::XMMATRIX& transform = DirectX::XMMatrixIdentity());

private:
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mImmediateContext;

	IDXGISwapChain* mSwapChain;

	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;

	ID3D11Texture2D* mDepthStencilBuffer;

	D3D11_VIEWPORT mViewport;

	D3D_DRIVER_TYPE mD3DDriverType;

	bool mEnableMultisample;
	UINT mMultisampleQuality;

	D3DApp* mApp;

	std::multimap<UINT, GObject*> mObjects;
	std::map<UINT, GMaterial*> mMaterials;
	GFirstPersonCamera* mCamera;
};

#endif // GRENDERER_H
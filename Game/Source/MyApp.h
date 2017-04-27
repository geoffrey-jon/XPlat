#ifndef MYAPP_H
#define MYAPP_H

#include "D3DApp.h"
#include "Vertex.h"
#include "RenderStates.h"
#include "GObject.h"
#include "GFirstPersonCamera.h"

struct ConstBufferPerObject
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldViewProj;
};
	
struct ConstBufferPerFrame
{
	DirectX::XMFLOAT3 camPosW;
	float pad;
};

struct ConstBufferPSParams
{
};

class MyApp : public D3DApp
{
public:
	MyApp(HINSTANCE Instance);
	~MyApp();

	bool Init() override;
	void OnResize() override;

	void UpdateScene(float dt) override;
	void DrawScene() override;

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	void CreateGeometryBuffers(GObject* obj, bool dynamic = false);

	void CreateConstantBuffer(ID3D11Buffer** buffer, UINT size);
	
	void CreateVertexShader(ID3D11VertexShader** shader, LPCWSTR filename, LPCSTR entryPoint);
	void CreatePixelShader(ID3D11PixelShader** shader, LPCWSTR filename, LPCSTR entryPoint);

	void LoadTextureToSRV(ID3D11ShaderResourceView** srv, LPCWSTR filename);

	void InitUserInput();
	void PositionObjects();
	void SetupStaticLights();

	void DrawObject(GObject* object, const GFirstPersonCamera* camera);
	void DrawObject(GObject* object, const GFirstPersonCamera* camera, DirectX::XMMATRIX& transform);
	void DrawShadow(GObject* object, const GFirstPersonCamera* camera, DirectX::XMMATRIX& transform);
	void Draw(GObject* object, const GFirstPersonCamera* camera, DirectX::XMMATRIX& world, bool bShadow);

private:
	// Constant Buffers
	ID3D11Buffer* mConstBufferPerObject;
	ID3D11Buffer* mConstBufferPerFrame;

	D3D11_MAPPED_SUBRESOURCE cbPerObjectResource;
	D3D11_MAPPED_SUBRESOURCE cbPerFrameResource;

	ConstBufferPerObject* cbPerObject;
	ConstBufferPerFrame* cbPerFrame;

	// Shaders

	// Vertex Layout
	ID3D11InputLayout* mVertexLayout;
	// Objects

	// Lights

	// Camera
	GFirstPersonCamera mCamera;

	// User Input
};

#endif // MYAPP_H
#ifndef MYAPP_H
#define MYAPP_H

#include "D3DApp.h"
#include "Vertex.h"
#include "RenderStates.h"
#include "GObject.h"
#include "GCube.h"
#include "GFirstPersonCamera.h"
#include "GStillCamera.h"

struct ConstBufferPerFrame
{
	DirectionalLight light;
	DirectX::XMFLOAT3 eyePosW;
	float pad;
};

struct ConstBufferPerObject
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldViewProj;
	DirectX::XMMATRIX worldInvTranspose;
	DirectX::XMMATRIX texTransform;
	Material material;
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

	void Draw(GObject* object, const GFirstPersonCamera* camera, DirectX::XMMATRIX& transform = DirectX::XMMatrixIdentity());

private:
	// Constant Buffers
	ID3D11Buffer* mConstBufferPerObject;
	ID3D11Buffer* mConstBufferPerFrame;

	D3D11_MAPPED_SUBRESOURCE cbPerObjectResource;
	D3D11_MAPPED_SUBRESOURCE cbPerFrameResource;

	ConstBufferPerObject* cbPerObject;
	ConstBufferPerFrame* cbPerFrame;

	// Shaders
	ID3D11VertexShader* mBasicVertexShader;
	ID3D11PixelShader* mBasicPixelShader;

	// Vertex Layout
	ID3D11InputLayout* mVertexLayout;
	// Objects
	GCube* mWallFloor;
	GCube* mWallCeil;
	GCube* mWallRight;
	GCube* mWallLeft;

	GCube* mCharacter;

	// Lights
	DirectionalLight mLight;

	// Camera
	GFirstPersonCamera mCamera;

	// User Input
};

#endif // MYAPP_H
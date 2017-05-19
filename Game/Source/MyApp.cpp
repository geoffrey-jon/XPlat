#include "MyApp.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "D3DCompiler.h"

MyApp::MyApp(HINSTANCE Instance) :
	D3DApp(Instance),
	mVertexLayout(0),
	mConstBufferPerObject(0),
	mConstBufferPerFrame(0),
	cbPerObject(0),
	cbPerFrame(0)
{
	mWindowTitle = L"Xplat";
	mClientWidth = 1280;
	mClientHeight = 720;
}

MyApp::~MyApp()
{
	mImmediateContext->ClearState();
	RenderStates::DestroyAll();

	ReleaseCOM(mVertexLayout);
	ReleaseCOM(mConstBufferPerObject);
	ReleaseCOM(mConstBufferPerFrame);
}

bool MyApp::Init()
{
	// Initialize parent D3DApp
	if (!D3DApp::Init()) { return false; }

	// Initiailize Render States
	RenderStates::InitAll(mDevice);
	
	// Initialize Camera
	float camDepth = 45.0f / tanf(0.5f * mCamera.GetFovY());
	mCamera.SetPosition(80.0f, 45.0f, -camDepth);
	mCamera.UpdateViewMatrix();

	// Initialize User Input

	// Intialize Lights
	mLight.Ambient   = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mLight.Diffuse   = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mLight.Specular  = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mLight.Direction = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

	// Create Objects
	mWallFloor = new GCube(160.0f, 5.0f, 1.0f);
	mWallCeil  = new GCube(160.0f, 5.0f, 1.0f);
	mWallLeft  = new GCube(5.0f, 90.0f, 1.0f);
	mWallRight = new GCube(5.0f, 90.0f, 1.0f);
	mCharacter = new GCube(5.0f, 5.0f, 5.0f);

	CreateGeometryBuffers(mWallFloor, false);
	CreateGeometryBuffers(mWallCeil, false);
	CreateGeometryBuffers(mWallLeft, false);
	CreateGeometryBuffers(mWallRight, false);
	CreateGeometryBuffers(mCharacter, false);

	// Initialize Object Placement and Properties
	mWallFloor->Translate(80.0f, 2.5f, 0.0f);

	mWallFloor->SetAmbient(DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
	mWallFloor->SetDiffuse(DirectX::XMFLOAT4(0.6f, 0.6f, 0.0f, 1.0f));
	mWallFloor->SetSpecular(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	mWallFloor->SetTextureScaling(8.0f, 0.25f);
	LoadTextureToSRV(mWallFloor->GetDiffuseMapSRV(), L"Resources/Textures/brick.dds");

	mWallCeil->Translate(80.0f, 87.5f, 0.0f);
	mWallCeil->SetAmbient(DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
	mWallCeil->SetDiffuse(DirectX::XMFLOAT4(0.6f, 0.6f, 0.0f, 1.0f));
	mWallCeil->SetSpecular(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	mWallCeil->SetTextureScaling(8.0f, 0.25f);
	LoadTextureToSRV(mWallCeil->GetDiffuseMapSRV(), L"Resources/Textures/brick.dds");

	mWallLeft->Translate(2.5f, 45.0f, 0.0f);
	mWallLeft->SetAmbient(DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
	mWallLeft->SetDiffuse(DirectX::XMFLOAT4(0.6f, 0.6f, 0.0f, 1.0f));
	mWallLeft->SetSpecular(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	mWallLeft->SetTextureScaling(0.25f, 4.5f);
	LoadTextureToSRV(mWallLeft->GetDiffuseMapSRV(), L"Resources/Textures/brick.dds");

	mWallRight->Translate(157.5f, 45.0f, 0.0f);
	mWallRight->SetAmbient(DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
	mWallRight->SetDiffuse(DirectX::XMFLOAT4(0.6f, 0.6f, 0.0f, 1.0f));
	mWallRight->SetSpecular(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	mWallRight->SetTextureScaling(0.25f, 4.5f);
	LoadTextureToSRV(mWallRight->GetDiffuseMapSRV(), L"Resources/Textures/brick.dds");

	mCharacter->Translate(80.0f, 10.0f, 0.0f);
	mCharacter->SetAmbient(DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
	mCharacter->SetDiffuse(DirectX::XMFLOAT4(0.6f, 0.6f, 0.0f, 1.0f));
	mCharacter->SetSpecular(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

//	mCharacter->SetTextureScaling(0.25f, 4.5f);
	LoadTextureToSRV(mCharacter->GetDiffuseMapSRV(), L"Resources/Textures/crate.dds");

	// Compile Shaders
	CreateVertexShader(&mBasicVertexShader, L"Shaders/VertexShader.hlsl", "VS");
	CreatePixelShader(&mBasicPixelShader, L"Shaders/PixelShader.hlsl", "PS");

	// Create Constant Buffers
	CreateConstantBuffer(&mConstBufferPerFrame, sizeof(ConstBufferPerFrame));
	CreateConstantBuffer(&mConstBufferPerObject, sizeof(ConstBufferPerObject));

	return true;
}

void MyApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		SetCapture(mMainWindow);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
	}
}

void MyApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void MyApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
	}
}

void MyApp::CreateGeometryBuffers(GObject* obj, bool bDynamic)
{
	D3D11_BUFFER_DESC vbd;
	vbd.ByteWidth = sizeof(Vertex) * obj->GetVertexCount();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.MiscFlags = 0;

	if (bDynamic == true)
	{
		vbd.Usage = D3D11_USAGE_DYNAMIC;
		vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else 
	{
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.CPUAccessFlags = 0;
	}

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = obj->GetVertices();
	HR(mDevice->CreateBuffer(&vbd, &vinitData, obj->GetVertexBuffer()));

	if (obj->IsIndexed())
	{
		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * obj->GetIndexCount();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = obj->GetIndices();
		HR(mDevice->CreateBuffer(&ibd, &iinitData, obj->GetIndexBuffer()));
	}
}

void MyApp::CreateVertexShader(ID3D11VertexShader** shader, LPCWSTR filename, LPCSTR entryPoint)
{
	ID3DBlob* VSByteCode = 0;
	HR(D3DCompileFromFile(filename, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, "vs_5_0", D3DCOMPILE_DEBUG, 0, &VSByteCode, 0));

	HR(mDevice->CreateVertexShader(VSByteCode->GetBufferPointer(), VSByteCode->GetBufferSize(), NULL, shader));

	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = sizeof(vertexDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	// Create the input layout
	HR(mDevice->CreateInputLayout(vertexDesc, numElements, VSByteCode->GetBufferPointer(), VSByteCode->GetBufferSize(), &mVertexLayout));

	VSByteCode->Release();
}

void MyApp::CreatePixelShader(ID3D11PixelShader** shader, LPCWSTR filename, LPCSTR entryPoint)
{
	ID3DBlob* PSByteCode = 0;
	HR(D3DCompileFromFile(filename, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, "ps_5_0", D3DCOMPILE_DEBUG, 0, &PSByteCode, 0));

	HR(mDevice->CreatePixelShader(PSByteCode->GetBufferPointer(), PSByteCode->GetBufferSize(), NULL, shader));

	PSByteCode->Release();
}

void MyApp::LoadTextureToSRV(ID3D11ShaderResourceView** SRV, LPCWSTR filename)
{
	ID3D11Resource* texResource = nullptr;
	HR(DirectX::CreateDDSTextureFromFile(mDevice, filename, &texResource, SRV));
	ReleaseCOM(texResource); // view saves reference
}

void MyApp::CreateConstantBuffer(ID3D11Buffer** buffer, UINT size)
{
	D3D11_BUFFER_DESC desc;

	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	HR(mDevice->CreateBuffer(&desc, NULL, buffer));
}

void MyApp::Draw(GObject* object, const GFirstPersonCamera* camera, DirectX::XMMATRIX& transform)
{
	// Store convenient matrices
	DirectX::XMMATRIX world = XMLoadFloat4x4(&object->GetWorldTransform()) * transform;

	// Set per object constants
	mImmediateContext->Map(mConstBufferPerObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbPerObjectResource);
	cbPerObject = (ConstBufferPerObject*)cbPerObjectResource.pData;
	
	cbPerObject->world = DirectX::XMMatrixTranspose(world);
	cbPerObject->worldViewProj = DirectX::XMMatrixTranspose(world*mCamera.ViewProj());
	cbPerObject->worldInvTranspose = MathHelper::InverseTranspose(world);
	cbPerObject->texTransform = DirectX::XMLoadFloat4x4(&object->GetTexTransform());
	cbPerObject->material = object->GetMaterial();

	mImmediateContext->Unmap(mConstBufferPerObject, 0);

	// Set Vertex Buffer to Input Assembler Stage
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	mImmediateContext->IASetVertexBuffers(0, 1, object->GetVertexBuffer(), &stride, &offset);

	// Set Index Buffer to Input Assembler Stage if indexing is enabled for this draw
	if (object->IsIndexed()) 
	{
		mImmediateContext->IASetIndexBuffer(*object->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	}

	// Add an SRV to the shader for the object's diffuse texture if one exists
	if (object->GetDiffuseMapSRV() != nullptr) 
	{
		mImmediateContext->PSSetShaderResources(0, 1, object->GetDiffuseMapSRV());
	}

	// Draw Object, with indexing if enabled
	if (object->IsIndexed())
	{
		mImmediateContext->DrawIndexed(object->GetIndexCount(), 0, 0);
	}
	else
	{
		mImmediateContext->Draw(object->GetVertexCount(), 0);
	}
}

void MyApp::OnResize()
{
	D3DApp::OnResize();

	mCamera.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
}

void MyApp::UpdateScene(float dt)
{
}

void MyApp::DrawScene()
{
	// Clear the render target and depth/stencil views
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	mImmediateContext->IASetInputLayout(mVertexLayout);
	mImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set per frame constants.
	mImmediateContext->Map(mConstBufferPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbPerFrameResource);
	cbPerFrame = (ConstBufferPerFrame*)cbPerFrameResource.pData;
	cbPerFrame->eyePosW = mCamera.GetPosition();
	cbPerFrame->light = mLight;
	mImmediateContext->Unmap(mConstBufferPerFrame, 0);

	// Bind Constant Buffers
	mImmediateContext->VSSetConstantBuffers(1, 1, &mConstBufferPerObject);
	mImmediateContext->PSSetConstantBuffers(0, 1, &mConstBufferPerFrame);
	mImmediateContext->PSSetConstantBuffers(1, 1, &mConstBufferPerObject);

	// Set Shaders
	mImmediateContext->VSSetShader(mBasicVertexShader, NULL, 0);
	mImmediateContext->PSSetShader(mBasicPixelShader, NULL, 0);

	// Set State
	ID3D11SamplerState* samplers[] = { RenderStates::DefaultSS };
	mImmediateContext->PSSetSamplers(0, 1, samplers);

	Draw(mWallFloor, &mCamera);
	Draw(mWallCeil, &mCamera);
	Draw(mWallLeft, &mCamera);
	Draw(mWallRight, &mCamera);
	Draw(mCharacter, &mCamera);

	HR(mSwapChain->Present(0, 0));
}

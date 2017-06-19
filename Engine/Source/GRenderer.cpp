#include "GRenderer.h"

GRenderer::GRenderer(D3DApp* app) :
	mEnableMultisample(false),
	mMultisampleQuality(0),
	mDevice(0),
	mImmediateContext(0),
	mSwapChain(0),
	mDepthStencilBuffer(0),
	mRenderTargetView(0),
	mDepthStencilView(0),
	mD3DDriverType(D3D_DRIVER_TYPE_HARDWARE)
{
	mApp = app;
	ZeroMemory(&mViewport, sizeof(D3D11_VIEWPORT));
}

GRenderer::~GRenderer()
{
	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mSwapChain);
	ReleaseCOM(mDepthStencilBuffer);

	if (mImmediateContext)
	{
		mImmediateContext->ClearState();
	}

	ReleaseCOM(mImmediateContext);
	ReleaseCOM(mDevice);

	RenderStates::DestroyAll();
}

bool GRenderer::Init(HWND appWindow)
{
	/* -----------------------------------------
		Create the D3D11 Device and DeviceContext
		----------------------------------------- */

	UINT CreateDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL FeatureLevel;

	HRESULT CreateDeviceResult = D3D11CreateDevice(
		nullptr,
		mD3DDriverType,
		nullptr,
		CreateDeviceFlags,
		nullptr, 0,
		D3D11_SDK_VERSION,
		&mDevice,
		&FeatureLevel,
		&mImmediateContext
		);

	if (FAILED(CreateDeviceResult)) {
		MessageBox(0, L"D3D11CreateDevice failed.", 0, 0);
		return false;
	}

	if (FeatureLevel != D3D_FEATURE_LEVEL_11_0) {
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	/* -----------------------------------
		Check 4X MSAA Quality Level Support
		----------------------------------- */

	HR(mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &mMultisampleQuality));
	assert(mMultisampleQuality > 0);

	/* ----------------------------------------------
		Describe the characteristics of the swap chain
		---------------------------------------------- */

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;

	DXGI_MODE_DESC BackBufferDesc;
	BackBufferDesc.Width = 800;
	BackBufferDesc.Height = 600;
	// Most monitors cannot output more than 24-bit color, so extra precision would be wasted
	// Even though the monitor cannot output the 8-bit alpha, those bits can be used for other things like effects
	BackBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	BackBufferDesc.RefreshRate.Numerator = 60;
	BackBufferDesc.RefreshRate.Denominator = 1;
	BackBufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	BackBufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferDesc = BackBufferDesc;

	if (mEnableMultisample) {
		SwapChainDesc.SampleDesc.Count = 4;
		SwapChainDesc.SampleDesc.Quality = mMultisampleQuality - 1;
	}
	else {
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.SampleDesc.Quality = 0;
	}

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = appWindow;
	SwapChainDesc.Windowed = true;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = 0;

	/* ---------------------
	Create the swap chain
	--------------------- */

	IDXGIDevice* dxgiDevice = 0;
	mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	dxgiFactory->CreateSwapChain(mDevice, &SwapChainDesc, &mSwapChain);

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	ShowWindow(appWindow, SW_SHOW);
	UpdateWindow(appWindow);

	OnResize();

	// Initialize Render States
	RenderStates::InitAll(mDevice);

	return true;
}

void GRenderer::OnResize()
{
	assert(mSwapChain);
	assert(mDevice);
	assert(mImmediateContext);

	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);

	HR(mSwapChain->ResizeBuffers(1, mApp->GetClientWidth(), mApp->GetClientHeight(), DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	/* -----------------------------
	Create the render target view
	----------------------------- */

	ID3D11Texture2D* BackBuffer;

	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
	mDevice->CreateRenderTargetView(BackBuffer, nullptr, &mRenderTargetView);

	ReleaseCOM(BackBuffer);

	/* -----------------------------------------------------
	Create the depth/stencil buffer and depth/stencil view
	----------------------------------------------------- */

	D3D11_TEXTURE2D_DESC DepthStencilDesc;

	DepthStencilDesc.Width = mApp->GetClientWidth();
	DepthStencilDesc.Height = mApp->GetClientHeight();
	DepthStencilDesc.MipLevels = 1;
	DepthStencilDesc.ArraySize = 1;
	DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (mEnableMultisample) {
		DepthStencilDesc.SampleDesc.Count = 4;
		DepthStencilDesc.SampleDesc.Quality = mMultisampleQuality - 1;
	}
	else {
		DepthStencilDesc.SampleDesc.Count = 1;
		DepthStencilDesc.SampleDesc.Quality = 0;
	}

	DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencilDesc.CPUAccessFlags = 0;
	DepthStencilDesc.MiscFlags = 0;

	mDevice->CreateTexture2D(&DepthStencilDesc, nullptr, &mDepthStencilBuffer);
	mDevice->CreateDepthStencilView(mDepthStencilBuffer, nullptr, &mDepthStencilView);

	/* -----------------------------------------
	Bind the views to the output merger stage
	----------------------------------------- */

	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	/* ----------------
	Set the viewport
	---------------- */

	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = (float)mApp->GetClientWidth();
	mViewport.Height = (float)mApp->GetClientHeight();
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	mImmediateContext->RSSetViewports(1, &mViewport);
}

bool GRenderer::DeviceExists()
{
	return (mDevice != 0);
}

void GRenderer::CreateGeometryBuffers(GObject* obj, bool bDynamic)
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
/*
void GRenderer::CreateVertexShader(ID3D11VertexShader** shader, LPCWSTR filename, LPCSTR entryPoint, D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc)
{
	ID3DBlob* VSByteCode = 0;
	HR(D3DCompileFromFile(filename, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, "vs_5_0", D3DCOMPILE_DEBUG, 0, &VSByteCode, 0));

	HR(mDevice->CreateVertexShader(VSByteCode->GetBufferPointer(), VSByteCode->GetBufferSize(), NULL, shader));

	UINT numElements = sizeof(inputLayoutDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	// Create the input layout
	HR(mDevice->CreateInputLayout(inputLayoutDesc, numElements, VSByteCode->GetBufferPointer(), VSByteCode->GetBufferSize(), &mVertexLayout));

	VSByteCode->Release();
}
*/

void GRenderer::CreateVertexShader(GMaterial* material)
{
	GVertexShader* gvs = material->GetVertexShader();
	ID3D11VertexShader* vs = 0;

	ID3DBlob* VSByteCode = 0;
	HR(D3DCompileFromFile(gvs->GetFilename(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, gvs->GetEntryPoint(), "vs_5_0", D3DCOMPILE_DEBUG, 0, &VSByteCode, 0));

	HR(mDevice->CreateVertexShader(VSByteCode->GetBufferPointer(), VSByteCode->GetBufferSize(), NULL, &vs));

	gvs->SetShader(vs);

	const D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc = material->GetInputLayoutDesc();
	UINT numElements = material->GetNumInputElements();

	// Create the input layout
	ID3D11InputLayout* inputLayout = 0;
	HR(mDevice->CreateInputLayout(inputLayoutDesc, numElements, VSByteCode->GetBufferPointer(), VSByteCode->GetBufferSize(), &inputLayout));

	material->SetInputLayout(inputLayout);

	VSByteCode->Release();
}
/*
void GRenderer::CreatePixelShader(ID3D11PixelShader** shader, LPCWSTR filename, LPCSTR entryPoint)
{
	ID3DBlob* PSByteCode = 0;
	HR(D3DCompileFromFile(filename, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, "ps_5_0", D3DCOMPILE_DEBUG, 0, &PSByteCode, 0));

	HR(mDevice->CreatePixelShader(PSByteCode->GetBufferPointer(), PSByteCode->GetBufferSize(), NULL, shader));

	PSByteCode->Release();
}
*/

void GRenderer::CreatePixelShader(GMaterial* material)
{
	GPixelShader* gps = material->GetPixelShader();
	ID3D11PixelShader* ps = 0;

	ID3DBlob* PSByteCode = 0;
	HR(D3DCompileFromFile(gps->GetFilename(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, gps->GetEntryPoint(), "ps_5_0", D3DCOMPILE_DEBUG, 0, &PSByteCode, 0));

	HR(mDevice->CreatePixelShader(PSByteCode->GetBufferPointer(), PSByteCode->GetBufferSize(), NULL, &ps));

	gps->SetShader(ps);

	PSByteCode->Release();
}

void GRenderer::LoadTextureToSRV(ID3D11ShaderResourceView** SRV, LPCWSTR filename)
{
	ID3D11Resource* texResource = nullptr;
	HR(DirectX::CreateDDSTextureFromFile(mDevice, filename, &texResource, SRV));
	ReleaseCOM(texResource); // view saves reference
}

ID3D11Buffer* GRenderer::CreateConstantBuffer(UINT size)
{
	ID3D11Buffer* buffer;
	D3D11_BUFFER_DESC desc;

	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	HR(mDevice->CreateBuffer(&desc, NULL, &buffer));
	return buffer;
}

void GRenderer::CreateConstantBuffers(GMaterial* material)
{
	// TODO: Clean this mess up. What is the right way to GetConstantBuffers
	auto cBuffers = material->GetConstantBuffers();
	UINT numBuffers = material->GetNumConstantBuffers();
	for (UINT i = 0; i < numBuffers; ++i)
	{
		GConstantBuffer* cb = cBuffers->second;

		D3D11_BUFFER_DESC desc;

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = cb->GetByteWidth();
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		ID3D11Buffer* buffer = 0;
		HR(mDevice->CreateBuffer(&desc, NULL, &buffer));
		cb->SetBuffer(buffer);

		cBuffers++;
	}
}

void GRenderer::Draw(GObject* object, const GFirstPersonCamera* camera, DirectX::XMMATRIX& transform)
{
	// Store convenient matrices
	DirectX::XMMATRIX world = XMLoadFloat4x4(&object->GetWorldTransform()) * transform;

	// TODO: Update all constant buffers with per object fields
	GMaterial* material = object->GetMaterial();

	auto cBuffers = material->GetConstantBuffers();
	UINT numBuffers = material->GetNumConstantBuffers();
	for (UINT i = 0; i < numBuffers; ++i)
	{
		GConstantBuffer* cb = cBuffers->second;

		if (cb->GetUpdateFrequency() == PER_OBJECT)
		{
			D3D11_MAPPED_SUBRESOURCE resource;
			mImmediateContext->Map(cb->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
			cb->Update(resource, object);
			mImmediateContext->Unmap(cb->GetBuffer(), 0);
		}

		cBuffers++;
	}

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

void GRenderer::RegisterObject(GObject* obj)
{
	GMaterial* material = obj->GetMaterial();

	// Object Operations

	CreateGeometryBuffers(obj, false);
	
	// TODO: Move Ownership of DiffuseMap to Material
	// TODO: Figure out the line between Material and MaterialInstance
	LoadTextureToSRV(obj->GetDiffuseMapSRV(), obj->GetDiffuseMap());

	UINT materialID = material->GetID();
	mObjects.insert(std::pair<UINT, GObject*>(materialID, obj));

	// Material Operations. Do Only if Material is new
	if (mMaterials.find(materialID) == mMaterials.end())
	{
		CreateVertexShader(material);
		CreatePixelShader(material);
		CreateConstantBuffers(material);

		mMaterials.insert(std::pair<UINT, GMaterial*>(materialID, material));
	}
}

void GRenderer::DrawScene()
{
	// Clear the render target and depth/stencil views
	// TODO: Set up RenderPasses that manage their own rtv and dsv.
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (auto it = mMaterials.begin(); it != mMaterials.end(); ++it)
	{
		GMaterial* material = it->second;
		mImmediateContext->IASetInputLayout(material->GetInputLayout());
		mImmediateContext->IASetPrimitiveTopology(material->GetTopology());

		// TODO: Clean up access to cBuffers
		auto cBuffers = material->GetConstantBuffers();
		UINT numBuffers = material->GetNumConstantBuffers();
		for (UINT i = 0; i < numBuffers; ++i)
		{
			GConstantBuffer* cb = cBuffers->second;

			if (cb->GetUpdateFrequency() == PER_FRAME)
			{
				D3D11_MAPPED_SUBRESOURCE resource;
				mImmediateContext->Map(cb->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
				cb->Update(resource);
				mImmediateContext->Unmap(cb->GetBuffer(), 0);
			}

			ID3D11Buffer* buffer = cb->GetBuffer();
			// TODO: Why am I using a map? Why not just give ConstantBuffers a registerID member variable?
			UINT registerID = cBuffers->first;
			if (cb->IsBoundToVS())
			{
				mImmediateContext->VSSetConstantBuffers(registerID, 1, &buffer);
			}
			if (cb->IsBoundToPS())
			{
				mImmediateContext->PSSetConstantBuffers(registerID, 1, &buffer);
			}

			cBuffers++;
		}

		// VERTEX SHADER
		GVertexShader* vs = material->GetVertexShader();
/*
		for (UINT i = 0; i < vs->GetNumSamplers(); ++i)
		{
			ID3D11SamplerState* ss = vs->GetSamplerAt(i);
			mImmediateContext->VSSetSamplers(i, 1, &ss);
		}

		for (UINT i = 0; i < vs->GetNumShaderResources(); ++i)
		{
			ID3D11ShaderResourceView* srv = vs->GetShaderResourceAt(i);
			mImmediateContext->VSSetShaderResources(i, 1, &srv);
		}
*/
		mImmediateContext->VSSetShader(vs->GetShader(), NULL, 0);


		// PIXEL SHADER
		GPixelShader* ps = material->GetPixelShader();
/*
		for (UINT i = 0; i < ps->GetNumSamplers(); ++i)
		{
			ID3D11SamplerState* ss = ps->GetSamplerAt(i);
			mImmediateContext->PSSetSamplers(i, 1, &ss);
		}

		for (UINT i = 0; i < ps->GetNumShaderResources(); ++i)
		{
			ID3D11ShaderResourceView* srv = ps->GetShaderResourceAt(i);
			mImmediateContext->PSSetShaderResources(i, 1, &srv);
		}
*/
		mImmediateContext->PSSetShader(ps->GetShader(), NULL, 0);


//		mImmediateContext->HSSetShader(material->GetHullShader(), NULL, 0);
//		mImmediateContext->DSSetShader(material->GetDomainShader(), NULL, 0);
//		mImmediateContext->GSSetShader(material->GetGeometryShader(), NULL, 0);

//		mImmediateContext->RSSetState(material->GetRasterizerState());
//		mImmediateContext->OMSetDepthStencilState(material->GetDepthStencilState(), material->GetStencilRef());
//		mImmediateContext->OMSetBlendState(material->GetBlendState(), material->GetBlendFactor(), material->GetSampleMask());

		// TODO Samplers
		ID3D11SamplerState* samplers[] = { RenderStates::DefaultSS };
		mImmediateContext->PSSetSamplers(0, 1, samplers);

//		for (GObject* object : material->GetObjects())
		auto objects = mObjects.equal_range(material->GetID());
		for (auto it = objects.first; it != objects.second; ++it)
		{
			// Draw Object
			Draw(it->second, mCamera);
		}
	}

	HR(mSwapChain->Present(0, 0));
}

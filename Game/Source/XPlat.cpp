#include "XPlat.h"

XPlat::XPlat(HINSTANCE Instance) :
	D3DApp(Instance)
{
	mWindowTitle = L"Xplat";
	mClientWidth = 1280;
	mClientHeight = 720;

	mRenderer = new GRenderer(this);
	mCamera = new GFirstPersonCamera();
}

XPlat::~XPlat()
{
}

bool XPlat::Init()
{
	if (!D3DApp::Init())
	{
		return false;
	}

	mRenderer->Init(mMainWindow);

	// Initialize Camera
	float camDepth = 45.0f / tanf(0.5f * mCamera->GetFovY());
	mCamera->SetPosition(80.0f, 45.0f, -camDepth);
	mCamera->UpdateViewMatrix();

	// Initialize Lights
	mLight.Ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mLight.Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mLight.Specular = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mLight.Direction = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

	// Create Objects
	mWallFloor = new GCube(160.0f, 5.0f, 1.0f);
	mWallCeil = new GCube(160.0f, 5.0f, 1.0f);
	mWallLeft = new GCube(5.0f, 90.0f, 1.0f);
	mWallRight = new GCube(5.0f, 90.0f, 1.0f);
	mCharacter = new GCube(5.0f, 5.0f, 5.0f);
	mEnemy = new GSphere();

	// Initialize Object Placement and Properties
	mWallFloor->Translate(80.0f, 2.5f, 0.0f);

	mWallFloor->SetAmbient(DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
	mWallFloor->SetDiffuse(DirectX::XMFLOAT4(0.6f, 0.6f, 0.0f, 1.0f));
	mWallFloor->SetSpecular(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	mWallFloor->SetTextureScaling(8.0f, 0.25f);

	// TODO: Should this belong to an "Asset Loader"?
	mWallFloor->SetDiffuseMap(L"Resources/Textures/brick.dds");

	mWallCeil->Translate(80.0f, 87.5f, 0.0f);
	mWallCeil->SetAmbient(DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
	mWallCeil->SetDiffuse(DirectX::XMFLOAT4(0.6f, 0.6f, 0.0f, 1.0f));
	mWallCeil->SetSpecular(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	mWallCeil->SetTextureScaling(8.0f, 0.25f);
	mWallCeil->SetDiffuseMap(L"Resources/Textures/brick.dds");

	mWallLeft->Translate(2.5f, 45.0f, 0.0f);
	mWallLeft->SetAmbient(DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
	mWallLeft->SetDiffuse(DirectX::XMFLOAT4(0.6f, 0.6f, 0.0f, 1.0f));
	mWallLeft->SetSpecular(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	mWallLeft->SetTextureScaling(0.25f, 4.5f);
	mWallLeft->SetDiffuseMap(L"Resources/Textures/brick.dds");

	mWallRight->Translate(157.5f, 45.0f, 0.0f);
	mWallRight->SetAmbient(DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
	mWallRight->SetDiffuse(DirectX::XMFLOAT4(0.6f, 0.6f, 0.0f, 1.0f));
	mWallRight->SetSpecular(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	mWallRight->SetTextureScaling(0.25f, 4.5f);
	mWallRight->SetDiffuseMap(L"Resources/Textures/brick.dds");

	mCharacter->Translate(80.0f, 10.0f, 0.0f);
	mCharacter->SetAmbient(DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
	mCharacter->SetDiffuse(DirectX::XMFLOAT4(0.6f, 0.6f, 0.0f, 1.0f));
	mCharacter->SetSpecular(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	mCharacter->SetDiffuseMap(L"Resources/Textures/crate.dds");

	mEnemy->Translate(120.0f, 50.0f, 0.0f);
	mEnemy->SetAmbient(DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
	mEnemy->SetDiffuse(DirectX::XMFLOAT4(0.6f, 0.6f, 0.0f, 1.0f));
	mEnemy->SetSpecular(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	mEnemy->SetDiffuseMap(L"Resources/Textures/ice.dds");

	// Create a new Material for my objects to use
	GMaterial* material = new GMaterial();
	material->SetVertexShader(L"Shaders/VertexShader.hlsl", "VS");
	material->SetPixelShader(L"Shaders/PixelShader.hlsl", "PS");
	material->SetInputLayout(InputLayouts::BasicVertexLayout, 3);
	material->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// TODO: Make CB for Per Object constants

	LightEyeCB* lightEyeCB = new LightEyeCB();
	lightEyeCB->SetDirectionalLight(mLight);
	lightEyeCB->SetCamera(mCamera);

	lightEyeCB->SetShaderStages(ShaderStage::PS);
	lightEyeCB->SetUpdateFrequency(PER_FRAME);
	material->AddConstantBuffer(lightEyeCB, 0);

	ObjectTransformsCB* objTransCB = new ObjectTransformsCB();
	objTransCB->SetCamera(mCamera);

	objTransCB->SetShaderStages(ShaderStage::VS | ShaderStage::PS);
	objTransCB->SetUpdateFrequency(PER_OBJECT);
	material->AddConstantBuffer(objTransCB, 1);

	// Assign my new Material to my objects
	mWallCeil->SetMaterial(material);
	mWallFloor->SetMaterial(material);
	mWallLeft->SetMaterial(material);
	mWallRight->SetMaterial(material);
	mCharacter->SetMaterial(material);
	mEnemy->SetMaterial(material);

	mRenderer->RegisterObject(mWallFloor);
	mRenderer->RegisterObject(mWallCeil);
	mRenderer->RegisterObject(mWallLeft);
	mRenderer->RegisterObject(mWallRight);
	mRenderer->RegisterObject(mCharacter);
	mRenderer->RegisterObject(mEnemy);

	return true;
}

void XPlat::OnResize()
{
	mRenderer->OnResize();

	mCamera->SetLens(0.25f*MathHelper::Pi, GetAspectRatio(), 1.0f, 1000.0f);
}

void XPlat::UpdateScene(float dt)
{

}

void XPlat::DrawScene()
{
	mRenderer->DrawScene();
}
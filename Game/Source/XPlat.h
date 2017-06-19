#ifndef XPLAT_H
#define XPLAT_H

#include "D3DApp.h"
#include "GCube.h"
#include "GSphere.h"
#include "GFirstPersonCamera.h"
#include "GMaterial.h"
#include "GRenderer.h"

#include "LightEyeCB.h"
#include "ObjectTransformsCB.h"

#include "malloc.h"

class XPlat : public D3DApp
{
public:
	XPlat(HINSTANCE Instance);
	~XPlat();

	bool Init();
	void UpdateScene(float dt) override;
	void DrawScene();
	void OnResize();

private:
	GRenderer* mRenderer;

	// Objects
	GCube* mWallFloor;
	GCube* mWallCeil;
	GCube* mWallRight;
	GCube* mWallLeft;

	GCube* mCharacter;
	GSphere* mEnemy;

	// Lights
	DirectionalLight mLight;

	// Camera
	GFirstPersonCamera* mCamera;
};

#endif // XPLAT_H
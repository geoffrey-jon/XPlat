/*  =============================================
	Summary: Direct3D Application Framework Class
	=============================================  */

#ifndef D3DAPP_H
#define D3DAPP_H

#include "D3D11.h"
#include "D3DUtil.h"
#include "GameTimer.h"
#include "Windowsx.h"
#include <sstream>

class D3DApp
{
public:
	D3DApp(HINSTANCE Instance);
	virtual ~D3DApp();

	bool Init();
	int Run();

	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	UINT GetClientWidth() const;
	UINT GetClientHeight() const;
	float GetAspectRatio() const;

	virtual void UpdateScene(float dt) = 0;
	virtual void DrawScene() = 0;
	virtual void OnResize() = 0;

	// TODO: Move to PlayerInputSystem 
	virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y) { }

	virtual void OnKeyDown(WPARAM key, LPARAM info) { }

protected:
	bool InitMainWindow();
	void CalculateFrameStats();

protected:
	HINSTANCE mAppInstance;
	HWND mMainWindow;

	std::wstring mWindowTitle;

	GameTimer mTimer;

	bool mAppPaused;
	bool mMinimized;
	bool mMaximized;

	int mClientWidth;
	int mClientHeight;
};

#endif // D3DAPP_H
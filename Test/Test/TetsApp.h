#ifndef TESTAPP_H
#define TESTAPP_H

#include "GLApp.h"

class TestApp:
	public GLApp
{
public:
	TestApp(HINSTANCE hInstance);
	~TestApp();

	//OVERRIDES
	bool Init() override;
	void Update() override;
	void Render() override;
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

#endif
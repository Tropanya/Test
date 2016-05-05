#include "TetsApp.h"

TestApp::TestApp(HINSTANCE hInstance):
	GLApp(hInstance)
{
}

TestApp::~TestApp()
{
}

bool TestApp::Init()
{
	if(!GLApp::Init())
		return false;

	return true;
}

void TestApp::Update()
{
	//Updater::MainGameUpdate();
}

void TestApp::Render()
{
	glClearColor(0.0f,0.0f,1.0f,1.0f);
	// Renderer::MainGameRender();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

LRESULT TestApp::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	default:
		return GLApp::MsgProc(hWnd,msg,wParam,lParam);
		break;
	}
}
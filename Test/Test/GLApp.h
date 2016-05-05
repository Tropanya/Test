#ifndef GLAPP_H
#define GLAPP_H

#include "GLUtil.h"

class GLApp
{
public:
	GLApp();
	GLApp(HINSTANCE hInstance);
	virtual ~GLApp();

	//MAIN APPLICATION LOOP
	int Run();

	//FRAMEWORK METHODS
	virtual bool Init();
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParar, LPARAM lParam);
protected:
	HWND m_hAppWnd;
	HINSTANCE m_hAppInstance;
	HDC m_hDevContext;
	HGLRC m_hGLRenderContext;
	UINT m_ClientWidth;
	UINT m_ClientHeight;
	DWORD m_WindowStyle;
	float m_FPS;
	char* m_AppTitle;
protected:
	//INITIALIZE WINDOW
	bool InitWindow();

	//INITIALIZE OPENGL
	bool InitGL();

	//CALCULATE FRAMES PER SECOND
	void CalculateFPS(float dt);

	//SHUTDOWN OPENGL
	void Shutdown();
};

#endif
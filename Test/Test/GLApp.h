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
	virtual void Update() = 0;
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
	char* m_AppTitle;
protected:
	//INITIALIZE WINDOW
	bool InitWindow();

	//INITIALIZE OPENGL
	bool InitGL();

	//SHUTDOWN OPENGL
	void Shutdown();
private:
	double updateRate;
	double updateInterval;

	unsigned long idleTime;
};

#endif
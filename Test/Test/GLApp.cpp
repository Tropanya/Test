#include "GLApp.h"
#include "Macros.h"
#include "Timer.h"

namespace
{
	//GLOBAL APP
	GLApp* g_pApp = NULL;
}

//PUMPS MESSAGE THROUGH USER DEFINED MSG PROC FROM GLOBAL SPACE
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(g_pApp)
		return g_pApp->MsgProc(hWnd,msg,wParam,lParam);
	else
	{
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
}

GLApp::GLApp()
{
}

GLApp::GLApp(HINSTANCE hInstance)
{
	updateRate = 60.0f;
	updateInterval = SECOND / updateRate;
	idleTime = 1;

	m_hAppInstance = hInstance;
	m_hAppWnd = NULL;
	m_hDevContext = NULL;
	m_hGLRenderContext = NULL;
	m_ClientWidth = WINDOW_WIDTH;
	m_ClientHeight = WINDOW_HEIGHT;
	m_AppTitle = "OPENGL APPLICATION";
	m_WindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	g_pApp = this;
}

GLApp::~GLApp()
{
}

//MAIN APPLICATION LOOP
int GLApp::Run()
{

	//CALCULATE TIMING
	__int64 lastTime = 0;
	lastTime = GetTickCount();

	int fps = 0;
	int upd = 0;
	int updl = 0;

	int count = 0;

	double delta = 0.0;

	//MIAN MESSAGE LOOP
	MSG msg = {0};

	while (WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//CALCULATE DELTA TIME
			__int64 nowTime = GetTickCount();
			__int64 elapsedTime = nowTime - lastTime;

			count += (int)elapsedTime;

			bool render = false;

			delta += (elapsedTime / updateInterval);

			while(delta > 1.0f)
			{
				//UPDATE
				Update();

				upd++;
				delta--;

				if(render)
				{
					updl++;
				}
				else
				{
					render = true;
				}
			}

			if(render)
			{
				//RENDER
				Render();

				fps++;
			}
			else
			{
				Sleep(idleTime);
			}
			
			while(count >= SECOND)
			{
				//OUTPUT TO WINDOW TITLE
				std::stringstream ss;

				ss << m_AppTitle << " FPS: " << fps << " UPD: " << upd << " UPDL: " << updl << "  " <<
					glGetString(GL_VENDOR) << " " << glGetString(GL_RENDERER) << " " << glGetString(GL_VERSION);
				SetWindowText(m_hAppWnd,ss.str().c_str());

				fps = 0;
				upd = 0;
				updl = 0;
				count = 0;
			}

			//SWAP THE BUFFERS
			SwapBuffers(m_hDevContext);

			//RESET PREV TIME FOR NEXT FRAME
			lastTime = nowTime;
		}
	}

	Shutdown();

	return static_cast<int>(msg.wParam);
}

bool GLApp::InitWindow()
{
	//WNDCLASSEX
	WNDCLASSEX wcex;
	ZeroMemory(&wcex,sizeof(WNDCLASSEX));

	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = m_hAppInstance;
	wcex.lpfnWndProc = MainWndProc;
	wcex.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszClassName = "GLAPPWNDCLASS";
	wcex.lpszMenuName = NULL; //NO HMENU DEFINED
	wcex.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

	if(!RegisterClassEx(&wcex))
		return OutErrorMsg("FAILED TO REGISTER WNDCLASSEX");

	//ADJUST WINDOW RECT FOR REQUEST CLIEN SIZE
	RECT rect = {0,0,m_ClientWidth,m_ClientHeight};
	AdjustWindowRect(&rect,m_WindowStyle,FALSE);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	int x = GetSystemMetrics(SM_CXSCREEN)/2 - width/2;
	int y = GetSystemMetrics(SM_CYSCREEN)/2 - height/2;

	//CREATE OUR WINDOW
	m_hAppWnd = CreateWindow("GLAPPWNDCLASS",m_AppTitle,m_WindowStyle,x,y,width,height,NULL,NULL,m_hAppInstance,NULL);

	if(!m_hAppWnd)
		return OutErrorMsg("FAILED TO CREATE WINDOW");

	//SHOW WINDOW
	ShowWindow(m_hAppWnd,SW_SHOW);

	return true;
}

bool GLApp::InitGL()
{
	//CREATE OUR DEVICE CONTEXT
	m_hDevContext = GetDC(m_hAppWnd);

	//CREATE A PIXELFORMATDESCRIPTOR
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int format = ChoosePixelFormat(m_hDevContext,&pfd);

	if(!SetPixelFormat(m_hDevContext,format,&pfd))
		return OutErrorMsg("FAILED TO SET PIXEL FORMAT");

	//CREATE OUR RENDER CONTEXT
	m_hGLRenderContext = wglCreateContext(m_hDevContext);

	if(!wglMakeCurrent(m_hDevContext,m_hGLRenderContext))
		return OutErrorMsg("FAILED TO CREATE AND ACTIVATE RENDER CONTEXT");

	//INITIALIZE GLEW
	if(glewInit())
		return OutErrorMsg("FAILED TO INITIALIZE GLEW");

	return true;
}

bool GLApp::Init()
{
	if(!InitWindow())
		return false;
	if(!InitGL())
		return false;

	return true;
}

LRESULT GLApp::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
}

void GLApp::Shutdown()
{
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(m_hGLRenderContext);
	ReleaseDC(m_hAppWnd,m_hDevContext);
}
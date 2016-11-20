#ifdef WIN32

#ifndef UNICODE
#define UNICODE
#endif

#include "DCLogger.h"
#include "DCPlatform.h"
#include "DCConfiguration.h"
#include "DCConfigurationFileGrammar.h"
#include <Windows.h>
#include <string>
#include <iostream>

#include <atlstr.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib, "glew32.lib")

namespace DevilsCanvas 
{
	static bool tempWindowFlag = false;

	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	DCPlatform::DCPlatform() 
	{
		Init();

		m_configuration = NULL;
	}

	DCPlatform::~DCPlatform() 
	{
		ChangeDisplaySettings(NULL, 0);		//Restore the resolution.

		if (m_configuration)
			delete m_configuration;

		if (m_windowHandle.m_hGLRC) 
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(m_windowHandle.m_hGLRC);
		}
		m_windowHandle.m_hGLRC = NULL;

		if (m_windowHandle.m_hWnd)
			DestroyWindow(m_windowHandle.m_hWnd);
		m_windowHandle.m_hWnd = NULL;

		m_configuration = NULL;
	}

	void DCPlatform::Init()
	{
		//Initialize window parameters
		m_windowParameters.m_windowName = L"Devil's Canvas";
		m_windowParameters.m_startPositionX = 200;
		m_windowParameters.m_startPositionY = 200;
		m_windowParameters.m_width = 500;
		m_windowParameters.m_height = 500;
		m_windowParameters.m_fullScreen = true;
		m_windowParameters.m_resizable = false;
		m_windowParameters.m_minimize = false;
		m_windowParameters.m_hIcon = NULL;	
		
		//Initialize graphics parameters
		m_graphicsParameters.m_glMajorVersion = 3;
		m_graphicsParameters.m_glMinorVersion = 1;
		m_graphicsParameters.m_vSync = true;
	}

	void DCPlatform::GetDesktopResolution(int& oWidth, int &oHeight) 
	{
		RECT desktop;
		const HWND hDesktop = ::GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		oWidth = desktop.right;
		oHeight = desktop.bottom;
	}

	void DCPlatform::FillWindowConfiguration() 
	{
		std::string value;

		for (int type = WinParamType::eStartPositionX; type < WinParamType::eEnd; type++) 
		{
			switch (type) 
			{
			case WinParamType::eStartPositionX :
				value = m_configuration->GetValue(DCConfigurationFileGrammar::s_typeValWindow, DCConfigurationFileGrammar::s_windowParamPosX);
				if (value != "") 
				{
					m_windowParameters.m_startPositionX = std::stoi(value);
					value = "";
				}
				break;
				
			case WinParamType::eStartPositionY:
				value = m_configuration->GetValue(DCConfigurationFileGrammar::s_typeValWindow, DCConfigurationFileGrammar::s_windowParamPosY);
				if (value != "")
				{
					m_windowParameters.m_startPositionY = std::stoi(value);
					value = "";
				}
				break;
								
			case WinParamType::eWidth:
				value = m_configuration->GetValue(DCConfigurationFileGrammar::s_typeValWindow, DCConfigurationFileGrammar::s_windowParamWidth);
				if (value != "")
				{
					m_windowParameters.m_width = std::stoi(value);
					value = "";
				}
				break;

			case WinParamType::eHeight:
				value = m_configuration->GetValue(DCConfigurationFileGrammar::s_typeValWindow, DCConfigurationFileGrammar::s_windowParamHeight);
				if (value != "")
				{
					m_windowParameters.m_height = std::stoi(value);
					value = "";
				}
				break;

			case WinParamType::eIcon:
				value = m_configuration->GetValue(DCConfigurationFileGrammar::s_typeValWindow, DCConfigurationFileGrammar::s_windowParamIcon);
				if (value != "")
				{
					/* Icon work should be done */
					value = "";
				}
				break;

			case WinParamType::eFullScreen:
				value = m_configuration->GetValue(DCConfigurationFileGrammar::s_typeValWindow, DCConfigurationFileGrammar::s_windowParamFullScreen);
				if (value != "")
				{
					if(DCConfigurationFileGrammar::s_ConfigValTrue == value)
						m_windowParameters.m_fullScreen = true;
					
					else if (DCConfigurationFileGrammar::s_ConfigValFalse == value)
						m_windowParameters.m_fullScreen = false;

					value = "";
				}
				break;

			case WinParamType::eResizable:
				value = m_configuration->GetValue(DCConfigurationFileGrammar::s_typeValWindow, DCConfigurationFileGrammar::s_windowParamResizable);
				if (value != "")
				{
					if (DCConfigurationFileGrammar::s_ConfigValTrue == value)
						m_windowParameters.m_resizable = true;

					else if (DCConfigurationFileGrammar::s_ConfigValFalse == value)
						m_windowParameters.m_resizable = false;

					value = "";
				}
				break;

			case WinParamType::eMinimize:
				value = m_configuration->GetValue(DCConfigurationFileGrammar::s_typeValWindow, DCConfigurationFileGrammar::s_windowParamMinimize);
				if (value != "")
				{
					if (DCConfigurationFileGrammar::s_ConfigValTrue == value)
						m_windowParameters.m_minimize = true;

					else if (DCConfigurationFileGrammar::s_ConfigValFalse == value)
						m_windowParameters.m_minimize = false;

					value = "";
				}
				break;
			}
		}

		for (int type = GraphicsParamType::eGLMajorVersion; type < GraphicsParamType::eEnd; type++)
		{
			switch (type)
			{
			case GraphicsParamType::eGLMajorVersion:
				value = m_configuration->GetValue(DCConfigurationFileGrammar::s_typeValGraphics, DCConfigurationFileGrammar::s_glMajorVersion);
				if (value != "")
				{
					m_graphicsParameters.m_glMajorVersion = std::stoi(value);
					value = "";
				}
				break;

			case GraphicsParamType::eGLMinorVersion:
				value = m_configuration->GetValue(DCConfigurationFileGrammar::s_typeValGraphics, DCConfigurationFileGrammar::s_glMinorVersion);
				if (value != "")
				{
					m_graphicsParameters.m_glMinorVersion = std::stoi(value);
					value = "";
				}
				break;

			case GraphicsParamType::eVSync:
				value = m_configuration->GetValue(DCConfigurationFileGrammar::s_typeValGraphics, DCConfigurationFileGrammar::s_vSync);
				if (value != "")
				{
					if (DCConfigurationFileGrammar::s_ConfigValTrue == value)
						m_graphicsParameters.m_vSync = true;

					else if (DCConfigurationFileGrammar::s_ConfigValFalse == value)
						m_graphicsParameters.m_vSync = false;

					value = "";
				}
				break;
			}
		}
	}
	
	ReturnStatus DCPlatform::InitializeGL() 
	{
		ReturnStatus rs = ReturnStatus::eOK;

		rs = CreateWindowForPlatform();
		if (ReturnStatus::eError == rs) 
		{
			LOG_TO_FILE(Error occurred in creating);
			return rs;
		}
		
		tempWindowFlag = true;

		PIXELFORMATDESCRIPTOR pfd = 
		{
			sizeof(PIXELFORMATDESCRIPTOR),									//Size of PFD
			1,																//Version
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,		//Drawing to window, support openGL and buffering.
			PFD_TYPE_RGBA,													//Color is of RGBA type
			DCGraphicsConfig::s_defaultColorBufferDepth,					//24-bit color depth
			0, 0, 0, 0, 0, 0,												//Ignore color bits
			DCGraphicsConfig::s_defaultAlphaBufferDepth,					//8-bit alpha buffer
			0,
			0,
			0,																//No accumulation buffer
			DCGraphicsConfig::s_defaultDepthBufferDepth,					//24-bit depth buffer depth
			DCGraphicsConfig::s_defaultStencilBufferDepth,					//8-bit stencil buffer depth
			0,																//No auxilary buffer
			PFD_MAIN_PLANE,													//Main plane
			0,																//Reserved
			0, 0, 0															//Layer masks ignored
		};

		int pixelFormat = ChoosePixelFormat(m_windowHandle.m_hDc, &pfd);
		if (0 == pixelFormat) 
		{
			LOG_TO_FILE(Pixel format is not matches for the hardware);
			return ReturnStatus::eError;
		}
			
		if (!SetPixelFormat(m_windowHandle.m_hDc, pixelFormat, &pfd))
			return ReturnStatus::eError;

		//Create temporary context
		HGLRC tempContext = wglCreateContext(m_windowHandle.m_hDc);
		if (!tempContext) 
		{
			LOG_TO_FILE(Failed in context creation.);
			return ReturnStatus::eError;
		}
			
		wglMakeCurrent(m_windowHandle.m_hDc, tempContext);

		//Initialize GLEW
		if (GLEW_OK != glewInit()) 
		{
			LOG_TO_FILE(Unable to initialize GLEW);
			wglDeleteContext(tempContext);
			tempContext = NULL;
			return ReturnStatus::eError;
		}
		
		/* Choose extensible pixel format */
		if (wglChoosePixelFormatARB && wglCreateContextAttribsARB) 
		{
			//Remove the temporary stuff
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(tempContext);
			tempContext = NULL;
			ReleaseDC(m_windowHandle.m_hWnd, m_windowHandle.m_hDc);
			DestroyWindow(m_windowHandle.m_hWnd);
			m_windowHandle.m_hDc = NULL;
			m_windowHandle.m_hDc = NULL;
			
			tempWindowFlag = false;

			if (ReturnStatus::eError == CreateWindowForPlatform())
			{
				LOG_TO_FILE(Unable to create window);
				return ReturnStatus::eError;
			}
			
			int attributeList[] = 
			{
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB, DCGraphicsConfig::s_defaultColorBufferDepth,
				WGL_DEPTH_BITS_ARB, DCGraphicsConfig::s_defaultDepthBufferDepth,
				WGL_STENCIL_BITS_ARB, DCGraphicsConfig::s_defaultStencilBufferDepth,
				WGL_ALPHA_BITS_ARB, DCGraphicsConfig::s_defaultAlphaBufferDepth,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
				0
			};

			UINT formatCount = 0;
			if (!wglChoosePixelFormatARB(m_windowHandle.m_hDc, attributeList, NULL, 1, &pixelFormat, &formatCount) /*|| 0 == formatCount*/) 
			{
				LOG_TO_FILE(Given pixel format is not available for the hardware);
				return ReturnStatus::eError;
			}
			
			//Set new pixel format
			if (!SetPixelFormat(m_windowHandle.m_hDc, pixelFormat, &pfd)) 
			{
				return ReturnStatus::eError;
			}

			int glAttrib[] = 
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, m_graphicsParameters.m_glMajorVersion,
				WGL_CONTEXT_MINOR_VERSION_ARB, m_graphicsParameters.m_glMinorVersion,
				0
			};
			
			m_windowHandle.m_hGLRC = wglCreateContextAttribsARB(m_windowHandle.m_hDc, 0, glAttrib);
			if (NULL == m_windowHandle.m_hGLRC) 
			{
				return ReturnStatus::eError;
			}

			if (!wglMakeCurrent(m_windowHandle.m_hDc, m_windowHandle.m_hGLRC)) 
			{
				return ReturnStatus::eError;
			}
		}
		else 
		{
			/* If ARB extension is not available, set the default windows context */
			m_windowHandle.m_hGLRC = tempContext;
			rs = ReturnStatus::eWarning;
		}

		//Set vSync if available
		if (wglSwapIntervalEXT)
			wglSwapIntervalEXT(m_graphicsParameters.m_vSync ? 1 : 0);
		else
			m_graphicsParameters.m_vSync = false;

		return ReturnStatus::eOK;
	}

	ReturnStatus DCPlatform::CreateWindowForPlatform()
	{
		if (NULL == m_windowHandle.m_hInstance)
			return ReturnStatus::eError;

		// Decide dimensions of window
		int windowWidth = m_windowParameters.m_width;
		int windowHeight = m_windowParameters.m_height;
		int startPosX = m_windowParameters.m_startPositionX;
		int startPosY = m_windowParameters.m_startPositionY;

		if (0 == windowWidth || 0 == windowHeight || m_windowParameters.m_fullScreen)
		{
			GetDesktopResolution(windowWidth, windowHeight);
			startPosX = startPosY = 0;
		}

		// In fullscreen mode, set the DEVMODE settings
		if (m_windowParameters.m_fullScreen) 
		{
			DEVMODE devMode;
			memset(&devMode, 0, sizeof(devMode));
			devMode.dmSize = sizeof(devMode);
			devMode.dmPelsWidth = windowWidth;
			devMode.dmPelsHeight = windowHeight;
			devMode.dmBitsPerPel = DCGraphicsConfig::s_defaultColorBufferDepth + DCGraphicsConfig::s_defaultAlphaBufferDepth;
			devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			if (DISP_CHANGE_SUCCESSFUL != ChangeDisplaySettings(&devMode, CDS_FULLSCREEN)) 
			{
				LOG_TO_FILE(Failed to start application in full-screen mode with available resolution.);
				return ReturnStatus::eError;
			}
		}

		// Decides he suitable window style
		DWORD styleEx, style;

		if (m_windowParameters.m_fullScreen) 
		{
			style = WS_POPUP;
			styleEx = WS_EX_APPWINDOW;
		}
		else 
		{
			if (m_windowParameters.m_resizable)
				style = WS_OVERLAPPEDWINDOW;
			else 
				style = WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU;	

			styleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		}

		if (m_windowParameters.m_minimize)
			style |= WS_MINIMIZE;

		style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
				
		// Adjust the window size so the client area is of requested size.
		RECT winSize;
		winSize.left = startPosX;
		winSize.top = startPosY;
		winSize.right = windowWidth;
		winSize.bottom = windowHeight;

		AdjustWindowRectEx(&winSize, style, FALSE, styleEx);

		startPosX = winSize.left;
		startPosY = winSize.top;
		windowWidth = winSize.right - winSize.left;
		windowHeight = winSize.bottom - winSize.top;

		// Create the window
		std::wstring className = m_windowParameters.m_windowName + L"Class";

		const wchar_t* CLASSNAME = className.c_str();

		m_windowHandle.m_hWnd = CreateWindowEx(styleEx,
			CLASSNAME,
			m_windowParameters.m_windowName.c_str(),
			style,
			startPosX,
			startPosY,
			windowWidth,
			windowHeight,
			NULL, NULL,
			m_windowHandle.m_hInstance,
			NULL);

		if (NULL == m_windowHandle.m_hWnd)
			return ReturnStatus::eError;

		m_windowHandle.m_hDc = GetDC(m_windowHandle.m_hWnd);
		if (NULL == m_windowHandle.m_hDc) 
		{
			LOG_TO_FILE(Unable to create device context);
			DestroyWindow(m_windowHandle.m_hWnd);
			m_windowHandle.m_hWnd = NULL;
			return ReturnStatus::eError;
		}
		
		return ReturnStatus::eOK;
	}

	ReturnStatus DCPlatform::CreatePlatform(std::wstring iWindowName, std::string iConfigFilePath, DCPlatform** oPlateform)
	{
		ReturnStatus rs = ReturnStatus::eOK;
		DCPlatform* platform = new DCPlatform();

		//Initialize the window parameters.
		platform->Init();

		if (iConfigFilePath != "") 
		{
			platform->m_configuration = new DCConfiguration(iConfigFilePath);
			rs = platform->m_configuration->LoadConfiguration();
			platform->FillWindowConfiguration();
			platform->m_configuration->ReleaseConfiguration();
		}
		
		if (platform->m_graphicsParameters.m_glMajorVersion < 3 || (platform->m_graphicsParameters.m_glMajorVersion < 3 && platform->m_graphicsParameters.m_glMinorVersion < 1)) 
		{
			LOG_TO_FILE(OpenGL version should be more than 3.1);
			return ReturnStatus::eError;
		}
			
		platform->m_windowParameters.m_windowName = iWindowName;

		platform->m_windowHandle.m_hInstance = ::GetModuleHandle(NULL);
		std::wstring className = platform->m_windowParameters.m_windowName + L"Class";

		const wchar_t* CLASSNAME = className.c_str();

		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = platform->m_windowHandle.m_hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = CLASSNAME;

		if (!::RegisterClassEx(&wc))
		{
			std::cout << "Failed to register window." << std::endl;
			delete platform;
			platform = NULL;
			return ReturnStatus::eError;
		}

		rs = platform->InitializeGL();
		if (ReturnStatus::eError == rs) 
		{
			LOG_TO_FILE(Unable to initialize graphics);
			delete platform;
			platform = NULL;
			return rs;
		}
		
		ShowWindow(platform->m_windowHandle.m_hWnd, SW_SHOW);
		
		*oPlateform = platform;		//Return created platform

		return ReturnStatus::eOK;
	}

	void DCPlatform::EnterEventLoop() 
	{
		MSG msg = {};

		bool loopCompletion = false;
		while (!loopCompletion)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			
			if (WM_QUIT == msg.message)
			{
				loopCompletion = true;
			}
		}
	}

	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if(tempWindowFlag)
			return DefWindowProc(hWnd, uMsg, wParam, lParam);

		HDC hDc;
		
		switch (uMsg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
			PAINTSTRUCT ps;
			hDc = BeginPaint(hWnd, &ps);

			FillRect(hDc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

			EndPaint(hWnd, &ps);

			return 0;

		case WM_CLOSE:
			if (MessageBox(hWnd, L"Really Quit?", L"My Application", MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(hWnd);
			}
			return 0;

		//Temp
		default :
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_TRIANGLES);
			glVertex2f(-0.5f, 0.0f);
			glVertex2f(0.0f, 0.5f);
			glVertex2f(0.5f, 0.0f);
			glEnd();

			SwapBuffers(GetDC(hWnd));
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

#endif

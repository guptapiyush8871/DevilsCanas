#ifndef DCWindowParameters_hpp
#define DCWindowParameters_hpp

#include "DCPlatform.h"
#include <Windows.h>
#include <string>

namespace DevilsCanvas
{
	struct DCWindowParameters
	{
		friend class DCPlatform;

		enum EDCWindowParametersType 
		{
			eStartPositionX=0,
			eStartPositionY,
			eWidth,
			eHeight,
			eIcon,
			eFullScreen,
			eResizable,
			eMinimize,
			eEnd
		};

	private :
		int m_startPositionX;
		int m_startPositionY;
		int m_width;
		int m_height;
		HICON m_hIcon;
		std::wstring m_windowName;
		bool m_fullScreen;
		bool m_resizable;
		bool m_minimize;
	};
	typedef DCWindowParameters::EDCWindowParametersType WinParamType;

	struct DCGraphicsParameters
	{
		friend DCPlatform;

		enum EDCGraphicsParametersType
		{
			eGLMajorVersion= 0,
			eGLMinorVersion,
			eVSync,
			eEnd
		};

	private:
		int m_glMajorVersion;
		int m_glMinorVersion;
		int m_vSync;
	};
	typedef DCGraphicsParameters::EDCGraphicsParametersType GraphicsParamType;

	struct DCWindowHandle 
	{
	public :
		friend class DCPlatform;

	private :
		HINSTANCE m_hInstance;
		HWND m_hWnd;
		HDC m_hDc;
		HGLRC m_hGLRC;
	};
}

#endif

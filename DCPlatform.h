#ifndef DCPlatform_h
#define DCPlatform_h

#include "DCBase.h"
#include "DCWindowParameters.hpp"
#include <GL/glew.h>
#include <GL/wglew.h>
#include <string>

namespace DevilsCanvas 
{
	class DCConfiguration;

	class DCPlatform 
	{
	public :
		virtual ~DCPlatform();
		
		static ReturnStatus CreatePlatform(std::wstring iWindowName, std::string iConfigFilePath="", DCPlatform** oPlateform=NULL);

		void EnterEventLoop();

	private :
		DCPlatform();
		DCPlatform(const DCPlatform&);
		DCPlatform& operator=(const DCPlatform&);

		void Init();
		void FillWindowConfiguration();
		void GetDesktopResolution(int& oWidth, int &oHeight);
		ReturnStatus CreateWindowForPlatform();
		ReturnStatus InitializeGL();

		DCWindowParameters m_windowParameters;
		DCGraphicsParameters m_graphicsParameters;
		DCWindowHandle m_windowHandle;
		DCConfiguration* m_configuration;
	};
}

#endif

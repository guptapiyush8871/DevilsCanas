#ifndef DCConfigurationFileGrammar_h
#define DCConfigurationFileGrammar_h

#include <string>

namespace DevilsCanvas
{
	namespace DCConfigurationFileGrammar 
	{
		static char s_comment = '#';
		static char s_assignment = '=';
		static char s_termination = ';';
		static std::string s_type = "TYPE";
		static std::string s_typeValDefault = "DEFAULT";
		static std::string s_typeValWindow = "WINDOW";
		static std::string s_typeValGraphics = "GRAPHICS";
		static std::string s_windowParamFullScreen = "FULLSCREEN";
		static std::string s_windowParamResizable = "RESIZABLE";
		static std::string s_windowParamMinimize = "MINIMIZE";
		static std::string s_windowParamWidth = "WIDTH";
		static std::string s_windowParamHeight = "HEIGHT";
		static std::string s_windowParamPosX = "POSITION_X";
		static std::string s_windowParamPosY = "POSITION_Y";
		static std::string s_windowParamIcon = "ICON";
		static std::string s_ConfigValTrue = "TRUE";
		static std::string s_ConfigValFalse	 = "FALSE";
		static std::string s_glMajorVersion = "GL_MAJOR_VERSION";
		static std::string s_glMinorVersion = "GL_MINOR_VERSION";
		static std::string s_vSync = "VSYNC";
	}
}

#endif

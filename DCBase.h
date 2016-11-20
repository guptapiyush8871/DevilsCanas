#ifndef DCBase_h
#define DCBase_h

namespace DevilsCanvas 
{
	/* Define return codes */
	struct ReturnCodes 
	{
		enum RC 
		{
			eOK=0,
			eWarning,
			eError
		};
	};

	typedef ReturnCodes::RC ReturnStatus;

	namespace DCGraphicsConfig 
	{
		static const int s_defaultColorBufferDepth = 24;
		static const int s_defaultAlphaBufferDepth = 8;
		static const int s_defaultDepthBufferDepth = 24;
		static const int s_defaultStencilBufferDepth = 8;
	}
}

#endif

#ifndef DCConfiguration_h
#define DCConfiguration_h

#include "DCBase.h"
#include <string>
#include <map>
#include <vector>

namespace DevilsCanvas 
{
	class DCConfiguration 
	{
	public :
		DCConfiguration(const std::string iFilePath);
		virtual ~DCConfiguration();

		ReturnStatus LoadConfiguration();
		std::string GetValue(std::string iType, std::string iProperty);
		void ReleaseConfiguration();

	private :
		DCConfiguration();
		DCConfiguration(const DCConfiguration&) {};
		DCConfiguration& operator=(const DCConfiguration&) {};
		
		/*
			There can be a posible loss of data if ".config" file is curropted. 
			Warning(ReturnStatus::eWarning) will be signaled at this time.
		*/
		ReturnStatus LoadData(std::string& oData);	//Load the data from the file.

		//Refine the line by removing comments and extra spaces.
		ReturnStatus RefineLine(const std::string iData, std::string& oData);	
		
		/*
			Refine the tioekns. Remove extra leading and trailing spaces. 
			Returns Warning(ReturnStatus::eWarning) if any of the token is in the bad format.
		*/
		ReturnStatus RefineTokens(std::vector<std::string> iTokenData, std::vector<std::string>& oTokenData);
		ReturnStatus RefineToken(std::string& ioTokenData);

		ReturnStatus FillConfiguration(const std::string iData);	//Fill the configuration into the map.

		//Tokenize the string and returns the vector of tokens.
		std::vector<std::string> StringTokenizer(const std::string iString, const std::string iDelim);

		std::string m_configFilePath;
		std::map<std::string, std::map<std::string, std::string>> m_configuration;
	};
}

#endif

#include "DCLogger.h"
#include "DCConfiguration.h"
#include "DCConfigurationFileGrammar.h"
#include <fstream>
#include <iterator>
#include <algorithm>

namespace DevilsCanvas 
{
	DCConfiguration::DCConfiguration(const std::string iFilePath) 
	{
		m_configFilePath = iFilePath;
	}

	DCConfiguration::~DCConfiguration() 
	{
		ReleaseConfiguration();
	}

	ReturnStatus DCConfiguration::LoadConfiguration()
	{
		if ("" == m_configFilePath) 
		{
			LOG_TO_FILE(Config file path is empty.);
			return ReturnStatus::eError;
		}
		
		std::string data = "";
		ReturnStatus rs = LoadData(data);
		if (ReturnStatus::eError == rs) 
		{
			LOG_TO_FILE(Unable to load data from the config file. Check its format);
			return rs;
		}
		
		if ("" == data) 
		{
			LOG_TO_FILE(Retrieve data from file is empty);
			return ReturnStatus::eError;
		}
		
		/* Fill the map */
		rs = FillConfiguration(data);

		return rs;
	}

	std::string DCConfiguration::GetValue(std::string iType, std::string iProperty) 
	{
		return m_configuration[iType][iProperty];
	}

	void DCConfiguration::ReleaseConfiguration() 
	{
		m_configuration.clear();
	}

	ReturnStatus DCConfiguration::LoadData(std::string& oData)
	{
		ReturnStatus rsFinal = ReturnStatus::eOK;
		ReturnStatus rsTemp = ReturnStatus::eOK;
		std::string data = "";
		std::string line = "";
		std::ifstream file(m_configFilePath);

		while (std::getline(file, line)) 
		{
			if (DCConfigurationFileGrammar::s_comment == line[0])
				continue;

			else 
			{
				rsTemp = RefineLine(line, line);
				if (ReturnStatus::eWarning == rsTemp)
				{
					rsFinal = rsTemp;
					continue;
				}

				else if ("" == line)
					continue;

				else if (ReturnStatus::eError == rsTemp) 
				{
					return rsTemp;
				}
			}

			data += line;
		}

		if ("" == data || std::string::npos == data.find(DCConfigurationFileGrammar::s_assignment)) 
		{
			rsFinal = ReturnStatus::eError;
		}

		oData = data;
		return rsFinal;
	}

	ReturnStatus DCConfiguration::RefineLine(const std::string iData, std::string& oData)
	{
		if ("" == iData)
			return ReturnStatus::eOK;

		ReturnStatus rsFinal = ReturnStatus::eOK;
		int assignOperatorIndex = iData.find(DCConfigurationFileGrammar::s_assignment);
		if (std::string::npos == assignOperatorIndex) 
		{
			oData = "";
			LOG_TO_FILE(Data given in file is not good. Check file.);
			return ReturnStatus::eWarning;
		}
				
		int terminationOperatorIndex = iData.find(DCConfigurationFileGrammar::s_termination);
		if (std::string::npos == terminationOperatorIndex) 
		{
			oData = "";
			LOG_TO_FILE(Data given in file is not good.Check file.);
			return ReturnStatus::eWarning;
		}
		
		int commentIndex = iData.find(DCConfigurationFileGrammar::s_comment);
		std::string data = "";
		if (std::string::npos != commentIndex)
			data = iData.substr(0, commentIndex);
		else
			data = iData;
		
		//Tokenize string for delimitor ';'
		std::vector<std::string> tokens = StringTokenizer(data, std::string(&DCConfigurationFileGrammar::s_termination));
		
		//Refine tokens
		std::vector<std::string> refineTokens;
		rsFinal = RefineTokens(tokens, refineTokens);

		//Fill Data
		oData = "";
		for (std::vector<std::string>::iterator it = refineTokens.begin(); it != refineTokens.end(); ++it) 
		{
			oData += *it;
			oData.push_back('\n');
		}

		return rsFinal;
	}

	ReturnStatus DCConfiguration::RefineTokens(std::vector<std::string> iTokenData, std::vector<std::string>& oTokenData)
	{
		if (iTokenData.empty())
			return ReturnStatus::eWarning;

		ReturnStatus rsFinal = ReturnStatus::eOK;
		for (std::vector<std::string>::iterator it = iTokenData.begin(); it != iTokenData.end(); ++it) 
		{
			ReturnStatus rs = RefineToken(*it);
			if (ReturnStatus::eWarning == rs) 
			{
				rsFinal = rs;
			}
			else 
			{
				oTokenData.push_back(*it);
			}
		}
		return rsFinal;
	}

	ReturnStatus DCConfiguration::RefineToken(std::string& ioTokenData)
	{
		if ("" == ioTokenData)
			return ReturnStatus::eWarning;

		ReturnStatus rsFinal = ReturnStatus::eOK;

		size_t assignOperatorCount = std::count(ioTokenData.begin(), ioTokenData.end(), '=');
		if (assignOperatorCount != 1)
			return ReturnStatus::eWarning;

		//Remove leading and trailing spaces.
		const char* trimData = " \t\n\r\f\v";
		ioTokenData.erase(0, ioTokenData.find_first_not_of(trimData));
		ioTokenData.erase(ioTokenData.find_last_not_of(trimData) + 1);

		const char* trimData2 = " \t\r\v";
		int assignOperatorIndex = ioTokenData.find(DCConfigurationFileGrammar::s_assignment);
		
		int firstRemove = ioTokenData.find_first_of(trimData2);
		if (std::string::npos != firstRemove && firstRemove < assignOperatorIndex)
			ioTokenData.erase(firstRemove, assignOperatorIndex);
		
		ioTokenData.erase(assignOperatorIndex + 1, ioTokenData.find_first_not_of(trimData2));
		
		return rsFinal;
	}

	std::vector<std::string> DCConfiguration::StringTokenizer(const std::string iString, const std::string iDelim) 
	{
		std::vector<std::string> tokenData;

		// Skip delimiters at beginning.
		std::string::size_type lastPos = iString.find_first_not_of(iDelim, 0);
		
		// Find first "non-delimiter".
		std::string::size_type pos = iString.find_first_of(iDelim, lastPos);

		while (std::string::npos != pos || std::string::npos != lastPos)
		{
			// Found a token, add it to the vector.
			tokenData.push_back(iString.substr(lastPos, pos - lastPos));
			
			// Skip delimiters.  Note the "not_of"
			lastPos = iString.find_first_not_of(iDelim, pos);
			
			// Find next "non-delimiter"
			pos = iString.find_first_of(iDelim, lastPos);
		}

		return tokenData;
	}

	ReturnStatus DCConfiguration::FillConfiguration(const std::string iData)
	{
		if (iData == "") 
		{
			LOG_TO_FILE(Given input is empty "DCConfiguration::FillConfiguration".);
			return ReturnStatus::eError;
		}
			
		std::vector<std::string> data = StringTokenizer(iData, std::string(1, '\n'));
		std::map<std::string, std::string> innerValues;

		if (std::string::npos == iData.find(DCConfigurationFileGrammar::s_type)) 
		{
			for (std::vector<std::string>::iterator it = data.begin(); it != data.end(); ++it) 
			{
				std::string token = *it;
				std::string key = token.substr(0, token.find(DCConfigurationFileGrammar::s_assignment));
				std::string value = token.substr(token.find(DCConfigurationFileGrammar::s_assignment)+1, token.find('\n'));
				innerValues[key] = value;
			}
			m_configuration[DCConfigurationFileGrammar::s_typeValDefault] = innerValues;
		}
		else 
		{
			std::string type = DCConfigurationFileGrammar::s_typeValDefault;
			bool typeFlag = false;

			for (std::vector<std::string>::iterator it = data.begin(); it != data.end(); ++it)
			{
				std::string token = *it;
				std::string key = token.substr(0, token.find(DCConfigurationFileGrammar::s_assignment));
				std::string value = token.substr(token.find(DCConfigurationFileGrammar::s_assignment) + 1, token.find('\n'));
				
				if (DCConfigurationFileGrammar::s_type == key) 
				{
					if (type != value) 
					{
						m_configuration[type] = innerValues;
						innerValues.clear();
						type = value;
						continue;
					}

					typeFlag = true;
					type = value;
					continue;
				}
				
				typeFlag = false;
				innerValues[key] = value;
			}

			m_configuration[type] = innerValues;

			if (m_configuration[DCConfigurationFileGrammar::s_typeValDefault].empty())
				m_configuration.erase(DCConfigurationFileGrammar::s_typeValDefault);
		}

		return ReturnStatus::eOK;
	}
}

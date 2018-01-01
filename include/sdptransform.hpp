#ifndef SDPTRANSFORM_HPP
#define SDPTRANSFORM_HPP

#include "json.hpp"
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <functional>

using json = nlohmann::json;

namespace sdptransform
{
	namespace grammar
	{
		struct Rule
		{
			std::string name;
			std::string push;
			std::regex reg;
			std::vector<std::string> names;
			std::string format;
			std::function<const std::string(const json&)> formatFunc;
		};

		extern const std::map<char, std::vector<Rule>> rulesMap;
	}

	json parse(const std::string& sdp);

	json parseParams(const std::string& str);

	inline json parseFmtpConfig(const std::string& str)
	{
		return parseParams(str);
	}

	json parseImageAttributes(const std::string& str);

	std::string write(json& session);
}

#endif

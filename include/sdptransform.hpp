#ifndef SDPTRANSFORM_HPP
#define SDPTRANSFORM_HPP

#include "json.hpp"
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <functional>

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
			std::function<std::string(nlohmann::json&)> formatFunc;
		};

		extern std::map<char, std::vector<Rule>> mapRules;
	}

	inline nlohmann::json getEmptySession()
	{
		nlohmann::json session{ nlohmann::json::object() };

		return session;
	}

	void parse(std::string& sdp, nlohmann::json& session);

	std::string write(nlohmann::json& session);
}

#endif

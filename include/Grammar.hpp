#ifndef SDPTRANSFORM_GRAMMAR_HPP
#define SDPTRANSFORM_GRAMMAR_HPP

#include "json.hpp"
#include <vector>
#include <string>
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
}

#endif

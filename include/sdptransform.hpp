#ifndef SDPTRANSFORM_HPP
#define SDPTRANSFORM_HPP

#include <nlohmann/json_fwd.hpp>

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
			std::vector<char> types;
			std::string format;
			std::function<const std::string(const nlohmann::json&)> formatFunc;
		};

		extern const std::map<char, std::vector<Rule>> rulesMap;
	}

	nlohmann::json parse(const std::string& sdp);

	nlohmann::json parseParams(const std::string& str);

	std::vector<int> parsePayloads(const std::string& str);

	nlohmann::json parseImageAttributes(const std::string& str);

	nlohmann::json parseSimulcastStreamList(const std::string& str);

	std::string write(const nlohmann::json& session);
}

#endif // SDPTRANSFORM_HPP

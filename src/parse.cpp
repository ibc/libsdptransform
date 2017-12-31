#include "sdptransform.hpp"
#include "Grammar.hpp"
#include <cstddef> // size_t
#include <memory> // std::addressof()
#include <sstream>
#include <regex>

using json = nlohmann::json;

namespace sdptransform
{
	void parseReg(Grammar::Rule& rule, json& location, std::string& content);
	void attachProperties(
		std::smatch& match, json& location, std::vector<std::string>& names, std::string& rawName);
	json toIntIfInt(const std::ssub_match& subMatch);
	bool isNumber(const std::string& string);

	bool parse(std::string& sdp, json& session)
	{
		static const std::regex RegexSdpValidLine("^([a-z])=(.*)");

		std::stringstream sdpstream(sdp);
		std::string line;
		json media = json::array();
		json* location = std::addressof(session);

		if (!session.is_object())
			return false;

		while (std::getline(sdpstream, line, '\n'))
		{
			// Remove \r if lines are separated with \r\n (as mandated in SDP).
			if (line[line.length() - 1] == '\r')
				line.pop_back();

			// Ensure it's a valid SDP line.
			if (!std::regex_match(line, RegexSdpValidLine))
				continue;

			char type = line[0];
			std::string content = line.substr(2);

			if (type == 'm')
			{
				json m = json::object();

				m["rtp"] = json::array();
				m["fmtp"] = json::array();

				media.push_back(m);
				location = std::addressof(media[media.size() - 1]); // Point at latest media line.
			}

			if (Grammar::mapRules.find(type) == Grammar::mapRules.end())
				continue;

			for (int j = 0; j < Grammar::mapRules[type].size(); ++j)
			{
				auto& rule = Grammar::mapRules[type][j];

				if (std::regex_match(content, rule.reg))
				{
					parseReg(rule, *location, content);

					break;
				}
			}
		}

		session["media"] = media; // Link it up.

		return true;
	}

	void parseReg(Grammar::Rule& rule, json& location, std::string& content)
	{
		bool needsBlank = !rule.name.empty() && !rule.names.empty();

		if (!rule.push.empty() && location.find(rule.push) == location.end())
		{
			location[rule.push] = json::array();
		}
		else if (needsBlank && location.find(rule.name) == location.end())
		{
			location[rule.name] = json::object();
		}

		std::smatch match;

		std::regex_match(content, match, rule.reg);

		json object = json::object();
		json& keyLocation = !rule.push.empty() ?
			object : // Blank object that will be pushed.
			needsBlank ? location[rule.name] : location; // Otherwise named location or root.

		attachProperties(match, keyLocation, rule.names, rule.name);

		if (!rule.push.empty())
			location[rule.push].push_back(keyLocation);
	}

	void attachProperties(
		std::smatch& match, json& location, std::vector<std::string>& names, std::string& rawName)
	{
		if (!rawName.empty() && names.empty())
		{
			location[rawName] = toIntIfInt(match[1]);
		}
		else
		{
			for (size_t i = 0; i < names.size(); ++i)
			{
				if (i + 1 < match.size())
					location[names[i]] = toIntIfInt(match[i + 1]);
			}
		}
	}

	json toIntIfInt(const std::ssub_match& subMatch)
	{
		auto string = subMatch.str();

		if (!isNumber(string))
			return string;

		try
		{
			return std::stoll(string);
		}
		catch (const std::exception& error)
		{
			return string;
		}
	}

	bool isNumber(const std::string& string)
	{
		return (
			!string.empty() &&
			std::find_if(
				string.begin(),
				string.end(),
				[](char c) { return !std::isdigit(c); }) == string.end()
		);
	}
}

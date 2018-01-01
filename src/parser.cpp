#include "sdptransform.hpp"
#include <cstddef>   // size_t
#include <memory>    // std::addressof()
#include <sstream>   // std::stringstream, std::istringstream
#include <algorithm> // std::find_if()
#include <locale>    // std::isspace()

namespace sdptransform
{
	void parseReg(const grammar::Rule& rule, json& location, const std::string& content);

	void attachProperties(
		const std::smatch& match,
		json& location,
		const std::vector<std::string>& names,
		const std::string& rawName
	);

	json toNumberIfNumber(const std::string& str);

	bool isNumber(const std::string& str);

	void trim(std::string &str);

	void insertParam(json& o, const std::string& str);

	json parse(const std::string& sdp)
	{
		static const std::regex ValidLineRegex("^([a-z])=(.*)");

		json session = json::object();
		std::stringstream sdpstream(sdp);
		std::string line;
		json media = json::array();
		json* location = std::addressof(session);

		while (std::getline(sdpstream, line, '\n'))
		{
			// Remove \r if lines are separated with \r\n (as mandated in SDP).
			if (line[line.length() - 1] == '\r')
				line.pop_back();

			// Ensure it's a valid SDP line.
			if (!std::regex_search(line, ValidLineRegex))
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

			if (grammar::rulesMap.find(type) == grammar::rulesMap.end())
				continue;

			for (int j = 0; j < grammar::rulesMap.at(type).size(); ++j)
			{
				auto& rule = grammar::rulesMap.at(type)[j];

				if (std::regex_search(content, rule.reg))
				{
					parseReg(rule, *location, content);

					break;
				}
			}
		}

		session["media"] = media; // Link it up.

		return session;
	}

	json parseParams(const std::string& str)
	{
		json params = json::object();
		std::stringstream ss(str);
		std::string param;

		while (std::getline(ss, param, ';'))
		{
			trim(param);

			if (param.length() == 0)
				continue;

			insertParam(params, param);
		}

		return params;
	}

	void parseReg(const grammar::Rule& rule, json& location, const std::string& content)
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

		std::regex_search(content, match, rule.reg);

		json object = json::object();
		json& keyLocation = !rule.push.empty() ?
			object : // Blank object that will be pushed.
			needsBlank ? location[rule.name] : location; // Otherwise named location or root.

		attachProperties(match, keyLocation, rule.names, rule.name);

		if (!rule.push.empty())
			location[rule.push].push_back(keyLocation);
	}

	void attachProperties(
		const std::smatch& match,
		json& location,
		const std::vector<std::string>& names,
		const std::string& rawName
	)
	{
		if (!rawName.empty() && names.empty())
		{
			location[rawName] = toNumberIfNumber(match[1].str());
		}
		else
		{
			for (size_t i = 0; i < names.size(); ++i)
			{
				if (i + 1 < match.size() && !match[i + 1].str().empty())
					location[names[i]] = toNumberIfNumber(match[i + 1].str());
			}
		}
	}

	json toNumberIfNumber(const std::string& str)
	{
		// https://stackoverflow.com/a/447307/4827838.

		// Test long long.
		{
			std::istringstream iss(str);
			long long ll;

			iss >> std::noskipws >> ll;

			if (iss.eof() && !iss.fail())
				return std::stoll(str);
		}

		// Test double.
		{
			std::istringstream iss(str);
			double d;

			iss >> std::noskipws >> d;

			if (iss.eof() && !iss.fail())
				return std::stod(str);
		}

		// Otherwise return it as a string.
		return str;
	}

	void trim(std::string &str)
	{
		str.erase(
			str.begin(),
			std::find_if(
				str.begin(), str.end(), [](int ch) { return !std::isspace(ch); }
			)
		);

		str.erase(
			std::find_if(
				str.rbegin(), str.rend(), [](int ch) { return !std::isspace(ch); }).base(),
			str.end()
		);
	}

	void insertParam(json& o, const std::string& str)
	{
		static const std::regex KeyValueRegex("^\\s*([^= ]+)\\s*=\\s*([^ ]+)$");

		std::smatch match;

		std::regex_match(str, match, KeyValueRegex);

		// Wrong key value syntax.
		if (match.size() != 3)
			return;

		// Insert into the given JSON object.
		o[match[1].str()] = toNumberIfNumber(match[2].str());
	}
}

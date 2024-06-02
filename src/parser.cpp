#include "sdptransform.hpp"

#include <nlohmann/json.hpp>

#include <unordered_map>
#include <cstddef>   // size_t
#include <memory>    // std::addressof()
#include <sstream>   // std::stringstream, std::istringstream
#include <ios>       // std::noskipws
#include <algorithm> // std::find_if()
#include <cctype>    // std::isspace()
#include <cstdint>   // std::uint64_t

namespace sdptransform
{
	void parseReg(const grammar::Rule& rule, nlohmann::json& location, const std::string& content);

	void attachProperties(
		const std::smatch& match,
		nlohmann::json& location,
		const std::vector<std::string>& names,
		const std::string& rawName,
		const std::vector<char>& types
	);

	nlohmann::json toType(const std::string& str, char type);

	bool isNumber(const std::string& str);

	bool isInt(const std::string& str);

	bool isFloat(const std::string& str);

	void trim(std::string& str);

	void insertParam(nlohmann::json& o, const std::string& str);

	nlohmann::json parse(const std::string& sdp)
	{
		static const std::regex ValidLineRegex("^([a-z])=(.*)");

		nlohmann::json session = nlohmann::json::object();
		std::stringstream sdpstream(sdp);
		std::string line;
		nlohmann::json media = nlohmann::json::array();
		nlohmann::json* location = std::addressof(session);

		while (std::getline(sdpstream, line, '\n'))
		{
			// Remove \r if lines are separated with \r\n (as mandated in SDP).
			if (line.size() && line[line.length() - 1] == '\r')
				line.pop_back();

			// Ensure it's a valid SDP line.
			if (!std::regex_search(line, ValidLineRegex))
				continue;

			char type = line[0];
			std::string content = line.substr(2);

			if (type == 'm')
			{
				nlohmann::json m = nlohmann::json::object();

				m["rtp"] = nlohmann::json::array();
				m["fmtp"] = nlohmann::json::array();

				media.push_back(m);

				// Point at latest media line.
				location = std::addressof(media[media.size() - 1]);
			}

			auto it = grammar::rulesMap.find(type);

			if (it == grammar::rulesMap.end())
				continue;

			auto& rules = it->second;

			for (size_t j = 0; j < rules.size(); ++j)
			{
				auto& rule = rules[j];

				if (std::regex_search(content, rule.reg))
				{
					parseReg(rule, *location, content);

					break;
				}
			}
		}

		// Link it up.
		session["media"] = media;

		return session;
	}

	nlohmann::json parseParams(const std::string& str)
	{
		nlohmann::json obj = nlohmann::json::object();
		std::stringstream ss(str);
		std::string param;

		while (std::getline(ss, param, ';'))
		{
			trim(param);

			if (param.length() == 0)
				continue;

			insertParam(obj, param);
		}

		return obj;
	}

	std::vector<int> parsePayloads(const std::string& str)
	{
		std::vector<int> arr;

		std::stringstream ss(str);
		std::string payload;

		while (std::getline(ss, payload, ' '))
		{
			arr.push_back(std::stoi(payload));
		}

		return arr;
	}

	nlohmann::json parseImageAttributes(const std::string& str)
	{
		nlohmann::json arr = nlohmann::json::array();
		std::stringstream ss(str);
		std::string item;

		while (std::getline(ss, item, ' '))
		{
			trim(item);

			// Special case for * value.
			if (item == "*")
				return item;

			if (item.length() < 5) // [x=0]
				continue;

			nlohmann::json obj = nlohmann::json::object();
			std::stringstream ss2(item.substr(1, item.length() - 2));
			std::string param;

			while (std::getline(ss2, param, ','))
			{
				trim(param);

				if (param.length() == 0)
					continue;

				insertParam(obj, param);
			}

			arr.push_back(obj);
		}

		return arr;
	}

	nlohmann::json parseSimulcastStreamList(const std::string& str)
	{
		nlohmann::json arr = nlohmann::json::array();
		std::stringstream ss(str);
		std::string item;

		while (std::getline(ss, item, ';'))
		{
			if (item.length() == 0)
				continue;

			nlohmann::json arr2 = nlohmann::json::array();
			std::stringstream ss2(item);
			std::string format;

			while (std::getline(ss2, format, ','))
			{
				if (format.length() == 0)
					continue;

				nlohmann::json obj = nlohmann::json::object();

				if (format[0] != '~')
				{
					obj["scid"] = format;
					obj["paused"] = false;
				}
				else
				{
					obj["scid"] = format.substr(1);
					obj["paused"] = true;
				}

				arr2.push_back(obj);
			}

			arr.push_back(arr2);
		}

		return arr;
	}

	void parseReg(const grammar::Rule& rule, nlohmann::json& location, const std::string& content)
	{
		bool needsBlank = !rule.name.empty() && !rule.names.empty();

		if (!rule.push.empty() && location.find(rule.push) == location.end())
		{
			location[rule.push] = nlohmann::json::array();
		}
		else if (needsBlank && location.find(rule.name) == location.end())
		{
			location[rule.name] = nlohmann::json::object();
		}

		std::smatch match;

		std::regex_search(content, match, rule.reg);

		nlohmann::json object = nlohmann::json::object();
		nlohmann::json& keyLocation = !rule.push.empty()
			// Blank object that will be pushed.
			? object
			// Otherwise named location or root.
			: needsBlank
				? location[rule.name]
				: location;

		attachProperties(match, keyLocation, rule.names, rule.name, rule.types);

		if (!rule.push.empty())
			location[rule.push].push_back(keyLocation);
	}

	void attachProperties(
		const std::smatch& match,
		nlohmann::json& location,
		const std::vector<std::string>& names,
		const std::string& rawName,
		const std::vector<char>& types
	)
	{
		if (!rawName.empty() && names.empty())
		{
			location[rawName] = toType(match[1].str(), types[0]);
		}
		else
		{
			for (size_t i = 0; i < names.size(); ++i)
			{
				if (i + 1 < match.size() && !match[i + 1].str().empty())
				{
					location[names[i]] = toType(match[i + 1].str(), types[i]);
				}
			}
		}
	}

	bool isInt(const std::string& str)
	{
		std::istringstream iss(str);
		long l;

		iss >> std::noskipws >> l;

		return iss.eof() && !iss.fail();
	}

	bool isFloat(const std::string& str)
	{
		std::istringstream iss(str);
		float f;

		iss >> std::noskipws >> f;

		return iss.eof() && !iss.fail();
	}

	nlohmann::json toType(const std::string& str, char type)
	{
		// https://stackoverflow.com/a/447307/4827838.

		switch (type)
		{
			case 's':
			{
				return str;
			}

			case 'u':
			{
				std::istringstream iss(str);
				std::uint64_t ll;

				iss >> std::noskipws >> ll;

				if (iss.eof() && !iss.fail())
					return ll;
				else
					return 0u;
			}

			case 'd':
			{
				std::istringstream iss(str);
				std::int64_t ll;

				iss >> std::noskipws >> ll;

				if (iss.eof() && !iss.fail())
					return ll;
				else
					return 0;
			}

			case 'f':
			{
				std::istringstream iss(str);
				double d;

				iss >> std::noskipws >> d;

				if (iss.eof() && !iss.fail())
					return std::stod(str);
				else
					return 0.0f;
			}
		}

		return nullptr;
	}

	void trim(std::string& str)
	{
		str.erase(
			str.begin(),
			std::find_if(
				str.begin(), str.end(), [](unsigned char ch) { return !std::isspace(ch); }
			)
		);

		str.erase(
			std::find_if(
				str.rbegin(), str.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(),
			str.end()
		);
	}

	// @str parameters is a string like "profile-level-id=42e034".
	void insertParam(nlohmann::json& o, const std::string& str)
	{
		static const std::regex KeyValueRegex("^\\s*([^= ]+)(?:\\s*=\\s*([^ ]+))?$");
		static const std::unordered_map<std::string, char> WellKnownParameters =
		{
			// H264 codec parameters.
			{ "profile-level-id",   's' },
			{ "packetization-mode", 'd' },
			// VP9 codec parameters.
			{ "profile-id",         's' }
		};

		std::smatch match;

		std::regex_match(str, match, KeyValueRegex);

		if (match.size() == 0)
			return;

		std::string param = match[1].str();
		std::string value = match[2].str();

		auto it = WellKnownParameters.find(param);
		char type;

		if (it != WellKnownParameters.end())
			type = it->second;
		else if (isInt(match[2].str()))
			type = 'd';
		else if (isFloat(match[2].str()))
			type = 'f';
		else
			type = 's';

		// Insert into the given JSON object.
		o[match[1].str()] = toType(match[2].str(), type);
	}
}

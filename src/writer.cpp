#include "sdptransform.hpp"
#include <sstream> // std::stringstream
// TODO: REMOVE
#include <iostream>

using json = nlohmann::json;

namespace sdptransform
{
	const std::string makeLine(char type, const grammar::Rule& rule, const json& location);

	const std::string format(const std::string& line, std::vector<json>& args);

	std::string write(json& session)
	{
		// RFC specified order.
		static const std::vector<char> OuterOrder =
			{ 'v', 'o', 's', 'i', 'u', 'e', 'p', 'c', 'b', 't', 'r', 'z', 'a' };
		static const std::vector<char> InnerOrder =
			{ 'i', 'c', 'b', 'a' };

		if (!session.is_object())
			throw std::invalid_argument("given session is not a JSON object");

		// Ensure certain properties exist.

		if (session.find("version") == session.end())
			session["version"] = 0;

		if (session.find("name") == session.end())
			session["name"] = "-";

		if (session.find("media") == session.end())
			session["media"] = json::array();

		for (auto& mLine : session.at("media"))
		{
			if (mLine.find("payloads") == mLine.end())
				mLine["payloads"] = "";
		}

		std::stringstream sdpstream;

		// Loop through OuterOrder for matching properties on session.
		for (auto type : OuterOrder)
		{
			for (auto& rule : grammar::mapRules.at(type))
			{
				if (
					!rule.name.empty() &&
					session.find(rule.name) != session.end() &&
					!session[rule.name].is_null()
				)
				{
					sdpstream << makeLine(type, rule, session);
				}
				else if (
					!rule.push.empty() &&
					session.find(rule.push) != session.end() &&
					session[rule.push].is_array()
				)
				{
					for (auto& el : session.at(rule.push))
					{
						sdpstream << makeLine(type, rule, el);
					}
				}
			}
		}

		// Then for each media line, follow the InnerOrder.
		for (auto& mLine : session.at("media"))
		{
			for (auto type : InnerOrder)
			{
				for (auto& rule : grammar::mapRules.at(type))
				{
					if (
						!rule.name.empty() &&
						mLine.find(rule.name) != mLine.end() &&
						!mLine[rule.name].is_null()
					)
					{
						sdpstream << makeLine(type, rule, mLine);
					}
					else if (
						!rule.push.empty() &&
						mLine.find(rule.push) != mLine.end() &&
						mLine[rule.push].is_array()
					)
					{
						for (auto& el : mLine.at(rule.push))
						{
							sdpstream << makeLine(type, rule, el);
						}
					}
				}
			}
		}

		return sdpstream.str();
	}

	const std::string makeLine(char type, const grammar::Rule& rule, const json& location)
	{
		std::string str = rule.format.empty() ?
			(rule.formatFunc(!rule.push.empty() ? location : location.at(rule.name))) :
			rule.format;

		const std::string line = std::string(1, type) + "=" + str;
		std::vector<json> args;

		if (!rule.names.empty())
		{
			for (auto& name : rule.names)
			{
				if (!rule.name.empty())
					args.push_back(location.at(rule.name).at(name));
				else // For mLine and push attributes.
					args.push_back(location.at(name));
			}
		}
		else
		{
			args.push_back(location.at(rule.name));
		}

		return format(line, args);
	}

	const std::string format(const std::string& line, std::vector<json>& args)
	{
		// for (auto& arg : args)
		// {
		// 	std::cout << arg << std::endl;
		// }






		return line + "\r\n";
	}
}

#include "sdptransform.hpp"
#include <cstddef>   // size_t
#include <sstream>   // std::stringstream
#include <stdexcept>

namespace sdptransform
{
	void makeLine(
		std::stringstream& sdpstream,
		char type,
		const grammar::Rule& rule,
		const json& location
	);

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
			for (auto& rule : grammar::rulesMap.at(type))
			{
				if (
					!rule.name.empty() &&
					session.find(rule.name) != session.end() &&
					!session[rule.name].is_null()
				)
				{
					makeLine(sdpstream, type, rule, session);
				}
				else if (
					!rule.push.empty() &&
					session.find(rule.push) != session.end() &&
					session[rule.push].is_array()
				)
				{
					for (auto& el : session.at(rule.push))
					{
						makeLine(sdpstream, type, rule, el);
					}
				}
			}
		}

		// Then for each media line, follow the InnerOrder.
		for (auto& mLine : session.at("media"))
		{
			makeLine(sdpstream, 'm', grammar::rulesMap.at('m')[0], mLine);

			for (auto type : InnerOrder)
			{
				for (auto& rule : grammar::rulesMap.at(type))
				{
					if (
						!rule.name.empty() &&
						mLine.find(rule.name) != mLine.end() &&
						!mLine[rule.name].is_null()
					)
					{
						makeLine(sdpstream, type, rule, mLine);
					}
					else if (
						!rule.push.empty() &&
						mLine.find(rule.push) != mLine.end() &&
						mLine[rule.push].is_array()
					)
					{
						for (auto& el : mLine.at(rule.push))
						{
							makeLine(sdpstream, type, rule, el);
						}
					}
				}
			}
		}

		return sdpstream.str();
	}

	void makeLine(
		std::stringstream& sdpstream,
		char type,
		const grammar::Rule& rule,
		const json& location
	)
	{
		static const std::regex FormatRegex("%[sdv%]");

		const std::string format = rule.format.empty()
			? (rule.formatFunc(!rule.push.empty() ? location : location.at(rule.name)))
			: rule.format;

		std::vector<json> args;

		if (!rule.names.empty())
		{
			for (auto& name : rule.names)
			{
				if (
					!rule.name.empty() &&
					location.find(rule.name) != location.end() &&
					location.at(rule.name).find(name) != location.at(rule.name).end()
				)
				{
					args.push_back(location.at(rule.name).at(name));
				}
				// For mLine and push attributes.
				else if (location.find(name) != location.end())
				{
					args.push_back(location.at(name));
				}
				// NOTE: Otherwise ensure an empty value is inserted into args array.
				else
				{
					args.push_back("");
				}
			}
		}
		else if (location.find(rule.name) != location.end())
		{
			args.push_back(location.at(rule.name));
		}

		std::stringstream linestream;
		size_t i = 0;
		size_t len = args.size();

		linestream << type << "=";

		for(
			auto it = std::sregex_iterator(format.begin(), format.end(), FormatRegex);
			it != std::sregex_iterator();
		  ++it
		 )
		{
			const std::smatch& match = *it;
			const std::string& str = match.str();

			if (i >= len)
			{
				linestream << str;
			}
			else
			{
				auto& arg = args[i];
				i++;

				linestream << match.prefix();

				if (str == "%%")
				{
					linestream << "%";
				}
				else if (str == "%s" || str == "%d")
				{
					if (arg.is_string())
						linestream << arg.get<std::string>();
					else
						linestream << arg;
				}
				else if (str == "%v")
				{
					// Do nothing.
				}
			}
		}

		linestream << "\r\n";

		sdpstream << linestream.str();
	}
}

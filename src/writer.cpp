#include "sdptransform.hpp"
#include <cstddef>   // size_t
#include <sstream>   // std::stringstream
#include <stdexcept>
#include "string_view.hpp"

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
				json::const_iterator it;

				if (
					!rule.name.empty() &&
					(it = session.find(rule.name)) != session.end() &&
					!it->is_null()
				)
				{
					makeLine(sdpstream, type, rule, session);
				}
				else if (
					!rule.push.empty() &&
					(it = session.find(rule.push)) != session.end() &&
					it->is_array()
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
					json::const_iterator it;

					if (
						!rule.name.empty() &&
						(it = mLine.find(rule.name)) != mLine.end() &&
						!it->is_null()
					)
					{
						makeLine(sdpstream, type, rule, mLine);
					}
					else if (
						!rule.push.empty() &&
						(it = mLine.find(rule.push)) != mLine.end() &&
						it->is_array()
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
		const std::string format = rule.format.empty()
			? rule.formatFunc(
					!rule.push.empty()
						? location
						: !rule.name.empty()
							? location.at(rule.name)
							: location)
			: rule.format;

		std::vector<json> args;
		auto it = location.find(rule.name);

		if (!rule.names.empty())
		{
			for (auto& name : rule.names)
			{
				json::const_iterator it;

				if (
					!rule.name.empty() &&
					(it = location.find(rule.name)) != location.end() &&
					it->find(name) != it->end()
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
		else if (it != location.end())
		{
			args.push_back(*it);
		}

		std::stringstream linestream;
		size_t i = 0;
		size_t len = args.size();

		linestream << type << "=";

		string_view runningFormat(format);
		size_t nextPos = 0U;
		for (
			auto pos = runningFormat.find('%'); pos != std::string::npos; pos = runningFormat.find('%', nextPos))
		{
			const char nextChar = (pos + 1 < runningFormat.size()) ? runningFormat[pos + 1] : '0';
			const string_view prefix = runningFormat.substr(0, pos);

			if (nextChar != '%' && nextChar != 'v' && nextChar != 'd' && nextChar != 's')
			{
				nextPos = pos + 2;
				continue;
			}

			if (i >= len)
			{
				linestream << prefix << '%' << nextChar;
			}
			else
			{
				auto &arg = args[i];
				i++;

				linestream << prefix;

				if (nextChar == '%')
				{
					linestream << "%";
				}
				else if (nextChar == 's' || nextChar == 'd')
				{
					if (arg.is_string())
						linestream << arg.get<std::string>();
					else
						linestream << arg;
				}
				else if (nextChar == 'v')
				{
					// Do nothing.
				}
			}

			runningFormat = runningFormat.substr(pos + 2);
			nextPos = 0;
		}
		if (!runningFormat.empty())
		{
			linestream << runningFormat;
		}

		linestream << "\r\n";

		sdpstream << linestream.str();
	}
}

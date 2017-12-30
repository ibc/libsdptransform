#include "sdptransform.hpp"
#include <iostream>
#include <sstream>
#include <regex>

using json = nlohmann::json;

namespace sdptransform
{
	bool parse(std::string &sdp, json &session)
	{
		static const std::regex RegexSdpValidLine("^([a-z])=(.*)");

		std::stringstream sdpstream(sdp);
		std::string line;
		// json session = json::object();
		json media = json::array();
		json location = session;

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
				location = media[media.size() - 1]; // Point at latest media line.
			}
		}

		session["media"] = media; // Link it up.

		std::cout << session.dump(2) << std::endl;

		return true;
	}
}

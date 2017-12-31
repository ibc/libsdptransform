#ifndef SDPTRANSFORM_HPP
#define SDPTRANSFORM_HPP

#include "json.hpp"
#include <string>

namespace sdptransform
{
	inline nlohmann::json getEmptySession()
	{
		nlohmann::json session{ nlohmann::json::object() };

		return session;
	}

	void parse(std::string& sdp, nlohmann::json& session);

	std::string write(nlohmann::json& session);
}

#endif

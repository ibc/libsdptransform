#ifndef SDPTRANSFORM_HPP
#define SDPTRANSFORM_HPP

#include "json.hpp"
#include <string>

using json = nlohmann::json;

namespace sdptransform
{
	bool parse(std::string& sdp, json& session);
}

#endif

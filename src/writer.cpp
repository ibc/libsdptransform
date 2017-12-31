#include "sdptransform.hpp"
#include "Grammar.hpp"

using json = nlohmann::json;

namespace sdptransform
{
	std::string write(json& session)
	{
		if (!session.is_object())
			throw std::invalid_argument("given session is not a JSON object");

		return "TODO";
	}
}

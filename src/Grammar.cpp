#include "Grammar.hpp"
#include <iostream> // TODO: TMP

namespace sdptransform
{
	std::map<char, Grammar::Item> Grammar::items =
	{
		{
			'v',
			{
				"version",
				std::regex("^(\\d*)$")
			}
		}
	};
}

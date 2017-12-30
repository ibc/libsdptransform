#ifndef SDPTRANSFORM_GRAMMAR_HPP
#define SDPTRANSFORM_GRAMMAR_HPP

#include <string>
#include <map>
#include <regex>

namespace sdptransform
{
	class Grammar
	{
	public:
		struct Item
		{
			std::string name;
			std::regex reg;
		};

	public:
		static std::map<char, Item> items;
	};
}

#endif

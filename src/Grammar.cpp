#include "Grammar.hpp"
#include <iostream> // TODO: TMP

namespace sdptransform
{
	std::map<char, std::vector<Grammar::Rule>> Grammar::mapRules =
	{
		{
			'v',
			{
				// v=0
				{
					// name:
					"version",
					// push:
					"",
					// reg:
					std::regex("^(\\d*)$"),
					// names:
					{ },
					// format:
					"%s"
				}
			}
		},

		{
			'o',
			{
				// o=- 20518 0 IN IP4 203.0.113.1
				{
					// name:
					"origin",
					// push:
					"",
					// reg:
					std::regex("^(\\S*) (\\d*) (\\d*) (\\S*) IP(\\d) (\\S*)"),
					// names:
					{ "username", "sessionId", "sessionVersion", "netType", "ipVer", "address" },
					// format:
					"%s %s %d %s IP%d %s"
				}
			}
		},

		{
			's',
			{
				// s=-
				{
					// name:
					"name",
					// push:
					"",
					// reg:
					std::regex("(.*)"),
					// names:
					{ },
					// format:
					"%s"
				}
			}
		},

		{
			'i',
			{
				// i=foo
				{
					// name:
					"description",
					// push:
					"",
					// reg:
					std::regex("(.*)"),
					// names:
					{ },
					// format:
					"%s"
				}
			}
		},

		{
			'u',
			{
				// u=https://foo.com
				{
					// name:
					"uri",
					// push:
					"",
					// reg:
					std::regex("(.*)"),
					// names:
					{ },
					// format:
					"%s"
				}
			}
		},

		{
			'e',
			{
				// e=alice@foo.com
				{
					// name:
					"email",
					// push:
					"",
					// reg:
					std::regex("(.*)"),
					// names:
					{ },
					// format:
					"%s"
				}
			}
		},

		{
			'p',
			{
				// p=+12345678
				{
					// name:
					"phone",
					// push:
					"",
					// reg:
					std::regex("(.*)"),
					// names:
					{ },
					// format:
					"%s"
				}
			}
		},

		{
			'z',
			{
				{
					// name:
					"timezones",
					// push:
					"",
					// reg:
					std::regex("(.*)"),
					// names:
					{ },
					// format:
					"%s"
				}
			}
		},

		{
			'r',
			{
				{
					// name:
					"repeats",
					// push:
					"",
					// reg:
					std::regex("(.*)"),
					// names:
					{ },
					// format:
					"%s"
				}
			}
		},

		{
			't',
			{
				// t=0 0
				{
					// name:
					"timing",
					// push:
					"",
					// reg:
					std::regex("^(\\d*) (\\d*)"),
					// names:
					{ "start", "stop" },
					// format:
					"%d %d"
				}
			}
		},

		{
			'c',
			{
				// c=IN IP4 10.47.197.26
				{
					// name:
					"connection",
					// push:
					"",
					// reg:
					std::regex("^IN IP(\\d) (\\S*)"),
					// names:
					{ "version", "ip" },
					// format:
					"IN IP%d %s"
				}
			}
		},

		{
			'b',
			{
				// b=AS:4000
				{
					// name:
					"",
					// push:
					"bandwidth",
					// reg:
					std::regex("^(TIAS|AS|CT|RR|RS):(\\d*)"),
					// names:
					{ "type", "limit" },
					// format:
					"%s:%s"
				}
			}
		},

		{
			'm',
			{
				// m=video 51744 RTP/AVP 126 97 98 34 31
				{
					// name:
					"",
					// push:
					"",
					// reg:
					std::regex("^(\\w*) (\\d*) ([\\w\\/]*)(?: (.*))?"),
					// names:
					{ "type", "port", "protocol", "payloads" },
					// format:
					"%s %d %s %s"
				}
			}
		},

		{
			'a',
			{
				// a=ssrc:2566107569 cname:t9YU8M1UxTF8Y1A1
				{
					// name:
					"",
					// push:
					"ssrcs",
					// reg:
					std::regex("^ssrc:(\\d*) ([\\w_]*)(?::(.*))?"),
					// names:
					{ "id", "attribute", "value" },
					// format:
					"",
					// formatFunc:
					[](int x)
					{
						return std::to_string(x);
					}
				}
			}
		}
	};

	void Grammar::dump()
	{
		std::cout << "--- Grammar::mapRules:" << std::endl;
		for (auto& kv : Grammar::mapRules)
		{
			auto type = kv.first;
			auto& rules = kv.second;

			std::cout << "- " << type << std::endl;

			for (auto& rule : rules)
			{
				if (!rule.name.empty())
					std::cout << "  - name: " << rule.name << std::endl;
				else
					std::cout << "  - push: " << rule.push << std::endl;

				std::cout << "  - names:";
				for (auto& name : rule.names)
				{
					std::cout << " " << name;
				}
				std::cout << std::endl;

				if (!rule.format.empty())
					std::cout << "  - format: " << rule.format << std::endl;
				else if (rule.formatFunc)
					std::cout << "  - formatFunc(2): " << rule.formatFunc(2) << std::endl;
			}
		}
	}
}

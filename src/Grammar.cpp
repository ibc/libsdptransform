#include "Grammar.hpp"

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
				// a=rtpmap:110 opus/48000/2
				{
					// name:
					"",
					// push:
					"rtp",
					// reg:
					std::regex("^rtpmap:(\\d*) ([\\w\\-\\.]*)(?:\\s*\\/(\\d*)(?:\\s*\\/(\\S*))?)?"),
					// names:
					{ "payload", "codec", "rate", "encoding" },
					// format:
					"",
					// formatFunc:
					[](json& o)
					{
						return "TODO";
					}
				},

				// a=fmtp:108 profile-level-id=24;object=23;bitrate=64000
				// a=fmtp:111 minptime=10; useinbandfec=1
				{
					// name:
					"",
					// push:
					"fmtp",
					// reg:
					std::regex("^fmtp:(\\d*) ([\\S| ]*)"),
					// names:
					{ "payload", "config" },
					// format:
					"fmtp:%d %s"
				},

				// a=control:streamid=0
				{
					// name:
					"control",
					// push:
					"",
					// reg:
					std::regex("^control:(.*)"),
					// names:
					{ },
					// format:
					"control:%s"
				},

				// a=rtcp:65179 IN IP4 193.84.77.194
				{
					// name:
					"rtcp",
					// push:
					"",
					// reg:
					std::regex("^rtcp:(\\d*)(?: (\\S*) IP(\\d) (\\S*))?"),
					// names:
					{ "port", "netType", "ipVer", "address" },
					// format:
					"",
					// formatFunc:
					[](json& o)
					{
						return "TODO";
					}
				},

				// a=rtcp-fb:98 trr-int 100
				{
					// name:
					"",
					// push:
					"rtcpFbTrrInt",
					// reg:
					std::regex("^rtcp-fb:(\\*|\\d*) trr-int (\\d*)"),
					// names:
					{ "payload", "value" },
					// format:
					"rtcp-fb:%d trr-int %d"
				},

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
					[](json& o)
					{
						return "TODO";
					}
				}
			}
		}
	};
}

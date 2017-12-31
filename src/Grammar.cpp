#include "Grammar.hpp"

using json = nlohmann::json;

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

				// a=rtcp-fb:98 nack rpsi
				{
					// name:
					"",
					// push:
					"rtcpFb",
					// reg:
					std::regex("^rtcp-fb:(\\*|\\d*) ([\\w\\-_]*)(?: ([\\w\\-_]*))?"),
					// names:
					{ "payload", "type", "subtype" },
					// format:
					"",
					// formatFunc:
					[](json& o)
					{
						return "TODO";
					}
				},

				// a=extmap:2 urn:ietf:params:rtp-hdrext:toffset
				// a=extmap:1/recvonly URI-gps-string
				{
					// name:
					"",
					// push:
					"ext",
					// reg:
					std::regex("^extmap:(\\d+)(?:\\/(\\w+))? (\\S*)(?: (\\S*))?"),
					// names:
					{ "value", "direction", "uri", "config" },
					// format:
					"",
					// formatFunc:
					[](json& o)
					{
						return "TODO";
					}
				},

				// a=crypto:1 AES_CM_128_HMAC_SHA1_80 inline:PS1uQCVeeCFCanVmcjkpPywjNWhcYD0mXXtxaVBR|2^20|1:32
				{
					// name:
					"",
					// push:
					"crypto",
					// reg:
					std::regex("^crypto:(\\d*) ([\\w_]*) (\\S*)(?: (\\S*))?"),
					// names:
					{ "id", "suite", "config", "sessionConfig" },
					// format:
					"",
					// formatFunc:
					[](json& o)
					{
						return "TODO";
					}
				},

				// a=setup:actpass
				{
					// name:
					"setup",
					// push:
					"",
					// reg:
					std::regex("^setup:(\\w*)"),
					// names:
					{ },
					// format:
					"setup:%s"
				},

				// a=mid:1
				{
					// name:
					"mid",
					// push:
					"",
					// reg:
					std::regex("^mid:([^\\s]*)"),
					// names:
					{ },
					// format:
					"mid:%s"
				},

				// a=msid:0c8b064d-d807-43b4-b434-f92a889d8587 98178685-d409-46e0-8e16-7ef0db0db64a
				{
					// name:
					"msid",
					// push:
					"",
					// reg:
					std::regex("^msid:(.*)"),
					// names:
					{ },
					// format:
					"msid:%s"
				},

				// a=ptime:20
				{
					// name:
					"ptime",
					// push:
					"",
					// reg:
					std::regex("^ptime:(\\d*)"),
					// names:
					{ },
					// format:
					"ptime:%d"
				},

				// a=maxptime:60
				{
					// name:
					"maxptime",
					// push:
					"",
					// reg:
					std::regex("^maxptime:(\\d*)"),
					// names:
					{ },
					// format:
					"maxptime:%d"
				},

				// a=sendrecv
				{
					// name:
					"direction",
					// push:
					"",
					// reg:
					std::regex("^(sendrecv|recvonly|sendonly|inactive)"),
					// names:
					{ },
					// format:
					"%s"
				},

				// a=ice-lite
				{
					// name:
					"icelite",
					// push:
					"",
					// reg:
					std::regex("^(ice-lite)"),
					// names:
					{ },
					// format:
					"%s"
				},

				// a=ice-ufrag:F7gI
				{
					// name:
					"iceUfrag",
					// push:
					"",
					// reg:
					std::regex("^ice-ufrag:(\\S*)"),
					// names:
					{ },
					// format:
					"ice-ufrag:%s"
				},

				// a=fingerprint:SHA-1 00:11:22:33:44:55:66:77:88:99:AA:BB:CC:DD:EE:FF:00:11:22:33
				{
					// name:
					"fingerprint",
					// push:
					"",
					// reg:
					std::regex("^fingerprint:(\\S*) (\\S*)"),
					// names:
					{ "type", "hash" },
					// format:
					"fingerprint:%s %s"
				},

				// a=candidate:0 1 UDP 2113667327 203.0.113.1 54400 typ host
				// a=candidate:1162875081 1 udp 2113937151 192.168.34.75 60017 typ host generation 0 network-id 3 network-cost 10
				// a=candidate:3289912957 2 udp 1845501695 193.84.77.194 60017 typ srflx raddr 192.168.34.75 rport 60017 generation 0 network-id 3 network-cost 10
				// a=candidate:229815620 1 tcp 1518280447 192.168.150.19 60017 typ host tcptype active generation 0 network-id 3 network-cost 10
				// a=candidate:3289912957 2 tcp 1845501695 193.84.77.194 60017 typ srflx raddr 192.168.34.75 rport 60017 tcptype passive generation 0 network-id 3 network-cost 10
				{
					// name:
					"",
					// push:
					"candidates",
					// reg:
					std::regex("^candidate:(\\S*) (\\d*) (\\S*) (\\d*) (\\S*) (\\d*) typ (\\S*)(?: raddr (\\S*) rport (\\d*))?(?: tcptype (\\S*))?(?: generation (\\d*))?(?: network-id (\\d*))?(?: network-cost (\\d*))?"),
					// names:
					{ "foundation", "component", "transport", "priority", "ip", "port", "type", "raddr", "rport", "tcptype", "generation", "network-id", "network-cost" },
					// format:
					"",
					// formatFunc:
					[](json& o)
					{
						return "TODO";
					}
				},

				// a=end-of-candidates
				{
					// name:
					"endOfCandidates",
					// push:
					"",
					// reg:
					std::regex("^remote-candidates:(.*)"),
					// names:
					{ },
					// format:
					"remote-candidates:%s"
				},

				// a=ice-options:google-ice
				{
					// name:
					"iceOptions",
					// push:
					"",
					// reg:
					std::regex("^ice-options:(\\S*)"),
					// names:
					{ },
					// format:
					"ice-options:%s"
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
				},

				// a=ssrc-group:FEC 1 2
				// a=ssrc-group:FEC-FR 3004364195 1080772241
				{
					// name:
					"",
					// push:
					"ssrcGroups",
					// reg:
					std::regex("^ssrc-group:([\x21\x23\x24\x25\x26\x27\x2A\x2B\x2D\x2E\\w]*) (.*)"),
					// names:
					{ "semantics", "ssrcs" },
					// format:
					"ssrc-group:%s %s"
				},
			}
		}
	};
}

#include "catch.hpp"
#include "helpers.hpp"
#include "sdptransform.hpp"

#include <iostream>

SCENARIO("normalSdp", "[parse]")
{
	auto sdp = helpers::readFile("test/data/normal.sdp");
	auto session = sdptransform::parse(sdp);

	REQUIRE(session.size() > 0);
	REQUIRE(session.find("media") != session.end());

	auto& media = session.at("media");

	REQUIRE(session.at("origin").at("username") == "-");
	REQUIRE(session.at("origin").at("sessionId") == 20518);
	REQUIRE(session.at("origin").at("sessionVersion") == 0);
	REQUIRE(session.at("origin").at("netType") == "IN");
	REQUIRE(session.at("origin").at("ipVer") == 4);
	REQUIRE(session.at("origin").at("address") == "203.0.113.1");

	REQUIRE(session.at("connection").at("ip") == "203.0.113.1");
	REQUIRE(session.at("connection").at("version") == 4);

	// Global ICE and fingerprint.
	REQUIRE(session.at("iceUfrag") == "F7gI");
	REQUIRE(session.at("icePwd") == "x9cml/YzichV2+XlhiMu8g");

	auto& audio = media[0];

	REQUIRE(audio.at("type") == "audio");
	REQUIRE(audio.at("port") == 54400);
	REQUIRE(audio.at("protocol") == "RTP/SAVPF");
	REQUIRE(audio.at("direction") == "sendrecv");
	REQUIRE(audio.at("rtp")[0].at("payload") == 0);
	REQUIRE(audio.at("rtp")[0].at("codec") == "PCMU");
	REQUIRE(audio.at("rtp")[0].at("rate") == 8000);
	REQUIRE(audio.at("rtp")[1].at("payload") == 96);
	REQUIRE(audio.at("rtp")[1].at("codec") == "opus");
	REQUIRE(audio.at("rtp")[1].at("rate") == 48000);
	REQUIRE(
		audio.at("ext")[0] ==
		"{ \"value\": 1, \"uri\": \"URI-toffset\" }"_json
	);
	REQUIRE(
		audio.at("ext")[1] ==
		"{ \"value\": 2, \"direction\": \"recvonly\", \"uri\": \"URI-gps-string\" }"_json
	);

	auto& video = media[1];

	REQUIRE(video.at("type") == "video");
	REQUIRE(video.at("port") == 55400);
	REQUIRE(video.at("protocol") == "RTP/SAVPF");
	REQUIRE(video.at("direction") == "sendrecv");
	REQUIRE(video.at("rtp")[0].at("payload") == 97);
	REQUIRE(video.at("rtp")[0].at("codec") == "H264");
	REQUIRE(video.at("rtp")[0].at("rate") == 90000);
	REQUIRE(video.at("fmtp")[0].at("payload") == 97);

	// TODO:
	// auto vidFmtp = sdptransform::parseFmtpConfig(video.at("fmtp")[0].at("config"));

	// REQUIRE(vidFmtp.at("profile-level-id") == "4d0028");
	// REQUIRE(vidFmtp.at("packetization-mode") == 1);
	// REQUIRE(vidFmtp.at("sprop-parameter-sets") == "Z0IAH5WoFAFuQA==,aM48gA==");

	REQUIRE(video.at("fmtp")[1].at("payload") == 98);

	// TODO:
	// auto vidFmtp2 = sdptransform::parseFmtpConfig(video.at("fmtp")[1].at("config"));

	// REQUIRE(vidFmtp2.at("minptime") == 10);
	// REQUIRE(vidFmtp2.at("useinbandfec") == 1);

	REQUIRE(video.at("rtp")[1].at("payload") == 98);
	REQUIRE(video.at("rtp")[1].at("codec") == "VP8");
	REQUIRE(video.at("rtp")[1].at("rate") == 90000);
	REQUIRE(video.at("rtcpFb")[0].at("payload") == "*");
	REQUIRE(video.at("rtcpFb")[0].at("type") == "nack");
	REQUIRE(video.at("rtcpFb")[1].at("payload") == 98);
	REQUIRE(video.at("rtcpFb")[1].at("type") == "nack");
	REQUIRE(video.at("rtcpFb")[1].at("subtype") == "rpsi");
	REQUIRE(video.at("rtcpFbTrrInt")[0].at("payload") == 98);
	REQUIRE(video.at("rtcpFbTrrInt")[0].at("value") == 100);
	REQUIRE(video.at("crypto")[0].at("id") == 1);
	REQUIRE(video.at("crypto")[0].at("suite") == "AES_CM_128_HMAC_SHA1_32");
	REQUIRE(video.at("crypto")[0].at("config") == "inline:keNcG3HezSNID7LmfDa9J4lfdUL8W1F7TNJKcbuy|2^20|1:32");
	REQUIRE(video.at("ssrcs").size() == 2);
	REQUIRE(
		video.at("ssrcs")[0] ==
		"{ \"id\": 1399694169, \"attribute\": \"foo\", \"value\": \"bar\" }"_json
	);
	REQUIRE(
		video.at("ssrcs")[1] ==
		"{ \"id\": 1399694169, \"attribute\": \"baz\" }"_json
	);

	auto& cs = audio.at("candidates");

	REQUIRE(cs.size() == 4);
	REQUIRE(cs[0].at("foundation") == 0);
	REQUIRE(cs[0].at("component") == 1);
	REQUIRE(cs[0].at("transport") == "UDP");
	REQUIRE(cs[0].at("priority") == 2113667327);
	REQUIRE(cs[0].at("ip") == "203.0.113.1");
	REQUIRE(cs[0].at("port") == 54400);
	REQUIRE(cs[0].at("type") == "host");
	REQUIRE(cs[1].at("foundation") == 1);
	REQUIRE(cs[1].at("component") == 2);
	REQUIRE(cs[1].at("transport") == "UDP");
	REQUIRE(cs[1].at("priority") == 2113667326);
	REQUIRE(cs[1].at("ip") == "203.0.113.1");
	REQUIRE(cs[1].at("port") == 54401);
	REQUIRE(cs[1].at("type") == "host");
	REQUIRE(cs[2].at("foundation") == 2);
	REQUIRE(cs[2].at("component") == 1);
	REQUIRE(cs[2].at("transport") == "UDP");
	REQUIRE(cs[2].at("priority") == 1686052607);
	REQUIRE(cs[2].at("ip") == "203.0.113.1");
	REQUIRE(cs[2].at("port") == 54402);
	REQUIRE(cs[2].at("type") == "srflx");
	REQUIRE(cs[2].at("raddr") == "192.168.1.145");
	REQUIRE(cs[2].at("rport") == 54402);
	REQUIRE(cs[2].at("generation") == 0);
	REQUIRE(cs[2].at("network-id") == 3);
	REQUIRE(cs[2].at("network-cost") == 10);
	REQUIRE(cs[3].at("foundation") == 3);
	REQUIRE(cs[3].at("component") == 2);
	REQUIRE(cs[3].at("transport") == "UDP");
	REQUIRE(cs[3].at("priority") == 1686052606);
	REQUIRE(cs[3].at("ip") == "203.0.113.1");
	REQUIRE(cs[3].at("port") == 54403);
	REQUIRE(cs[3].at("type") == "srflx");
	REQUIRE(cs[3].at("raddr") == "192.168.1.145");
	REQUIRE(cs[3].at("rport") == 54403);
	REQUIRE(cs[3].at("generation") == 0);
	REQUIRE(cs[3].at("network-id") == 3);
	REQUIRE(cs[3].at("network-cost") == 10);

	auto& cs2 = video.at("candidates");

	REQUIRE(cs2[2].find("network-cost") == cs2[2].end());
	REQUIRE(cs2[3].find("network-cost") == cs2[3].end());

	REQUIRE(media.size() == 2);

	auto newSdp = sdptransform::write(session);

	REQUIRE(newSdp == sdp);
}

SCENARIO("hackySdp", "[parse]")
{
	auto sdp = helpers::readFile("test/data/hacky.sdp");
	auto session = sdptransform::parse(sdp);

	REQUIRE(session.size() > 0);
	REQUIRE(session.find("media") != session.end());

	auto& media = session.at("media");

	REQUIRE(session.at("origin").at("sessionId") == 3710604898417546434);
	REQUIRE(session.at("groups").size() == 1);
	REQUIRE(session.at("groups")[0].at("type") == "BUNDLE");
	REQUIRE(session.at("groups")[0].at("mids") == "audio video");
	REQUIRE(session.at("msidSemantic").at("semantic") == "WMS");
	REQUIRE(session.at("msidSemantic").at("token") == "Jvlam5X3SX1OP6pn20zWogvaKJz5Hjf9OnlV");

	// Verify a=rtcp:65179 IN IP4 193.84.77.194.
	REQUIRE(media[0].at("rtcp").at("port") == 1);
	REQUIRE(media[0].at("rtcp").at("netType") == "IN");
	REQUIRE(media[0].at("rtcp").at("ipVer") == 4);
	REQUIRE(media[0].at("rtcp").at("address") == "0.0.0.0");

	// Verify ICE TCP types.
	REQUIRE(
		media[0].at("candidates")[0].find("tcptype") == media[0].at("candidates")[0].end()
	);
	REQUIRE(media[0].at("candidates")[1].at("tcptype") == "active");
	REQUIRE(media[0].at("candidates")[1].at("transport") == "tcp");
	REQUIRE(media[0].at("candidates")[1].at("generation") == 0);
	REQUIRE(media[0].at("candidates")[1].at("type") == "host");
	REQUIRE(
		media[0].at("candidates")[2].find("generation") == media[0].at("candidates")[2].end()
	);
	REQUIRE(media[0].at("candidates")[2].at("type") == "host");
	REQUIRE(media[0].at("candidates")[2].at("tcptype") == "active");
	REQUIRE(media[0].at("candidates")[3].at("tcptype") == "passive");
	REQUIRE(media[0].at("candidates")[4].at("tcptype") == "so");
	// raddr + rport + tcptype + generation.
	REQUIRE(media[0].at("candidates")[5].at("type") == "srflx");
	REQUIRE(media[0].at("candidates")[5].at("rport") == 9);
	REQUIRE(media[0].at("candidates")[5].at("raddr") == "10.0.1.1");
	REQUIRE(media[0].at("candidates")[5].at("tcptype") == "active");
	REQUIRE(media[0].at("candidates")[6].at("tcptype") == "passive");
	REQUIRE(media[0].at("candidates")[6].at("rport") == 8998);
	REQUIRE(media[0].at("candidates")[6].at("raddr") == "10.0.1.1");
	REQUIRE(media[0].at("candidates")[6].at("generation") == 5);

	// And verify it works without specifying the IP.
	REQUIRE(media[1].at("rtcp").at("port") == 12312);
	REQUIRE(media[1].at("rtcp").find("netType") == media[1].at("rtcp").end());
	REQUIRE(media[1].at("rtcp").find("ipVer") == media[1].at("rtcp").end());
	REQUIRE(media[1].at("rtcp").find("address") == media[1].at("rtcp").end());

	// Verify a=rtpmap:126 telephone-event/8000.
	auto lastRtp = media[0].at("rtp").size() - 1;

	REQUIRE(media[0].at("rtp")[lastRtp].at("codec") == "telephone-event");
	REQUIRE(media[0].at("rtp")[lastRtp].at("rate") == 8000);

	REQUIRE(media[0].at("iceOptions") == "google-ice");
	REQUIRE(media[0].at("maxptime") == 60);
	REQUIRE(media[0].at("rtcpMux") == "rtcp-mux");

	REQUIRE(media[0].at("rtp")[0].at("codec") == "opus");
	REQUIRE(media[0].at("rtp")[0].at("encoding") == 2);

	REQUIRE(media[0].at("ssrcs").size() == 4);

	auto& ssrcs = media[0].at("ssrcs");

	REQUIRE(
		ssrcs[0] ==
		"{ \"id\": 2754920552, \"attribute\": \"cname\", \"value\": \"t9YU8M1UxTF8Y1A1\" }"_json
	);
	REQUIRE(
		ssrcs[1] ==
		"{ \"id\": 2754920552, \"attribute\": \"msid\", \"value\": \"Jvlam5X3SX1OP6pn20zWogvaKJz5Hjf9OnlV Jvlam5X3SX1OP6pn20zWogvaKJz5Hjf9OnlVa0\" }"_json
	);
	REQUIRE(
		ssrcs[2] ==
		"{ \"id\": 2754920552, \"attribute\": \"mslabel\", \"value\": \"Jvlam5X3SX1OP6pn20zWogvaKJz5Hjf9OnlV\" }"_json
	);
	REQUIRE(
		ssrcs[3] ==
		"{ \"id\": 2754920552, \"attribute\": \"label\", \"value\": \"Jvlam5X3SX1OP6pn20zWogvaKJz5Hjf9OnlVa0\" }"_json
	);

	// Verify a=sctpmap:5000 webrtc-datachannel 1024.
	REQUIRE(media[2].find("sctpmap") != media[2].end());
	REQUIRE(media[2].at("sctpmap").at("sctpmapNumber") == 5000);
	REQUIRE(media[2].at("sctpmap").at("app") == "webrtc-datachannel");
	REQUIRE(media[2].at("sctpmap").at("maxMessageSize") == 1024);

	// Verify a=framerate:29.97.
	REQUIRE(media[1].at("framerate") == 1234);
	REQUIRE(media[2].at("framerate") == double{ 29.97 });
}

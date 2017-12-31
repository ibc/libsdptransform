#include "sdptransform.hpp"
#include <string>
#include <iostream>
#include <fstream>   // std::ifstream
#include <streambuf> // std::istreambuf_iterator

int main(int argc, char* argv[])
{
	std::ifstream t("test/data/normal.sdp");
	std::string sdp;

	t.seekg(0, std::ios::end);
	sdp.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	sdp.assign(
		(std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	auto session = sdptransform::getEmptySession();

	sdptransform::parse(sdp, session);

	// std::cout << session.dump(2) << std::endl;

	auto newSdp = sdptransform::write(session);

	std::cout << "sdptransform::write(session):\n----\n" << newSdp << "----\n";

	return 0;
}

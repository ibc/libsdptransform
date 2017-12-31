#include "sdptransform.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>

using json = nlohmann::json;

int main(void)
{
	std::ifstream t("test/data/normal.sdp");
	std::string sdp;

	t.seekg(0, std::ios::end);
	sdp.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	sdp.assign(
		(std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	json session = nlohmann::json::object();
	bool ret = sdptransform::parse(sdp, session);

	// std::cout << "ret: " << ret << std::endl;
	std::cout << session.dump(2) << std::endl;

	std::cout << "DONE" << std::endl;

	return 0;
}

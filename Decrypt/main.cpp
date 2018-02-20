#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const std::string& salt1 = "LM::TB::BB";
const std::string& salt2 = "_:/_77";
const std::string& salt3 = "line=wowC++";
const std::string &base64_codes = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string decodeBase64(const std::string &);
std::string decryptBase64(std::string& s);

int main(int argc, char** argv) {
	if (argc != 3)
		return std::cout << "Program needs two arguments" << std::endl, 2;

	std::string in(argv[1]), out(argv[2]);
	std::ifstream fi(in);
	if(!fi)
		return std::cout << "Cannot read input file '" << in << "'" << std::endl, 3;

	std::string data;
	fi >> data;
	if(!fi)
		return std::cout << "Input file '" << in << "' corrupted" << std::endl, 4;

	/*decrypt*/
	data = decryptBase64(data);
	std::ofstream fo(out);
	if(!fo)
		return std::cout << "Cannot write output file '" << out << "'" << std::endl, 5;
	fo << data;
	std::cout << "Decoding was successful" << std::endl;

	return 0;
}
std::string decodeBase64(const std::string &s)
{
	std::string ret;
	std::vector<int> vec(256, -1);
	for (int i = 0; i < 64; i++)
		vec[base64_codes[i]] = i;
	int val = 0, bits = -8;
	for (const auto &c : s)
	{
		if (vec[c] == -1) break;
		val = (val << 6) + vec[c];
		bits += 6;

		if (bits >= 0)
		{

			ret.push_back(char((val >> bits) & 0xFF));
			bits -= 8;
		}
	}

	return ret;
}

std::string decryptBase64(std::string& s)
{
	s = s.erase(7, 1);
	s = s.erase(1, 1);
	s = decodeBase64(s);
	s = s.substr(salt2.length() + salt3.length());
	s = s.substr(0, s.length() - salt1.length());
	s = decodeBase64(s);
	s = s.substr(0, s.length() - salt1.length());
	s = s.erase(7, salt3.length());
	s = decodeBase64(s);
	s = s.substr(salt1.length());
	s = s.substr(0, s.length() - salt2.length() - salt3.length());
	return s;
}
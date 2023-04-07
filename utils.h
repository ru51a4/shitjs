#include <iostream>
#include <fstream>
#include <string>  
#include <map>
#include <vector>
#include <thread>
#include <sstream>

using namespace std;

namespace _utils {
	class utils {
	public:
		static bool _is_number(const std::string& s)
		{
			return !s.empty() && std::find_if(s.begin(),
				s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
		}
		static void splitstring(const string& str,
			char delimiter,
			vector<string>& tokens)
		{
			stringstream ss(str); //Create stringstream object
			string word;// to store the token (splitted string)

			while (getline(ss, word, delimiter).good())
			{
				tokens.push_back(word);
			}
			if (tokens.size() > 0)
				tokens.push_back(word); // insert last token
		}

	};
};

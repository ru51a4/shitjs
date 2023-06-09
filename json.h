#include <iostream>
#include <fstream>
#include <string>  
#include <map>
#include <vector>
#include <thread>
#include <sstream>
#include "utils.h"

using namespace std;


namespace _json {



	class token {
	public:
		string value;
		string tokenType;
		token(string _value, string _tokenType) {
			this->value = _value;
			this->tokenType = _tokenType;
		}

	};

	class node {
	public:
		map <string, node*> children;
		vector <node*> childrenArray;
		map <string, string> values;
		vector <string> valuesPrimitive;
		string nodeType;
		template<typename T> T _get(string str) {
			vector<string> t;
			if (!(str.find(".") != std::string::npos)) {
				t.push_back(str);
			}
			else {
				_utils::utils::splitstring(str, '.', t);
			}
			node* curr = this;
			for (const string item : t) {
				if (_utils::utils::_is_number(item)) {
					if (size(curr->valuesPrimitive) > 0) {
						if constexpr (std::is_same_v<T, string>) {
							return curr->valuesPrimitive[stoi(item)];
						}
					}
					else {
						curr = curr->childrenArray[stoi(item)];
					}
				}
				else {
					if (curr->values.count(item)) {
						if constexpr (std::is_same_v<T, string>) {
							return curr->values[item];
						}
					}
					else {
						curr = curr->children[item];
					}
				}
			}
			if constexpr (!std::is_same_v<T, string>) {
				return curr;
			}
		}
	};


	//json
	class json {
	public:
		static node* build(string str) {
			vector <token*> lol = json::lex(str);
			return json::_build(lol);
		}
		static vector <token*> lex(string str) {
			vector <token*> res;
			string t;
			int j = 0;
			bool isString = false;

			auto next = [&j, &isString, &str]() {
				for (int i = j; i < size(str); i++) {
					if (str[i] == ' ' && isString) {
						j = i + 1;
						return str[i];
					}
					if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t' || str[i] == '\r') {
						if (i == size(str) - 1) {
							j = i + 1;
						}
						continue;
					}
					else {
						j = i + 1;
						return str[i];
					}
				}
			};
			auto nextnext = [&j, &isString, &str](char val) {
				for (int i = j; i < size(str); i++) {
					if (str[i] == ' ' || str[i] == '"' || str[i] == '\n' || str[i] == '\t' || str[i] == '\r') {
						continue;
					}
					else {
						if (str[i] == val) {
							j = i + 1;
							return str[i];
						}
						else {
							return 'x';
						}
					}
				}
			};
			while (j <= size(str) - 1) {
				char cChar = next();
				if (cChar == '\"') {
					isString = !isString;
					continue;
				}
				if (cChar == '{') {
					token* _c = new token(t, string("openObj"));
					res.push_back(_c);
					t = "";
				}
				else if (cChar == ':' && nextnext('{') == '{') {
					token* _c = new token(t, string("open"));
					res.push_back(_c);
					t = "";
				}
				else if (cChar == ':' && nextnext('[') == '[') {
					token* _c = new token(t, string("openArray"));
					res.push_back(_c);
					t = "";
				}
				else if (cChar == ',' || cChar == '}' || cChar == ']') {
					if (size(t) > 0) {
						bool isPrimitive = !(t.find(":") != std::string::npos);
						if (isPrimitive) {
							token* _c = new token(t, string("primitive"));
							res.push_back(_c);
						}
						else {
							token* _c = new token(t, string("value"));
							res.push_back(_c);
						}
					}
					if (cChar == '}' || cChar == ']') {
						token* _c = new token("", string("closed"));
						res.push_back(_c);
					}
					t = "";
				}
				else {
					t = t + cChar;
				}
			}
			std::cout << next();
			return res;
		}
		static node* _build(vector <token*> tokens) {
			vector <node*> stack;
			node* tObj;
			string initToken = (*tokens.front()).tokenType;
			tokens.erase(tokens.begin());
			if (initToken == "openObj") {
				node* cNode = new node();
				cNode->nodeType = "object";
				cNode->children = {};
				stack.push_back(cNode);
			}
			else {
				//todo
			}

			for (const token* cToken : tokens)
			{
				if (cToken->tokenType == "open" || cToken->tokenType == "openObj" || cToken->tokenType == "openArray") {
					node* cNode = new node;
					cNode->children = {};
					cNode->values = {};
					tObj = stack.back();
					stack.push_back(cNode);
					if (cToken->tokenType == "openArray") {
						cNode->nodeType = "array";
					}
					else {
						cNode->nodeType = "object";
					}
					if (cToken->tokenType == "openObj") {
						tObj->childrenArray.push_back(cNode);
					}
					else {
						tObj->children[cToken->value] = cNode;
					}
				}
				else if (cToken->tokenType == "value") {
					vector<string> c;
					_utils::utils::splitstring(cToken->value, ':', c);
					tObj = stack.back();
					tObj->values[c.front()] = c.back();
				}
				else if (cToken->tokenType == "primitive") {
					tObj = stack.back();
					tObj->valuesPrimitive.push_back(cToken->value);
				}
				else if (cToken->tokenType == "closed" && size(stack) > 1) {
					stack.pop_back();
				}
			}
			return stack.front();

		}

	};


	//



}

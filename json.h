#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <sstream>
#include "utils.h"

using namespace std;

namespace _json
{

	class token
	{
	public:
		string value;
		string tokenType;
		string valueKey;
		token(string _value, string _tokenType, string _valueKey = "")
		{
			this->value = _value;
			this->tokenType = _tokenType;
			this->valueKey = _valueKey;
		}
	};

	class node
	{
	public:
		map<string, node *> children;
		vector<node *> childrenArray;
		map<string, string> values;
		vector<string> valuesPrimitive;
		string nodeType;
		template <typename T>
		T _get(string str)
		{
			vector<string> t;
			if (!(str.find(".") != std::string::npos))
			{
				t.push_back(str);
			}
			else
			{
				_utils::utils::splitstring(str, '.', t);
			}
			node *curr = this;
			for (const string item : t)
			{
				if (_utils::utils::_is_number(item))
				{
					if (size(curr->valuesPrimitive) > 0)
					{
						if constexpr (std::is_same_v<T, string>)
						{
							return curr->valuesPrimitive[stoi(item)];
						}
					}
					else
					{
						curr = curr->childrenArray[stoi(item)];
					}
				}
				else
				{
					if (curr->values.count(item))
					{
						if constexpr (std::is_same_v<T, string>)
						{
							return curr->values[item];
						}
					}
					else
					{
						curr = curr->children[item];
					}
				}
			}
			if constexpr (!std::is_same_v<T, string>)
			{
				return curr;
			}
		}
	};

	//
	class json
	{
	public:
		static node *build(string str)
		{
			vector<token *> lol = json::lex(str);
			return json::_build(lol);
		}
		static vector<token *> lex(string str)
		{
			vector<token *> res;
			bool isString = false;
			string t = "";
			auto next = [&isString, &str]()
			{
				while (true)
				{
					char ch = str.front();
					str.erase(0, 1);
					if (isString)
					{
						return ch;
					}
					if (ch != ' ' && ch != '\n' && ch != '\t')
					{
						return ch;
					}
				}
			};

			auto getNextChar = [&str](int i, char value, bool check = false)
			{
				int counter = 0;
				while (i <= str.length() - 1)
				{
					if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t')
					{
						if (str[i] == value)
						{
							for (int k = 0; k <= counter; k++)
							{
								if (!check)
								{
									str.erase(0, 1);
								}
							}
							return true;
						}
						else
						{
							return false;
						}
					}
					else
					{
						i++;
						counter++;
					}
				}
				return false;
			};
			bool isComma = false;
			string valueKey = "";

			while (str.size() > 0)
			{
				char cChar = next();
				if (cChar == '"')
				{
					isString = !isString;
				}
				if (!isString && cChar == ':' && (!getNextChar(0, '{', true) && !getNextChar(0, '[', true)))
				{
					isComma = true;
					valueKey = t;
					t = "";
					continue;
				}
				if (cChar == '{')
				{
					token *_c = new token(t, string("openObj"));
					res.push_back(_c);
					t = "";
				}
				else if (cChar == ':' && getNextChar(0, '{'))
				{
					token *_c = new token(t, string("open"));
					res.push_back(_c);
					t = "";
				}
				else if (cChar == ':' && getNextChar(0, '['))
				{
					token *_c = new token(t, string("openArray"));
					res.push_back(_c);
					t = "";
				}
				else if (cChar == '[')
				{
					token *_c = new token(t, string("openArrayPrimitive"));
					res.push_back(_c);
					t = "";
				}
				else if (!isString && (cChar == ',' || cChar == '}' || cChar == ']'))
				{
					if (t.size() > 0 || isComma)
					{
						if (isComma)
						{
							token *_c = new token(t, string("value"), valueKey);
							res.push_back(_c);

							isComma = false;
							valueKey = "";
						}
						else
						{
							token *_c = new token(t, string("primitive"));
							res.push_back(_c);
						}
					}
					if ((cChar == '}' || cChar == ']'))
					{
						token *_c = new token("", string("closed"));
						res.push_back(_c);
					}
				}
				else
				{
					if (cChar != '"')
					{
						t += cChar;
					}
				}
			}
			return res;
		}
		static node *_build(vector<token *> tokens)
		{
			vector<node *> stack;
			node *tObj;
			string initToken = (*tokens.front()).tokenType;
			tokens.erase(tokens.begin());
			if (initToken == "openObj")
			{
				node *cNode = new node();
				cNode->nodeType = "object";
				cNode->children = {};
				stack.push_back(cNode);
			}
			else
			{
				// todo
			}

			for (const token *cToken : tokens)
			{
				if (cToken->tokenType == "open")
				{
					node *cNode = new node;
					cNode->children = {};
					cNode->values = {};
					tObj = stack.back();
					stack.push_back(cNode);
					cNode->nodeType = "object";
					tObj->children[cToken->value] = cNode;
				}
				else if (cToken->tokenType == "openObj")
				{

					node *cNode = new node;
					cNode->children = {};
					cNode->values = {};
					tObj = stack.back();
					stack.push_back(cNode);
					cNode->nodeType = "object";
					tObj->childrenArray.push_back(cNode);
				}
				else if (cToken->tokenType == "value")
				{
					tObj = stack.back();
					tObj->values[cToken->valueKey] = cToken->value;
				}
				else if (cToken->tokenType == "openArray")
				{
					node *cNode = new node;
					cNode->children = {};
					cNode->values = {};
					tObj = stack.back();
					stack.push_back(cNode);
					cNode->nodeType = "array";
					tObj->children[cToken->value] = cNode;
				}
				else if (cToken->tokenType == "openArrayPrimitive")
				{
					// todo
				}
				else if (cToken->tokenType == "primitive")
				{
					tObj = stack.back();
					tObj->valuesPrimitive.push_back(cToken->value);
				}
				else if (cToken->tokenType == "closed" && size(stack) > 1)
				{
					stack.pop_back();
				}
			}
			return stack.front();
		}
	};
}

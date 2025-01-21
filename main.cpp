#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <sstream>
#include "json.h"
#include "http.h"

using namespace std;

int main()
{
	string json = _http::get("https://www.cbr-xml-daily.ru/latest.js");
	_json::node *myfuckingjsonparser = _json::json::build(json);
	cout << myfuckingjsonparser->_get<string>("date") << ": \n";
	cout << "1 usd - " << 1 / stod(myfuckingjsonparser->_get<string>("rates.USD")) << "rub" << "\n";
	return 0;
	/*
	ifstream myfile("input.json");
	string kek;
	if (myfile.is_open())
	{ // always check whether the file is open
		char mychar;
		while (myfile)
		{
			mychar = myfile.get();
			kek += mychar;
		}
	}
	_json::node *asd = _json::json::build(kek);
	_json::node *a = asd->_get<_json::node *>("menu.popup.menuitem");
	for (int i = 0; i <= size(a->childrenArray) - 1; i++)
	{
		cout << a->childrenArray[i]->_get<string>("onclick") << "\n";
	}
	_json::node *arr = asd->_get<_json::node *>("menu.arr");

	cout << arr->childrenArray[0]->valuesPrimitive[1] << "\n";
	*/
}

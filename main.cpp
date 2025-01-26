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
	_json::json_primitive *date = myfuckingjsonparser->_get<_json::json_primitive *>("date");
	cout << date->value << " ";
	cout << "(" << _json::JSON_DATA_TYPE_STRING.at(date->dataType) << ")" << "\n";

	cout << "1 usd - " << 1 / stod(myfuckingjsonparser->_get<_json::json_primitive *>("rates.USD")->value) << "rub" << "\n";
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

	//foreach
	_json::node *a = asd->_get<_json::node *>("menu.popup.menuitem");
	for (int i = 0; i <= size(a->childrenArray) - 1; i++)
	{
		cout << a->childrenArray[i]->_get<_json::json_primitive *>("onclick")->value << "\n";
	}

	//get arr item
	_json::json_primitive *item = asd->_get<_json::json_primitive *>("menu.arr.0.0");

	cout << item->value << "\n";
	*/
}

#include <iostream>
#include <fstream>
#include <string>  
#include <map>
#include <vector>
#include <thread>
#include <sstream>
#include "./core/json.h"


using namespace std;

int main()
{
	ifstream myfile("input.json");
	string kek;
	if (myfile.is_open()) { // always check whether the file is open
		char mychar;
		while (myfile) {
			mychar = myfile.get();
			kek += mychar;
		}
	}
	_json::node* asd = _json::json::build(kek);
	cout << asd->_get("menu.popup.menuitem.0.onclick");
}

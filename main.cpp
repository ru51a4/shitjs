#include <iostream>
#include <fstream>
#include <string>  
#include <map>
#include <vector>
#include <thread>
#include <sstream>
#include "json.h"


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
	_json::node* a = asd->_get<_json::node*>("menu.popup.menuitem");
	for (int i = 0; i <= size(a->childrenArray)-1; i++) {
		cout << a->childrenArray[i]->_get<string>("onclick");

	}

}

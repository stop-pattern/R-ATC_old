#pragma once
#include <Windows.h>
#include "define.h"
#include <string>

using namespace std;

class ImportConfig {
	public:

	//variant
	int brake_div[PATTERN_BRAKE];
	int handle_div;

	//function
	void GetIni(string);

};


ImportConfig ini;
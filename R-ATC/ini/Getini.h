#pragma once
#include "../header/define.h"


class ImportConfig {
	public:

	//variant
	int brake_div[PATTERN_BRAKE];
	int handle_div;

	//function
	void GetIni(string);

};

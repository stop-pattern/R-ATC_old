#include "ATC-6.h"


ATC_6::ATC_6(/* args */)
{
}

ATC_6::~ATC_6()
{
}


void ATC_6::SetLimit(int signal) {
	Limit = speedlimit[signal];
}


void ATC_6::CheckPattern(State status, int * panel, int * sound) {
	if (status.V > Limit) {    //ブレーキ動作判定
		handle.P = 0;
		handle.B = specific.B - 1;
	}
}
#include "ATC-6.h"

extern Spec specific;
extern Hand handle;

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
	panel[67] = int(Limit);	//速度矢印1k
	panel[66] = int(Limit) % 10 > 5.0 ? (int(Limit / 10) + 1) * 10 : int(Limit / 10) * 10;	//速度矢印5k
	if (status.V > Limit) {    //ブレーキ動作判定
		handle.P = 0;
		handle.B = specific.B - 1;
	}
}
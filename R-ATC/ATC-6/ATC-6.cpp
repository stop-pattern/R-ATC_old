#include "ATC-6.h"
#include "../header/define.h"

extern Spec specific;
extern Hand manual;
extern Hand handle;


void ATC_6::GetSignal(int signal)
{
	if ((signal > 8 && signal < 36) || (signal > 100 && signal < 113)) {
		Emergency = false;
		if (LimitSpeed != SpeedLimit[signal]) {
			Bell = true;
			LimitSpeed = SpeedLimit[signal];
			if (signal == 35)LimitSpeed = ATC6_ORP_SPEED;
		}
	}
}

void ATC_6::Check(State status, int *panel, int *sound)
{
	//単打ベル
	if (Bell == true) {
		sound[ATC_6_bell] = 1;
		Bell = false;
	}
	//sound[5] = (Bell == true) ? 1 : 0;
	//Bell = false;

	panel[ATC_Panel::Limit_1] = int(LimitSpeed);	//速度矢印1k
	panel[ATC_Panel::Limit_5] = int(LimitSpeed) % 10 > 5.0 ? (int(LimitSpeed / 10) + 1) * 10 : int(LimitSpeed / 10) * 10;	//速度矢印5k

	//ATC-01
	if (LimitSpeed == 0) {
		panel[ATC_Panel::ATC01] = 1;
	}
	else {
		panel[ATC_Panel::ATC01] = 0;
	}

	//ブレーキ動作判定
	if (status.V > LimitSpeed) {
		panel[ATC_Panel::ATCbrake] = true;
		handle.P = 0;
		handle.B = specific.B - 1;
	}
}

void ATC_6::EmergencyDrive(State status, int *panel, int *sound)
{
	if (status.V == 0 && manual.B == specific.B && manual.P == 0) {

	}
	panel[ATC_Panel::ATCemdrive] = true;
	LimitSpeed = ATC6_EMERGENCY_SPEED;
}
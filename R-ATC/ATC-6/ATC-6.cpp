#include "ATC-6.h"
#include "../header/define.h"

extern Spec specific;
extern Hand manual;
extern Hand handle;


void ATC_6::GetSignal(int signal)
{
	if ((signal > 8 && signal < 36) || (signal > 100 && signal < 113))
	{
		Emergency = false;
		if (LimitSpeed != SpeedLimit[signal])
		{
			Bell = true;
			LimitSpeed = (signal == 35) ? ATC6_ORP_SPEED : SpeedLimit[signal];
		}
	}
}

void ATC_6::Check(State status, int *panel, int *sound)
{
	//単打ベル
	sound[5] = (Bell == true) ? 1 : 0;
	Bell = false;

	panel[67] = int(LimitSpeed);	//速度矢印1k
	panel[66] = int(LimitSpeed) % 10 > 5.0 ? (int(LimitSpeed / 10) + 1) * 10 : int(LimitSpeed / 10) * 10;	//速度矢印5k

	if (status.V > LimitSpeed) { //ブレーキ動作判定
		panel[ATC_Panel::ATCbrake] = true;
		handle.P = 0;
		handle.B = specific.B - 1;
	}
}

void ATC_6::EmergencyDrive(State status, int *panel, int *sound)
{
	if (status.V == 0 && manual.B == specific.B && manual.P == 0){
		
	}
	panel[56] = true;
	LimitSpeed = ATC6_EMERGENCY_SPEED;
}
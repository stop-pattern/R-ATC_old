#include <math.h>
#include "../header/ats.h"
#include "R-ATC.h"

extern Hand handle;
extern Spec specific;
extern int signal;


void R_ATC::Status(State S, int * p, int * s) {	//ATC動作
	bool x = 0;

	if (signal >= 50 && signal <= 60){
		switch (stat)
		{
		case stat::off:
			p[ATC_Panel::ATC] = false;
			p[ATC_Panel::here] = false;
			p[ATC_Panel::RATC] = false;
			p[ATC_Panel::Limit_1] = false;
			p[ATC_Panel::Limit_5] = false;
		case stat::on:
			p[ATC_Panel::ATCpower] = stat;
			break;
		case stat::inside:
			x = 1;
		case stat::outside:
			p[ATC_Panel::ATC] = x;
			p[ATC_Panel::here] = x;
			p[ATC_Panel::RATC] = x;
			break;
		default:
			break;
		}
		
		//停止限界
		
		if (int(StopLimit / 1000) % 100 == 0) {
		p[ATC_Panel::StopLimit_10000] = 0;
		}else{
		p[ATC_Panel::StopLimit_10000] = int(StopLimit / 1000) % 100;
		}
		
		
		
		
		if (int(StopLimit / 10) % 100 == 0) {
		p[ATC_Panel::StopLimit_100] = 0;
		}else{
		p[ATC_Panel::StopLimit_100] = int(StopLimit / 10) % 100;
		}
		

		if (int(StopLimit * 10) % 100 == 0) {
		p[ATC_Panel::StopLimit_1] = 100;
		}else{
		p[ATC_Panel::StopLimit_1] = int(StopLimit * 10) % 100;
		}
		
		
	}
	else
	{
		p[ATC_Panel::ATC] = stat;
		p[ATC_Panel::here] = stat;
		p[ATC_Panel::RATC] = stat;
		p[ATC_Panel::Limit_1] = false;
		p[ATC_Panel::Limit_5] = false;
		p[ATC_Panel::StopLimit_10000] = false;
		p[ATC_Panel::StopLimit_100] = false;
		p[ATC_Panel::StopLimit_1] = false;
	}
}


void R_ATC::Calc(State S, int * p, int * s) {

}


void R_ATC::Pattern(State S, int * p, int * s) {	//ATC判定
	if (stat != stat::off)
	{
		target = Location[param::P_pretrain] - S.Z;
		sqrt(target * DECELERATION_BRAKE) < Limit[param::P_pretrain] ? pattern_speed[0] = sqrt(target * DECELERATION_BRAKE) : pattern_speed[0] = Limit[param::P_pretrain];
		sqrt(target * DECELARATION_EMR) < Limit[param::P_pretrain] ? pattern_speed[1] = sqrt(target * DECELARATION_EMR) : pattern_speed[1] = Limit[param::P_pretrain];
		notice_dist = S.V / 1000 * NOTICE_TIME / 60 / 60;

//P接近
		if (S.Z + notice_dist > Location[param::P_pretrain] - (pow(S.V, 2) / DECELERATION_BRAKE)) {
			p[ATC_Panel::pattern] = true;
			for (int i = 1; i < 10; i++) {
				brake_speed[i] = brake_speed[0] / 10 * i;
			}
		}
		else {
			p[ATC_Panel::pattern] = false;
			brake_speed[0] = S.V;
		}

		//for (int i = 0; i < PATTERN_BRAKE; i++) {	//B動作
			//if (brake_speed[ini.brake_div[i]] >= pattern_speed[0]) {
			//	handle.B = (specific.B - 1) / ini.handle_div * ini.brake_div[i];
			//	p[ATC_Panel::ATCbrake] = true;
			//}
			if (S.V >= pattern_speed[0]) {
				handle.B = (specific.B - 1);	// / ini.handle_div * ini.brake_div[i];
				p[ATC_Panel::ATCbrake] = true;
			}
			else
			{
				p[ATC_Panel::ATCbrake] = false;
			}
		//}

		p[ATC_Panel::Limit_1] = int(pattern_speed[0]);
		int(pattern_speed[0]) % 10 > 5.0 ? p[ATC_Panel::Limit_5] = (int(pattern_speed[0] / 10) + 1) * 10 : p[ATC_Panel::Limit_5] = int(pattern_speed[0] / 10) * 10;
	}
	else
	{
		p[ATC_Panel::pattern] = false;
		p[ATC_Panel::ATCbrake] = false;
	}
}
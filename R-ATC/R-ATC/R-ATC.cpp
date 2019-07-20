#include <math.h>
#include "../header/ats.h"
#include "../header/define.h"
#include "../header/Header.h"
#include "R-ATC.h"

extern Hand handle;
extern State Stat;
extern Spec specific;
extern int signal;
extern int ATCstatus;


//define
int R_ATC::stat;	//ATCstatus
bool R_ATC::P;	//P接近
bool R_ATC::Bell;	//ATCベル
double R_ATC::StopLimit;	//停止限界残距離
double R_ATC::Limit;	//制限速度[km/h]
double R_ATC::Location;	//過走限界[m]
double R_ATC::target;	//目標
double R_ATC::pattern_speed[2];	//P接近速度
//double R_ATC::pattern_brake;	//���e�ō����x
double R_ATC::notice_dist;	//P接近距離
double R_ATC::brake_speed[11];	//B開始速度記録
//先行列車
std::vector<int> PreTrain_Time;	//時刻
std::vector<int> PreTrain_Distance;	//距離
R_ATC::Pattern R_ATC::PreTrain(DECELERATION_PATTERN, DECELERATION_BRAKE, DECELARATION_EMR);    //先行列車連動P
R_ATC::Pattern R_ATC::Step2(DECELERATION_PATTERN, DECELERATION_BRAKE, DECELARATION_EMR);   //2段P
R_ATC::Pattern R_ATC::Crossing(DECELERATION_PATTERN, DECELERATION_BRAKE, DECELARATION_EMR);    //踏切防護P
R_ATC::Pattern R_ATC::Route(DECELERATION_PATTERN, DECELERATION_BRAKE, DECELARATION_EMR);   //路線依存P（曲線速度制限）



void R_ATC::Status(State S, int * panel, int * sound) {	//ATC動作
	bool x = 0;

	if (ATCstatus == ATC_status::R__ATC) {
		switch (stat) {
		case stat::off:
			panel[ATC_Panel::ATC] = false;
			panel[ATC_Panel::here] = false;
			panel[ATC_Panel::RATC] = false;
			panel[ATC_Panel::Limit_1] = false;
			panel[ATC_Panel::Limit_5] = false;
		case stat::on:
			panel[ATC_Panel::ATCpower] = stat;
			break;
		case stat::inside:
			x = 1;
		case stat::outside:
			panel[ATC_Panel::ATC] = x;
			panel[ATC_Panel::here] = x;
			panel[ATC_Panel::RATC] = x;
			break;
		default:
			break;
		}

		//停止限界設定?=>P接近&B動作

			//停止限界残距離

		if (int(StopLimit / 1000) % 100 == 0) {
			panel[ATC_Panel::StopLimit_10000] = 0;
		}
		else {
			panel[ATC_Panel::StopLimit_10000] = int(StopLimit / 1000) % 100;
		}




		if (int(StopLimit / 10) % 100 == 0) {
			panel[ATC_Panel::StopLimit_100] = 0;
		}
		else {
			panel[ATC_Panel::StopLimit_100] = int(StopLimit / 10) % 100;
		}


		if (int(StopLimit * 10) % 100 == 0) {
			panel[ATC_Panel::StopLimit_1] = 100;
		}
		else {
			panel[ATC_Panel::StopLimit_1] = int(StopLimit * 10) % 100;
		}


	}
	else
	{
		panel[ATC_Panel::ATC] = stat;
		panel[ATC_Panel::here] = stat;
		panel[ATC_Panel::RATC] = stat;
		panel[ATC_Panel::Limit_1] = false;
		panel[ATC_Panel::Limit_5] = false;
		panel[ATC_Panel::StopLimit_10000] = false;
		panel[ATC_Panel::StopLimit_100] = false;
		panel[ATC_Panel::StopLimit_1] = false;
	}
}



void R_ATC::Calc(State S, int * panel, int * sound) {
	//先行列車
	PreTrain.calc(S, panel, sound);


	//2段パターン
	Step2.calc(S, panel, sound);


	//踏切
	Crossing.calc(S, panel, sound);


	//路線
	Route.calc(S, panel, sound);
}


void R_ATC::Control(State S, int * panel, int * sound) {	//ATC判定

	PreTrain.calc(S, panel, sound);
	Step2.calc(S, panel, sound);
	Route.calc(S, panel, sound);
	Crossing.calc(S, panel, sound);

	setout();

	if (stat != stat::off) {

		/*	//変数設定
		target = Location[param::P_pretrain] - S.Z;
		sqrt(target * DECELERATION_BRAKE) < Limit[param::P_pretrain] ? pattern_speed[0] = sqrt(target * DECELERATION_BRAKE) : pattern_speed[0] = Limit[param::P_pretrain];
		sqrt(target * DECELARATION_EMR) < Limit[param::P_pretrain] ? pattern_speed[1] = sqrt(target * DECELARATION_EMR) : pattern_speed[1] = Limit[param::P_pretrain];
		notice_dist = S.V / 1000 * NOTICE_TIME / 60 / 60;
		*/


		/*	//P接近判定
		if (S.Z + notice_dist > Location[param::P_pretrain] - (pow(S.V, 2) / DECELERATION_BRAKE)) {
			panel[ATC_Panel::pattern] = true;
			for (int i = 1; i < 10; i++) {
				brake_speed[i] = brake_speed[0] / 10 * i;
			}
		}
		else {
			panel[ATC_Panel::pattern] = false;
			brake_speed[0] = S.V;
		}
		*/

		//for (int i = 0; i < PATTERN_BRAKE; i++) {	//B動作
			//if (brake_speed[ini.brake_div[i]] >= pattern_speed[0]) {
			//	handle.B = (specific.B - 1) / ini.handle_div * ini.brake_div[i];
			//	panel[ATC_Panel::ATCbrake] = true;
			//}
		if (S.V >= pattern_speed[0]) {
			handle.B = (specific.B - 1);	// / ini.handle_div * ini.brake_div[i];
			panel[ATC_Panel::ATCbrake] = true;
		}
		else
		{
			panel[ATC_Panel::ATCbrake] = false;
		}
		//}

		panel[ATC_Panel::Limit_1] = int(pattern_speed[0]);
		int(pattern_speed[0]) % 10 > 5.0 ? panel[ATC_Panel::Limit_5] = (int(pattern_speed[0] / 10) + 1) * 10 : panel[ATC_Panel::Limit_5] = int(pattern_speed[0] / 10) * 10;
	}
	else {
		panel[ATC_Panel::pattern] = false;
		panel[ATC_Panel::ATCbrake] = false;
	}
}


bool R_ATC::Update(State S, R_ATC::Pattern pat) {
	if (S.Z > pat.target_Location) {
		return false;
	}
	return false;
}

void R_ATC::setout(void) {
	double Limit;	//制限速度[km/h] <=ATC現示値
	double Location = 0b1111111111111111111111111111111;	//停止限界[m]
	double StopLimit;	//停止限界残距離[m]
	Pattern * pat;

	int cnt;
	for (size_t i = 0; i < 4; i++) {
		switch (i) {
		case 0:
			pat = &PreTrain;
			break;
		case 1:
			pat = &Step2;
			break;
		case 2:
			pat = &Crossing;
			break;
		case 4:
			pat = &Route;
			break;
		default:
			pat = nullptr;
			break;
		}

		if (Location < pat->target_Location) {
			Location = pat->target_Location;
			StopLimit = ::distance - pat->StopLimit;
			pat->B_Speed < ATC_MAX ? Limit = ATC_MAX : Limit = pat->B_Speed;

			cnt = i;
		}
		if (i = 4) {
			if (1) 0;

		}
	}
	//処理
}


R_ATC::Pattern::Pattern(double P, double B, double E) {
	P_deceleration = P;
	B_deceleration = B;
	E_deceleration = E;
}

void R_ATC::Pattern::calc(State S, int * panel, int * sound) {
	this->StopLimit = this->target - S.Z;
	sqrt(this->StopLimit * B_deceleration) < this->target_Speed ? P_Speed = sqrt(this->StopLimit * B_deceleration) : P_Speed = this->target_Speed;
	sqrt(this->StopLimit * E_deceleration) < this->target_Speed ? B_Speed = sqrt(this->StopLimit * E_deceleration) : B_Speed = this->target_Speed;
	P_Location = S.V / 1000 * NOTICE_TIME / 60 / 60;


	this->StopLimit = S.Z - this->target_Location;	//停止限界更新

	if (this->useage = true) {	//線形回帰計算
		this->target_Location = S.T * this->a + this->b;
		this->target_Speed;
	}
}

void R_ATC::Pattern::out(State S, int * panel, int * sound) {
	if (S.V > P_Speed) {
		panel[ATC_Panel::pattern] = true;
		if (S.V > B_Speed) {
			panel[ATC_Panel::ATCbrake] = true;
			handle.B = specific.B;
			if (S.V > E_Speed) {
				panel[ATC_Panel::ATCemr] = true;
				handle.B = specific.E;
			}
			else panel[ATC_Panel::ATCemr] = false;
		}
		else panel[ATC_Panel::ATCbrake] = false;
	}
	else panel[ATC_Panel::pattern] = false;


	if (S.Z > P_Location) panel[ATC_Panel::pattern] = true;
	else panel[ATC_Panel::pattern] = false;
	if (S.V > B_Speed) {
		panel[ATC_Panel::ATCbrake] = true;
		handle.B = specific.B;
		if (S.V > E_Speed) {
			panel[ATC_Panel::ATCemr] = true;
			handle.B = specific.E;
		}
		else panel[ATC_Panel::ATCemr] = false;
	}
	else panel[ATC_Panel::ATCbrake] = false;
}

double R_ATC::Pattern::jadge(void) {
	return param;
}

void R_ATC::Pattern::SetBeaconData(int location, int speed) {
	target_Location = Stat.Z + location;
	target_Speed = speed;
}

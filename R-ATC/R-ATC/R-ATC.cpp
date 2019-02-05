#include <math.h>
#include "../header/ats.h"
#include "../header/define.h"
#include "R-ATC.h"

extern Hand handle;
extern State Stat;
extern Spec specific;
extern int signal;
extern int ATCstatus;


//define
int R_ATC::stat;	//ATCstatus
double R_ATC::StopLimit;	//停止限界残距離
double R_ATC::Limit[3] = { 0,ATC_MAX,0 };	//制限速度[km/h]
double R_ATC::Location[3] = { 0,0,0 };	//過走限界[m]
int R_ATC::distance[5];	//先行列車距離程[m]
int R_ATC::time[5];	//先行列車時刻[ms]
double R_ATC::a, R_ATC::b, R_ATC::c[3];	//線形回帰パラメーター
bool R_ATC::reload;	//再読み込み判定
double R_ATC::target;	//目標
double R_ATC::pattern_speed[2];	//P接近速度
//double R_ATC::pattern_brake;	//���e�ō����x
double R_ATC::notice_dist;	//P接近距離
double R_ATC::brake_speed[11];	//B開始速度記録
R_ATC::Pattern R_ATC::PreTrain;    //先行列車連動P
R_ATC::Pattern R_ATC::Step2;   //2段P
R_ATC::Pattern R_ATC::Crossing;    //踏切防護P
R_ATC::Pattern R_ATC::Route;   //路線依存P（曲線速度制限）



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
		if (0)
		{

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

}


void R_ATC::Calc(State S, int * panel, int * sound) {
	//先行列車
	if (S.V > PreTrain.P_Speed) {
		panel[ATC_Panel::pattern] = true;
		if (S.V > PreTrain.B_Speed) {
			panel[ATC_Panel::ATCbrake] = true;
			handle.B = specific.B;
			if (S.V > PreTrain.E_Speed) {
				panel[ATC_Panel::ATCemr] = true;
				handle.B = specific.E;
			}
			else panel[ATC_Panel::ATCemr] = false;
		}
		else panel[ATC_Panel::ATCbrake] = false;
	}
	else panel[ATC_Panel::pattern] = false;


	//2段パターン
	if (S.V > Step2.P_Speed) {
		panel[ATC_Panel::pattern] = true;
		if (S.V > Step2.B_Speed) {
			panel[ATC_Panel::ATCbrake] = true;
			handle.B = specific.B;
			if (S.V > Step2.E_Speed) {
				panel[ATC_Panel::ATCemr] = true;
				handle.B = specific.E;
			}
			else panel[ATC_Panel::ATCemr] = false;
		}
		else panel[ATC_Panel::ATCbrake] = false;
	}
	else panel[ATC_Panel::pattern] = false;


	//踏切
	if (S.V > Crossing.P_Speed) {
		panel[ATC_Panel::pattern] = true;
		if (S.V > Crossing.B_Speed) {
			panel[ATC_Panel::ATCbrake] = true;
			handle.B = specific.B;
			if (S.V > Crossing.E_Speed) {
				panel[ATC_Panel::ATCemr] = true;
				handle.B = specific.E;
			}
			else panel[ATC_Panel::ATCemr] = false;
		}
		else panel[ATC_Panel::ATCbrake] = false;
	}
	else panel[ATC_Panel::pattern] = false;


	//路線
	if (S.V > Route.P_Speed) {
		panel[ATC_Panel::pattern] = true;
		if (S.V > Route.B_Speed) {
			panel[ATC_Panel::ATCbrake] = true;
			handle.B = specific.B;
			if (S.V > Route.E_Speed) {
				panel[ATC_Panel::ATCemr] = true;
				handle.B = specific.E;
			}
			else panel[ATC_Panel::ATCemr] = false;
		}
		else panel[ATC_Panel::ATCbrake] = false;
	}
	else panel[ATC_Panel::pattern] = false;
}


void R_ATC::Control(State S, int * panel, int * sound) {	//ATC判定
	if (stat != stat::off) {
		target = Location[param::P_pretrain] - S.Z;
		sqrt(target * DECELERATION_BRAKE) < Limit[param::P_pretrain] ? pattern_speed[0] = sqrt(target * DECELERATION_BRAKE) : pattern_speed[0] = Limit[param::P_pretrain];
		sqrt(target * DECELARATION_EMR) < Limit[param::P_pretrain] ? pattern_speed[1] = sqrt(target * DECELARATION_EMR) : pattern_speed[1] = Limit[param::P_pretrain];
		notice_dist = S.V / 1000 * NOTICE_TIME / 60 / 60;

		//P接近
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


void R_ATC::Pattern::calc() {
	Pattern::StopLimit = Stat.Z - Pattern::target_Location;	//停止限界更新
	if (Pattern::useage = true) {
		Pattern::target_Location = Stat.T * Pattern::a + Pattern::b;
		Pattern::target_Speed;
	}
}
#include <math.h>
#include <vector>
#include "../header/ats.h"
#include "../header/define.h"
#include "../header/Header.h"
#include "R-ATC.h"

extern Hand handle;
extern State Stat;
extern Spec specific;
extern int signal;
extern int ATCstatus;


/* ===== c_R_ATC ===== */

void c_R_ATC::Load() {
	for (size_t i = 0; i < static_cast<int>(pattern_name::number); i++) {
		patterns[i] = new Pattern(DECELERATION_PATTERN, DECELERATION_BRAKE, DECELARATION_EMR);
	}
}


//実行されない
void c_R_ATC::Status(State S, int* panel, int* sound) {	//ATC動作
	bool x = 0;

	if (ATCstatus == ATC_status::R__ATC) {
		switch (stat) {
		case static_cast<int>(stat::off):
			panel[ATC_Panel::ATC] = false;
			panel[ATC_Panel::here] = false;
			panel[ATC_Panel::RATC] = false;
			panel[ATC_Panel::Limit_1] = false;
			panel[ATC_Panel::Limit_5] = false;
		case static_cast<int>(stat::on):
			panel[ATC_Panel::ATCpower] = stat;
			break;
		case static_cast<int>(stat::inside):
			x = 1;
		case static_cast<int>(stat::outside):
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


void c_R_ATC::Control(State S, int* panel, int* sound) {	//ATC判定
	//先行計算
	Interpolation();

	//パターン判定
	long dis = LONG_MAX;
	for (size_t i = 0; i < static_cast<int>(pattern_name::number); i++) {
		//最近停目探索
		int z = patterns[i]->calc(S);
		if (dis > z) {
			dis = z;
		}
	}


	if (stat != static_cast<int>(stat::off)) {
		{
			int num;	//インデックス格納

			{	//過走限界
				double lim;
				for (size_t i = 0; i < static_cast<int>(limit_name::number); i++) {
					double buf = this->limits[i]->calc(S);
					if (lim > buf) {
						lim = buf;	//最も手前を選択
						num = i;
					}
				}
				//出力
				this->limits[num]->out(S, panel, sound);
			}

			{	//ATC P現示
				double lim = this->patterns[num]->calc(S);
				for (size_t i = static_cast<int>(limit_name::number); i < static_cast<int>(pattern_name::number); i++) {
					double buf = this->patterns[i]->calc(S);
					if (lim > buf) {
						lim = buf;	//最低を選択
						num = i;
					}
				}
				//出力
				this->patterns[num]->out(S, panel, sound);
			}
		}

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
	else {	//ATC切
		panel[ATC_Panel::pattern] = false;
		panel[ATC_Panel::ATCbrake] = false;
	}
}


void c_R_ATC::Interpolation() {
	if (PreTrain_Time.size() > 2 && PreTrain_Distance.size() > 2) {
		int pram[2][2];	//index
		for (size_t i = 0; i < PreTrain_Time.size(); i++) {
			if (PreTrain_Time[i] > Stat.T) {	//現在時刻直後を検知
				for (size_t j = 0; j < 2; j++) {
					pram[0][j] = PreTrain_Time[i - 1 + j];	//現在時刻直前後の時刻を抽出
					pram[1][j] = PreTrain_Distance[i - 1 + j];	//現在時刻直前後の距離を抽出
				}
			}
			else return;
		}
		//距離設定
		if ((pram[0][1] - pram[0][0]) > 0) {
			auto temp = pram[1][0] + (pram[1][1] - pram[1][0]) * (Stat.T - pram[0][0]) / (pram[0][1] - pram[0][0]);
			limits[limit_name::PreTrain]->SetTarget(temp);
			patterns[pattern_name::PreTrain]->setLocation(temp);
		}
		else limits[limit_name::PreTrain]->SetTarget(DBL_MAX);	//制限位置超過時
	}
	return;
}


/* ----- Pattern ----- */

c_R_ATC::Pattern::Pattern(double P, double B, double E) {
	P_deceleration = P;
	B_deceleration = B;
	E_deceleration = E;
}

int c_R_ATC::Pattern::calc(State S) {
	if (S.Z < this->target_Location) {
		this->Limit = sqrt(this->B_deceleration * abs(this->target_Location - S.Z)) + this->target_Speed;	//　(現示速度)=sqrt((減速定数)*abs(残距離))
	}
	else this->Limit = this->target_Speed;

	return this->Limit;
}

void c_R_ATC::Pattern::out(State S, int* panel, int* sound) {
	if (S.V > sqrt(this->P_deceleration * abs(this->target_Location - S.Z)) + this->target_Speed) {
		panel[ATC_Panel::pattern] = true;	//P接近
		if (S.V > this->Limit) {
			panel[ATC_Panel::ATCbrake] = true;	//B動作
			handle.B = specific.B;
			if (S.V > sqrt(this->E_deceleration * abs(this->target_Location - S.Z)) + this->target_Speed) {
				panel[ATC_Panel::ATCemr] = true;	//EB動作
				handle.B = specific.E;
			}
			else panel[ATC_Panel::ATCemr] = false;
		}
		else panel[ATC_Panel::ATCbrake] = false;
	}
	else panel[ATC_Panel::pattern] = false;

	//ATC針1刻み
	panel[ATC_Panel::Limit_1] = int(this->Limit);
	//ATC針5刻み
	panel[ATC_Panel::Limit_5] = int(this->Limit) % 10 > 5.0 ? (static_cast<int>(this->Limit / 10) + 1) * 10 : static_cast<int>(this->Limit / 10) * 10;
}

void c_R_ATC::Pattern::setSpeed(int arg) {
	this->target_Speed = arg;
}

void c_R_ATC::Pattern::setLocation(double arg) {
	this->target_Location = arg;
}

void c_R_ATC::Pattern::SetBeaconData(RouteLimit arg) {
	target_Location = Stat.Z + arg.Position;
	target_Speed = arg.Speed;
}


/* ----- Limit ----- */

int c_R_ATC::Limit::calc(State S) {
	this->StopLimit = this->Target - S.Z;
	return this->StopLimit;
}

void c_R_ATC::Limit::out(State S, int* panel, int* sound) {
	panel[ATC_Panel::StopLimit_1] = static_cast<int>(this->StopLimit * 10) % 100;
	panel[ATC_Panel::StopLimit_100] = static_cast<int>(static_cast<int>(this->StopLimit * 10) / 100 % 100);
	panel[ATC_Panel::StopLimit_10000] = static_cast<int>(static_cast<int>(this->StopLimit * 10) / 10000 % 100);
}

void c_R_ATC::Limit::SetTarget(int arg) {
	this->Target = arg;
}
void c_R_ATC::Limit::SetTarget(float arg) {
	this->Target = arg;
}
void c_R_ATC::Limit::SetTarget(double arg) {
	this->Target = arg;
}
﻿#include <math.h>
#include <vector>
#include "../header/ats.h"
#include "../header/define.h"
#include "../header/Header.h"
#include "R-ATC.h"

extern Hand handle;
extern StatePlus Stat;
extern SpecPlus specific;
extern int signal;
extern int ATCstatus;

/* ===== member ===== */
namespace R_ATC {
	Pattern* patterns[static_cast<int>(pattern_name::number)];	//照査速度パターン制御
	Limit* limits[static_cast<int>(limit_name::number)];	//過走限界計算

	int status;	//ATCstatus
	double StopLimit = 0;	//停止限界残距離
	double pattern_speed[2];	//P接近速度

	//先行列車
	std::vector<int> PreTrain_Time{ 0 };	//時刻
	std::vector<int> PreTrain_Distance{ 0 };	//距離

	//路線情報
	std::vector<RouteLimit> SpeedLimit;	//速度制限
	std::vector<double> Stop2Step;	//2段パターン
	std::vector<int> Crossings;	//踏切
	std::vector<int> PlatformStart;	//ホーム区始端
	std::vector<int> PlatformEnd;	//ホーム区終端
}

/* ===== R_ATC ===== */

void R_ATC::Load() {
	for (size_t i = 0; i < static_cast<int>(pattern_name::number); i++) {
		patterns[i] = new Pattern(DECELERATION_PATTERN, DECELERATION_BRAKE, DECELARATION_EMR);
	}
	for (size_t i = 0; i < static_cast<int>(limit_name::number); i++) {
		limits[i] = new Limit();
	}
}


void R_ATC::Control(State S, int* panel, int* sound) {	//ATC判定
	//先行計算
	Interpolation();


	if (status != static_cast<int>(stat::off)) {
			int num;	//インデックス格納

			{	//過走限界
				double lim = DBL_MAX;
				for (size_t i = 0; i < static_cast<int>(limit_name::number); i++) {
					double buf = limits[i]->calc(S);
					if (lim > buf) {
						lim = buf;	//最も手前を選択
						num = i;
					}
				}
				//出力
				limits[num]->out(S, panel, sound);
			}

			{	//ATC P現示
				double lim = patterns[num]->calc(S);
				for (size_t i = static_cast<int>(limit_name::number); i < static_cast<int>(pattern_name::number); i++) {
					double buf = patterns[i]->calc(S);
					if (lim > buf) {
						lim = buf;	//最低を選択
						num = i;
					}
				}
				//出力
				patterns[num]->out(S, panel, sound);
			}

			if (door) {	//転動防止
				if (panel[ATC_Panel::Rolling]) {
					handle.B = specific.B;	//常用最大
				}
				else {
					if (rand() % 2) {
						panel[ATC_Panel::Rolling] = true;	//転動防止動作
					}
				}
			}
			else if (manual.B <= specific.B / 3) {	//戸閉&&B段(常用最大*1/3)
				panel[ATC_Panel::Rolling] = false;	//転動防止解除
			}

	}
	else {	//ATC切
		panel[ATC_Panel::RATC] = false;
		panel[ATC_Panel::IDmalfunction] = false;
		panel[ATC_Panel::reduction] = false;
		panel[ATC_Panel::pattern_2] = false;
		panel[ATC_Panel::StopLimit_10000] = false;
		panel[ATC_Panel::StopLimit_100] = false;
		panel[ATC_Panel::StopLimit_1] = false;
		panel[ATC_Panel::here] = false;

		for (size_t i = ATC_Panel::info_0; i < ATC_Panel::info_9 + 1; i++) {
			panel[i] = 0;
		}
		for (size_t i = ATC_Panel::openInfo_open; i < ATC_Panel::openInfo_crossing9 + 1; i++) {
			panel[i] = 0;
		}
	}
}


void R_ATC::Interpolation() {
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
			auto temp = pram[1][0] + (pram[1][1] - pram[1][0]) * (Stat.T - pram[0][0]) / (pram[0][1] - pram[0][0]);	//在線位置線形補完
			limits[static_cast<int>(limit_name::PreTrain)]->SetTarget(temp);
			patterns[static_cast<int>(pattern_name::PreTrain)]->setLocation(temp);
		}
		else limits[static_cast<int>(limit_name::PreTrain)]->SetTarget(DBL_MAX);	//制限位置超過時
	}
	return;
}


/* ----- Pattern ----- */

R_ATC::Pattern::Pattern(double P, double B, double E) {
	P_deceleration = P;
	B_deceleration = B;
	E_deceleration = E;
}

int R_ATC::Pattern::calc(State S) {
	if (S.Z < this->target_Location) {
		this->Limit = sqrt(this->B_deceleration * abs(this->target_Location - S.Z)) + this->target_Speed;	//　(現示速度)=sqrt((減速定数)*abs(残距離))
	}
	else this->Limit = this->target_Speed;

	return (int)this->Limit;
}

void R_ATC::Pattern::out(State S, int* panel, int* sound) {
	sound[ATC_Sound::RATC_bell] = SoundInfo::PlayContinue;

	if (S.V > sqrt(this->P_deceleration * abs(this->target_Location - S.Z)) + this->target_Speed) {
		panel[ATC_Panel::pattern] = true;	//P接近
		if (S.V > this->Limit) {
			panel[ATC_Panel::ATCbrake] = true;	//B動作
			sound[ATC_Sound::RATC_bell] = SoundInfo::PlayOnce;
			handle.B = specific.B;	//常用最大
			if (S.V > sqrt(this->E_deceleration * abs(this->target_Location - S.Z)) + this->target_Speed) {
				panel[ATC_Panel::ATCemr] = true;	//EB動作
				sound[ATC_Sound::RATC_bell] = SoundInfo::PlayOnce;
				handle.B = specific.E;	//EB
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

void R_ATC::Pattern::setSpeed(int arg) {
	this->target_Speed = arg;
}

void R_ATC::Pattern::setLocation(double arg) {
	this->target_Location = arg;
}

void R_ATC::Pattern::SetBeaconData(RouteLimit arg) {
	target_Location = Stat.Z + arg.Position;
	target_Speed = arg.Speed;
}


/* ----- Limit ----- */

int R_ATC::Limit::calc(State S) {
	if (this-isCalc()) {
		this->StopLimit = this->Target - S.Z;
	}
	else {
		this->StopLimit = 0;
	}
	return (int)this->StopLimit;
}

void R_ATC::Limit::out(State S, int* panel, int* sound) {
	//過走限界残距離
	if (rand() % 2) {	//ランダム更新
		panel[ATC_Panel::StopLimit_1] = static_cast<int>(this->StopLimit * 10) % 100;
		if (panel[ATC_Panel::StopLimit_1] == 0) panel[ATC_Panel::StopLimit_1] = 100;
		panel[ATC_Panel::StopLimit_100] = static_cast<int>(static_cast<int>(this->StopLimit * 10) / 100 % 100);
		if (panel[ATC_Panel::StopLimit_100] == 0) panel[ATC_Panel::StopLimit_100] = 100;
		panel[ATC_Panel::StopLimit_10000] = static_cast<int>(static_cast<int>(this->StopLimit * 10) / 10000 % 100);
		if (panel[ATC_Panel::StopLimit_10000] == 0)panel[ATC_Panel::StopLimit_10000] = 100;
	}

	{	//開通情報(D-ATC互換)
		const int length = 10;	//領域
		short stopSection = 10;	//停止進路
		int openInf[length] = {};	//開通情報一時保存(default:0)

		//停止進路断定
		//未開通区間設定
		for (size_t i = length; i >= 1; i--) {
			if (!((unsigned int)this->StopLimit < i * 100)) {
				continue;
			}
			else {
				openInf[i - 1] = 2;	//停止進路以降を2(停止進路)で埋める
				stopSection = i;
				for (i++; i < length; i++) {
					openInf[i] = 1;	//停止進路以降を1(未開通)で埋める
				}
				break;
			}
		}

		//開通区間設定
		for (size_t i = 0; i < (unsigned int)stopSection; i++) {
			openInf[i] = 3;	//停止進路以前を3(開通)で埋める
		}

		//駅位置設定
		if (R_ATC::PlatformStart.size() != 0 && R_ATC::PlatformStart.size() == R_ATC::PlatformEnd.size()) {	//駅区間の始と終の要素数が一致したときのみ表示(0以外)
			for (size_t i = 0; i < R_ATC::PlatformStart.size(); i++) {	//登録駅全探索
				if (R_ATC::PlatformStart[i] <= (unsigned int)Stat.Zd + stopSection * 100 && R_ATC::PlatformEnd[i] >= (unsigned int)Stat.Zd) {	//駅始点が停止進路以前かつ駅終点が現在位置以降
					bool sw = false;
					for (size_t j = 0; j < (unsigned int)stopSection; j++) {

						//todo : 駅表示対応

						/*
						if (R_ATC::PlatformStart[i] >= Stat.Zd) sw = true;	//駅始点探索
						if (sw) {
							openInf[j] = 4;	//駅表示
							if (R_ATC::PlatformEnd[i] >= Stat.Zd + (j + 1) * 100) break;	//駅終点判定
						}
						*/
					}

				}
			}
		}


		for (size_t i = ATC_Panel::info_0; i < ATC_Panel::info_9; i++) {
			unsigned int buf = openInf[i - ATC_Panel::info_0];
			panel[i] = buf > 4 ? 0 : buf;	//開通情報設定(0-4)
		}
	}

	{	//開通情報(R-ATC)
		const short vTotalNum = 100;	//開通情報最大表示縦マス数 <=ini入力
		const short maxDisp = 1000;	//開通情報最大表示距離[m]
		if (this->StopLimit < maxDisp) {
			int a = this->StopLimit / maxDisp * vTotalNum;
			panel[ATC_Panel::openInfo_open] = a;	//開通領域設定
			panel[ATC_Panel::openInfo_unopen] = a + 1;	//未開通領域設定
		}
		else {	//直近1㎞開通
			panel[ATC_Panel::openInfo_open] = vTotalNum;
			panel[ATC_Panel::openInfo_unopen] = 0;
		}

		//todo : 駅表示対応

		if (R_ATC::Crossings.size() > 0) {
			short cnt = 0;
			for (size_t i = 0; i < R_ATC::Crossings.size(); i++) {
				if (!(R_ATC::Crossings[i] > Stat.Zd && R_ATC::Crossings[i] < Stat.Zd + maxDisp)) continue;
				panel[ATC_Panel::openInfo_crossing0 + cnt] = R_ATC::Crossings[i] / maxDisp * vTotalNum;	//踏切
				if (cnt >= 10) break;
				else cnt++;
			}
		}

	};
}

void R_ATC::Limit::SetTarget(int arg) {
	this->Target = arg;
}
void R_ATC::Limit::SetTarget(float arg) {
	this->Target = arg;
}
void R_ATC::Limit::SetTarget(double arg) {
	this->Target = arg;
}

bool R_ATC::Limit::isCalc() {
	if (this->Target != 0) return true;
	else return false;
}

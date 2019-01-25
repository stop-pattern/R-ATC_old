#pragma once
#ifndef _RATC_
#define _RATC_


#include "../header/ats.h"
#include "../header/define.h"


class R_ATC {
public:
	int stat;	//ATCstatus

	double StopLimit;	//停止限界残距離

	double Limit[3] = { 0,ATC_MAX,0 };	//制限速度[km/h]
	double Location[3] = { 0,0,0 };	//過走限界[m]

	int distance[5];	//先行列車距離程[m]
	int time[5];	//先行列車時刻[ms]
	double a, b, c[3];	//線形回帰パラメーター
	bool reload;	//再読み込み判定


	double target;	//目標
	double pattern_speed[2];	//P接近速度
	//double pattern_brake;	//���e�ō����x
	double notice_dist;	//P接近距離
	double brake_speed[11];	//B開始速度記録

	void Status(State, int *, int *);	//ATC状態管理
	void Calc(State, int *, int *);	//パラメーター算出
	void Pattern(State, int *, int *);	//ATC制御
private:
	enum param {
		P_none = 0,	//通常
		P_pretrain = 1,	//先行連動P
		P_2step = 2,	//2段P
	};
	enum stat {
		off = 0,
		on = 1,
		inside = 2,
		outside = 3,
	};
};



#endif // !_RATC_
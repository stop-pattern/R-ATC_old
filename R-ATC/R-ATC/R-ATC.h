#pragma once
#ifndef _RATC_
#define _RATC_


#include "../header/ats.h"
#include "../header/define.h"


namespace R_ATC {
	extern int stat;	//ATCstatus

	extern double StopLimit;	//停止限界残距離

	extern double Limit[3];	//制限速度[km/h]
	extern double Location[3];	//過走限界[m]

	extern int distance[5];	//先行列車距離程[m]
	extern int time[5];	//先行列車時刻[ms]
	extern double a, b, c[3];	//線形回帰パラメーター
	extern bool reload;	//再読み込み判定


	extern double target;	//目標
	extern double pattern_speed[2];	//P接近速度
	//extern double pattern_brake;	//���e�ō����x
	extern double notice_dist;	//P接近距離
	extern double brake_speed[11];	//B開始速度記録

	extern void Status(State, int *, int *);	//ATC状態管理
	extern void Calc(State, int *, int *);	//パラメーター算出
	extern void Control(State, int *, int *);	//ATC制御

	class Pattern {
	public:
		//double target;	//停止限界[m]
		double StopLimit;	//停止限界残距離[m]
		int target_Speed;	//目標速度[km/h]
		int target_Location;	//目標距離程[m]
		double P_Speed;	//P接近速度
		double B_Speed;	//B動作速度
		double P_Location;	//P接近距離
		double B_Location;	//B動作距離
		void calc();
	private:
		double a;	//先行計算用線形回帰パラメーター
		double b;	//先行計算用線形回帰パラメーター
		double c;	//先行計算用線形回帰パラメーター
	};


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
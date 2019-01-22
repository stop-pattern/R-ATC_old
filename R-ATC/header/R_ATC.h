#pragma once
#include "ats.h"
#include "define.h"


class R_ATC {
public:
	int stat;	//ATCstatus

	double Limit[3] = { 0,ATC_MAX,0 };	//現示速度[km/h]
	double Location[3] = { 0,0,0 };	//目標距離程[m]

	int distance[5];	//先行距離程[m]
	int time[5];	//先行時刻[ms]
	double a, b, c[3];	//速度判定用式パラメーター
	bool reload;	//変更反映指示


	double target;	//停止点残距離
	double pattern_speed[2];	//許容最高速度
	//double pattern_brake;	//許容最高速度
	double notice_dist;	//P接近判定距離
	double brake_speed[11];	//減速開始速度記録

	void Status(State, int *, int *);	//ATC状態管理
	void Calc(void);	//先行計算
	void Pattern(State, int *, int *);	//ATCパターン速度照査
	//void Sort(int, int array[]);	//先行変更地上子通過時並べ替え処理
private:
	enum param {
		P_none = 0,	//曲線制限等
		P_pretrain = 1,	//先行制限
		P_2step = 2,	//2段P
	};
	enum stat {
		off = 0,
		on = 1,
		inside = 2,
		outside = 3,
	};
};


R_ATC ATACS;
#pragma once
#ifndef _RATC_
#define _RATC_

#include <vector>
#include "../header/ats.h"
#include "../header/define.h"


class c_R_ATC {
private:
	class Pattern {
	public:
		//double Limit;	//制限速度[km/h] <=ATC現示値(=P_Speed
		double target = 0;	//停止限界[m] <=距離程
		double StopLimit = 0;	//停止限界残距離[m] <=毎フレーム更新値
		double target_Speed = 0;	//目標速度[km/h] <=入力値(固定)
		double target_Location = DBL_MAX;	//目標距離程[m] <=入力値(固定)
		double P_Speed = DBL_MAX;	//P接近速度
		double B_Speed = DBL_MAX;	//B動作速度
		double E_Speed = DBL_MAX;	//EB動作速度
		double P_Location = DBL_MAX;	//P接近距離
		//double B_Location;	//B動作距離
		//double E_Location;	//EB動作距離
		Pattern(double, double, double);
		int calc(State, int*, int*);
		void out(State, int*, int*);
		void SetBeaconData(int, int);
	private:
		double P_deceleration;	//P接近減速定数
		double B_deceleration;	//B動作減速定数
		double E_deceleration;	//EB動作減速定数
	};

	class Limit {
	public:
		//double Limit;	//制限速度[km/h] <=ATC現示値(=P_Speed
		double target = 0;	//停止限界[m] <=距離程
		double StopLimit = 0;	//停止限界残距離[m] <=毎フレーム更新値
		double target_Speed = 0;	//目標速度[km/h] <=入力値(固定)
		double target_Location = DBL_MAX;	//目標距離程[m] <=入力値(固定)
		int calc(State, int*, int*);
		void out(State, int*, int*);
		void SetBeaconData(int, int);
	};

public:

	enum stat {
		off = 0,
		on = 1,
		inside = 2,
		outside = 3,
	};
	enum pattern_name {
		PreTrain,	//先行列車連動P
		Step2,	//防護2段パターン
		Crossing,	//踏切防護P
		Route,	//路線依存P（曲線速度制限）
		number	//P総数
	};
	enum limit_name {
		PreTrain,	//先行列車連動P
		Step2,	//防護2段パターン
		Crossing,	//踏切防護P
		number	//総数
	};


	void Load();	//consractor
	void SetOut();
	void Status(State, int*, int*);	//ATC状態管理
	void Control(State, int*, int*);	//ATC制御
	bool Update(State, c_R_ATC::Pattern);	//P更新・判定


	Pattern* patterns[pattern_name::number];	//照査速度パターン制御
	Limit* limits[limit_name::number];	//過走限界計算

	int stat;	//ATCstatus

	//bool P;	//P接近
	//bool Bell;	//ATCベル

	double StopLimit;	//停止限界残距離

	//double Limit;	//制限速度[km/h]
	//double Location;	//過走限界[m]


	//double target;	//目標
	double pattern_speed[2];	//P接近速度
	//double pattern_brake;	//���e�ō����x
	//double notice_dist;	//P接近距離
	//double brake_speed[11];	//B開始速度記録

	//先行列車
	std::vector<int> PreTrain_Time;	//時刻
	std::vector<int> PreTrain_Distance;	//距離

	//路線情報
	std::vector<int> PlatformStart;	//ホーム区始端
	std::vector<int> PlatformEnd;	//ホーム区終端

};



#endif // !_RATC_
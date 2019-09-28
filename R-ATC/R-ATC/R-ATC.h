﻿#pragma once
#ifndef _RATC_
#define _RATC_

#include <vector>
#include "../header/ats.h"
#include "../header/define.h"


namespace R_ATC {
	struct RouteLimit {
		int Speed;
		double Position;
	};

	class Pattern {
	private:
		//const
		double P_deceleration;	//P接近減速定数
		double B_deceleration;	//B動作減速定数
		double E_deceleration;	//EB動作減速定数
		//status
		bool status = true;	//使用可否
		//variable about pattern
		double Limit = 0;	//制限速度(ATC現示値)[km/h] (=P_Speed <=calc{return}
		double target_Speed = 0;	//目標速度[km/h] <=入力値(固定)
		double target_Location = 0;	//目標距離程[m] <=入力値(固定)
	public:
		Pattern(double, double, double);
		int calc(State);	//メイン演算
		void SetBeaconData(RouteLimit);
		void out(State, int*, int*);	//出力
		bool isCalc();	//計算使用可否判定
		bool isCalc(bool);	//計算使用可否判定
		//入力
		void setSpeed(int);
		void setLocation(double);
	};

	class Limit {
	private:
		bool status = true;	//使用可否
		double Target = 0;	//停止限界[m] <=距離程 <=入力値(固定)
		double StopLimit = 0;	//停止限界残距離[m] <=毎フレーム更新値 <=calc{return}
	public:
		int calc(State);	//メイン演算
		void out(State, int*, int*);	//出力
		void SetTarget(int);
		void SetTarget(float);
		void SetTarget(double);
		bool isCalc();	//限界計算可否判定
		bool isCalc(bool);	//限界計算可否判定
	};


	enum class stat {
		off = 0,
		on = 1,
		inside = 2,
		outside = 3,
	};
	enum class pattern_name {
		PreTrain,	//先行列車連動P
		Step2,	//防護2段パターン
		Crossing,	//踏切防護P
		Route,	//路線依存P（曲線速度制限）
		number	//P総数
	};
	enum class limit_name {
		PreTrain,	//先行列車連動P
		Step2,	//防護2段パターン
		Crossing,	//踏切防護P
		number	//総数
	};


	void Load();	//consractor
	void Status(State, int *, int *);	//ATC状態管理
	void Interpolation();	//先行列車位置線形回帰
	void Control(State, int *, int *);	//ATC制御


	extern Pattern* patterns[static_cast<int>(pattern_name::number)];	//照査速度パターン制御
	extern Limit* limits[static_cast<int>(limit_name::number)];	//過走限界計算

	extern int status;	//ATCstatus
	extern double StopLimit;	//停止限界残距離
	extern double pattern_speed[2];	//P接近速度
	extern int maxSpeed;	//路線最高速度

	//先行列車
	extern std::vector<int> PreTrain_Time;	//時刻
	extern std::vector<int> PreTrain_Distance;	//距離

	//路線情報
	extern std::vector<RouteLimit> SpeedLimit;	//速度制限
	extern std::vector<double> Stop2Step;	//2段パターン
	extern std::vector<int> Crossings;	//踏切
	extern std::vector<int> PlatformStart;	//ホーム区始端
	extern std::vector<int> PlatformEnd;	//ホーム区終端

};



#endif // !_RATC_
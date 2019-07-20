#pragma once
#ifndef _RATC_
#define _RATC_

#include <vector>
#include "../header/ats.h"
#include "../header/define.h"


namespace R_ATC {
	extern int stat;	//ATCstatus

	extern bool P;	//P接近
	extern bool Bell;	//ATCベル

	extern double StopLimit;	//停止限界残距離

	extern double Limit;	//制限速度[km/h]
	extern double Location;	//過走限界[m]



	extern double target;	//目標
	extern double pattern_speed[2];	//P接近速度
	//extern double pattern_brake;	//���e�ō����x
	extern double notice_dist;	//P接近距離
	extern double brake_speed[11];	//B開始速度記録

	//先行列車
	extern std::vector<int> PreTrain_Time;	//時刻
	extern std::vector<int> PreTrain_Distance;	//距離


	class Pattern {
	public:
		double Limit;	//制限速度[km/h] <=ATC現示値(=P_Speed
		double target;	//停止限界[m] <=距離程
		double StopLimit;	//停止限界残距離[m] <=毎フレーム更新値
		double target_Speed;	//目標速度[km/h] <=入力値(固定)
		double target_Location;	//目標距離程[m] <=入力値(固定)
		double P_Speed;	//P接近速度
		double B_Speed;	//B動作速度
		double E_Speed;	//EB動作速度
		double P_Location;	//P接近距離
		double B_Location;	//B動作距離
		double E_Location;	//EB動作距離
		Pattern(double, double, double);
		void calc(State, int *, int *);
		void out(State, int *, int *);
		bool useage;	//線形回帰判別
		double jadge(void);	//P制御採用判定
		void SetBeaconData(int, int);
	private:
		double param;	//P制御採用判定用
		double P_deceleration;	//P接近減速定数
		double B_deceleration;	//B動作減速定数
		double E_deceleration;	//EB動作減速定数
		double a;	//先行計算用線形回帰パラメーター
		double b;	//先行計算用線形回帰パラメーター
		double c;	//先行計算用線形回帰パラメーター
	};

	extern R_ATC::Pattern PreTrain;    //先行列車連動P
	extern R_ATC::Pattern Step2;   //2段P
	extern R_ATC::Pattern Crossing;    //踏切防護P
	extern R_ATC::Pattern Route;   //路線依存P（曲線速度制限）


	extern void Status(State, int *, int *);	//ATC状態管理
	extern void Calc(State, int *, int *);	//パラメーター算出
	extern void Control(State, int *, int *);	//ATC制御
	extern bool Update(State, Pattern);	//P更新・判定
	extern void setout(void);	//出力値設定


	enum stat {
		off = 0,
		on = 1,
		inside = 2,
		outside = 3,
	};
};



#endif // !_RATC_
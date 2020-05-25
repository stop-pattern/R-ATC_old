#pragma once

#include <vector>
#include "../header/ats.h"
#include "../header/define.h"


class  R_ATC {
private:
	struct RouteLimit {
		int Speed;
		double Position;
	};

	class Pattern {
	private:
		//const
		const double P_deceleration;	//P接近減速定数
		const double B_deceleration;	//B動作減速定数
		const double E_deceleration;	//EB動作減速定数
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
		backup = 2,
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

public:
	R_ATC();
	~R_ATC();
	void Load();	//consractor
	void On();	//consractor
	void Off();	//consractor
	void Status(State, int *, int *);	//ATC状態管理
	void Interpolation();	//先行列車位置線形回帰
	void Control(State, int *, int *);	//ATC制御
	Hand Elapse(State, int*, int*);	//毎フレーム処理

private:
	std::array<Pattern, static_cast<size_t>(pattern_name::number)> patterns;	//照査速度パターン制御
	std::array<Limit, static_cast<size_t>(limit_name::number)> limits;	//過走限界計算
	

	int status;	//ATCstatus
	double StopLimit;	//停止限界残距離
	double pattern_speed[2];	//P接近速度
	int maxSpeed;	//路線最高速度

	//先行列車
	std::vector<std::pair<int, int>> PreTrain;
	std::vector<int> PreTrain_Time;	//時刻
	std::vector<int> PreTrain_Distance;	//距離

	//路線情報
	std::vector<RouteLimit> SpeedLimit;	//速度制限
	std::vector<double> Stop2Step;	//2段パターン
	std::vector<int> Crossings;	//踏切
	std::vector<int> PlatformStart;	//ホーム区始端
	std::vector<int> PlatformEnd;	//ホーム区終端

};





/*---------------------------------------------------------------------*/





class  R_ATC {
private:
	double P_deceleration;	// P接近減速定数
	double B_deceleration;	// B動作減速定数
	double E_deceleration;	// EB動作減速定数

	int status;	// 有効/無効
	int maxSpeed;	// 路線最高速度
	
	std::vector<std::pair<int, int>> PreTrain;	// 先行列車
	std::vector<std::pair<int, double>> SpeedLimit;	// 速度制限
	std::vector<std::pair<int, double>> Stop2Step;	// 2段パターン
	std::vector<int> Crossings;	// 踏切
	std::vector<int> PlatformStart;	// ホーム区始端
	std::vector<int> PlatformEnd;	// ホーム区終端

	enum class stat : uint8_t;	// 状態表示

public:
	R_ATC();
	R_ATC(double p, double b, double e, int max = 120);
	~R_ATC();
	void Interpolation();	// 先行列車位置線形回帰
	Hand Elapse(State t, int* p, int* s);	// 毎フレーム処理
	void SetBeaconData(Beacon b);	// beacon
};

R_ATC *rAtc;
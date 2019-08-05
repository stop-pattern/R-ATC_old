#pragma once
#ifndef HEADER
#define HEADER

#include <string>
#include <math.h>
#include "ats.h"
#include "../R-ATC/R-ATC.h"
#include "../ATC-6/ATC-6.h"

using namespace std;


extern string dllPath;	//.dllのpath
extern string iniPath;	//.iniのpath

extern int signal;	//信号インデックス
extern double speed;	//現在速度
extern double distance;	//現在位置

extern bool door;	//ドア状態
extern int ATCstatus;	//ATC状態
extern int LimitSpeed;	//制限速度
extern double accelaration;	//加減速度

extern Spec specific;	//車両性能
extern State Stat;	//車両状態(前フレーム)
extern Hand manual;	//手動ハンドル位置
extern Hand handle;	//ハンドル制御値

extern c_R_ATC* R_ATC;	//R-ATC


void GetPath(HMODULE hModule);
double Sort(double, int array[], int);
void reload(void);	//駅ジャンプ時再読み込み処理

#endif // !HEADER
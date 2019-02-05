#pragma once
#ifndef HEADER
#define HEADER

#include <string>
#include <math.h>
#include "ats.h"
#include "../R-ATC/R-ATC.h"
#include "../ATC-6/ATC-6.h"

using namespace std;


string dllPath;	//.dllのpath
string iniPath;	//.iniのpath

int signal;	//信号インデックス
bool door;	//ドア状態
int ATCstatus;	//ATC状態
int LimitSpeed;	//制限速度
double accelaration;	//加減速度

Spec specific;	//車両性能
State Stat;	//車両状態(前フレーム)
Hand manual;	//手動ハンドル位置
Hand handle;	//ハンドル制御値


void GetPath(HMODULE hModule);
double Sort(double, int array[], int);

#endif // !HEADER
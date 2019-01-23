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
struct Spec specific;	//車両性能
struct Hand manual;	//手動ハンドル位置
struct Hand handle;	//ハンドル制御値

R_ATC ATACS;
ATC_6 ATC6;


#endif // !HEADER
void GetPath(HMODULE hModule);
double Sort(double, int array[], int);
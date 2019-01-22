#pragma once
#include <math.h>
#include "ats.h"
#include "define.h"
#include "GetIni.h"


string dllPath;	//.dllのpath
string iniPath;	//.iniのpath
int signal;	//信号インデックス
bool door;	//ドア状態
Spec specific;	//車両性能
Hand manual;	//手動ハンドル位置
void GetPath(HMODULE hModule);
double Sort(double,int array[],int);




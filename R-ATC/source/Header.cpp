#include "../header/Header.h"

string dllPath;	//.dllのpath
string iniPath;	//.iniのpath

int signal;	//信号インデックス
double speed;	//現在速度
double distance;	//現在位置

bool door;	//ドア状態
int ATCstatus;	//ATC状態
int LimitSpeed;	//制限速度
double accelaration;	//加減速度

Spec specific;	//車両性能
State Stat;	//車両状態(前フレーム)
Hand manual;	//手動ハンドル位置
Hand handle;	//ハンドル制御値

c_R_ATC* R_ATC;	//R-ATC
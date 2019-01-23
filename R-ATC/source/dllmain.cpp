/*BVE ATSプラグインテンプレート
 *Author : Tetsu Otter
 *SDK : ver10.0.17763.0
 *DLLは、プロジェクト名で出力されます。任意のプロジェクト名を付与してください。
 *プロジェクト名変更時は、dllmain.defの "LIBRARY" に設定された "PITemp" という文字列を新たなプロジェクト名に変更してください。
*/

#include <string.h>
#include <math.h>
#include "../R-ATC/R-ATC.h"
#include "../ATC-6/ATC-6.h"
#include "../header/Header.h"
#include "../header/define.h"

#include <fstream>


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	GetPath(hModule);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

DE void SC Load() {/*
	ini.GetIni(iniPath);
	ofstream fs;
	fs.open("./output.log", ios::app);
	while (!fs.is_open())
		fs << iniPath << endl;
	fs.close();*/
}
DE void SC Dispose() {
}
DE int SC GetPluginVersion() {
	return PI_VERSION;
}
DE void SC SetVehicleSpec(Spec s)
{
	specific.A = s.A;
	specific.J = s.J;
	specific.B = s.B;
	specific.C = s.C;
	specific.P = s.P;
}
DE void SC Initialize(int b) {
	b = InitialPosition::Service;
}
DE Hand SC Elapse(State S, int * p, int * s)
{
	handle.P = manual.P;
	handle.B = manual.B;
	handle.R = manual.R;

	if (ATACS.stat == 0) {	//ATACSoff
		if (ATC6.status == true)ATC6.Check(S, p, s);	//ATC-6
		if (ATC6.Emergency == true)ATC6.EmergencyDrive(S, p, s);
	}
	ATACS.Status(S, p, s);	//状態管理
	ATACS.Calc();	//先行計算
	ATACS.Pattern(S, p, s);	//指令制御

	//*/TIMS代用機能
		//速度計
	p[50] = abs(int(S.V));
	p[83] = abs(int(S.V)) / 100 % 10;
	p[84] = abs(int(S.V)) / 10 % 10;
	p[85] = abs(int(S.V)) % 10;
	p[93] = abs(S.I);	//test
//ユニット
	if (S.I = 0)
	{
		p[105] = 0;
		p[106] = 0;
		p[107] = 0;
		p[108] = 0;
	}
	else
	{
		p[41] = true;
		p[42] = true;
		p[43] = true;
		p[44] = true;
	}
	for (size_t i = 105; i < 109; i++)
	{
		if (S.I > 0 && p[i] != 1 && rand() % 25 == 0) { p[i] = 1; }
		else if (S.I < 0 && p[i] != 2 && rand() % 25 == 0) { p[i] = 2; }
		else if (S.I == 0 && p[i] != 0 && rand() % 25 == 0) { p[i] = 0; }
	}

	handle.C = ConstSPInfo::Continue;
	//s[255] = SoundInfo::Continue;

	return handle;
}
DE void SC SetPower(int p) {
	manual.P = p;
}
DE void SC SetBrake(int b) {
	manual.B = b;
}
DE void SC SetReverser(int r) {
	manual.R = r;
}
DE void SC DoorOpen() {
	door = true;
}
DE void SC DoorClose() {
	door = false;
}


DE void SC KeyDown(int k) {

	switch (k)
	{
	case ATSKeys::C1:	//ATC-6非常運転
		ATC6.Emergency = true;
		break;

	default:
		break;
	}
}
DE void SC KeyUp(int k) {
}
DE void SC HornBlow(int k) {

}
DE void SC SetSignal(int a) {
	ATC6.GetSignal(a);
	signal = a;
}
DE void SC SetBeaconData(Beacon b) {
	switch (b.Num)
	{
	case ATC_Beacon::SpeedDown:
		ATACS.Location[0] = int(b.Data / 1000);
	case ATC_Beacon::SpeedUp:
		ATACS.Limit[0] = b.Data % 1000;
		break;
	case ATC_Beacon::PlatformStart_1:
	case ATC_Beacon::PlatformStart_2:

		break;
	case ATC_Beacon::PlatformEnd_1:
	case ATC_Beacon::PlatformEnd_2:

		break;
	case ATC_Beacon::LocationCorrection:

		break;
	case ATC_Beacon::PreTrainDistance_1:
	case ATC_Beacon::PreTrainDistance_2:
	case ATC_Beacon::PreTrainDistance_3:
	case ATC_Beacon::PreTrainDistance_4:
	case ATC_Beacon::PreTrainDistance_5:
		Sort(b.Data, ATACS.distance, 5);
		break;
	case ATC_Beacon::PreTrainTime_1:
	case ATC_Beacon::PreTrainTime_2:
	case ATC_Beacon::PreTrainTime_3:
	case ATC_Beacon::PreTrainTime_4:
	case ATC_Beacon::PreTrainTime_5:
		Sort(b.Data, ATACS.time, 5);
		break;
	case 235:
		ATACS.Location[1] = b.Data;
		break;
	case ATC_Beacon::Status:
		ATACS.stat = b.Data;
		break;
	case ATC_Beacon::SetPattern:
		ATACS.Location[2] = b.Data;
	default:
		break;
	}
}


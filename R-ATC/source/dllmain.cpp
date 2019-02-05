/*BVE ATSプラグインテンプレート
 *Author : Tetsu Otter
 *SDK : ver10.0.17763.0
 *DLLは、プロジェクト名で出力されます。任意のプロジェクト名を付与してください。
 *プロジェクト名変更時は、dllmain.defの "LIBRARY" に設定された "PITemp" という文字列を新たなプロジェクト名に変更してください。
*/

#include <string.h>
#include <math.h>
#include "../header/Header.h"
#include "../header/define.h"
#include "../TIMS.h"

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
DE void SC SetVehicleSpec(Spec s) {
	specific.A = s.A;
	specific.J = s.J;
	specific.B = s.B;
	specific.E = s.B + 1;
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


	//02
	if (ATCstatus == ATC_status::OFF) p[ATC_Panel::ATC01] = 2;

	//ATC構内
	if (ATCstatus == ATC_status::OFF) {
		if (S.V > SPEED_ATS_OFF) {
			/*
			handle.P = 0;
			handle.B = specific.E;
			*/
		}
	}

#ifdef  TIMS	//TIMS代用機能

	//速度計
	p[50] = abs(int(S.V));
	p[83] = abs(int(signal)) / 100 % 10;
	p[84] = abs(int(signal)) / 10 % 10;
	p[85] = abs(int(signal)) % 10;
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
#endif //  TIMS

	handle.C = ConstSPInfo::Continue;
	//s[255] = SoundInfo::Continue;

	if (S.V >= 7.5)
	{
		//[km/h/s]	(S.V - Stat.V)
	}

	Stat = S;
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
	default:
		break;
	}
}
DE void SC KeyUp(int k) {
}
DE void SC HornBlow(int k) {

}
DE void SC SetSignal(int a) {

	if (a > 9) {
		if (a == 35) ATCstatus = ATC_status::ATC__10_ORP;
		if (a < 35) ATCstatus = ATC_status::ATC__10;
		else if (a > 49)
		{
			if (a < 61) ATCstatus = ATC_status::R__ATC;
			else if (a > 100) {
				if (a < 113)ATCstatus = ATC_status::ATC__6;
				else ATCstatus = ATC_status::OFF;
			}
			else ATCstatus = ATC_status::OFF;
		}
		else ATCstatus = ATC_status::OFF;
	}
	else ATCstatus = ATC_status::OFF;

	signal = a;
}
DE void SC SetBeaconData(Beacon b) {
	switch (b.Num) {
	default:
		break;
	}
}


/*BVE ATSプラグインテンプレート
 *Author : Tetsu Otter
 *SDK : ver10.0.17763.0
 *DLLは、プロジェクト名で出力されます。任意のプロジェクト名を付与してください。
 *プロジェクト名変更時は、dllmain.defの "LIBRARY" に設定された "PITemp" という文字列を新たなプロジェクト名に変更してください。
*/

#include <string.h>
#include <math.h>
#include <algorithm>
#include "../R-ATC/R-ATC.h"
#include "../ATC-6/ATC-6.h"
#include "../ATC-10/ATC-10.h"
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

DE void SC Load() {
	rAtc = new R_ATC(double p, double b, double e);
	rAtc->Load();
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
	if (!door) handle.P = manual.P;
	else handle.P = 0;
	handle.B = manual.B;
	handle.R = manual.R;


	if (ATC6::Emergency == true)ATC6::EmergencyDrive(S, p, s);	//ATC-6非常運転
	//ATC6::Check(S, p, s);	//ATC-6

	//R-ATC
	if (ATCstatus == ATC_status::R__ATC) rAtc->Control(S, p, s);

	//02
	if (ATCstatus == ATC_status::OFF) p[ATC_Panel::ATC01] = 2;

	//ATS構内
	if (ATCstatus == ATC_status::OFF) {

		//ATCoff
		p[ATC_Panel::ATCpower] = true;	//解放運転時以外on
		p[ATC_Panel::ATC] = false;
		p[ATC_Panel::ATCoff] = false;
		p[ATC_Panel::ATCrelease] = false;
		p[ATC_Panel::ATCemdrive] = false;
		p[ATC_Panel::ATCemr] = false;
		p[ATC_Panel::ATCbrake] = false;

		p[ATC_Panel::ATC01] = 2;	//矢印隠し
		p[ATC_Panel::ATC02] = false;
		
		if (S.V > SPEED_ATS_OFF) {
			/*
			handle.P = 0;
			handle.B = specific.E;
			*/
		}
	}


	if (S.V >= 7.5)	{
		//[km/h/s]	(S.V - Stat.V)
	}

	Stat.setData(Stat, S);
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
		if (manual.P == 0 && manual.B == specific.B + 1) {
			ATC6::Emergency = true;
		}
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

	if (a > 9) {
		if (a == 35) ATCstatus = ATC_status::ATC__10_ORP;
		if (a < 35) ATCstatus = ATC_status::ATC__10;
		else if (a > 49) {
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

	ATC6::GetSignal(a);
	signal = a;
}
DE void SC SetBeaconData(Beacon b) {
	switch (b.Num) {
	case ATC_Beacon::Status:
		R_ATC::status = b.Data;
		break;
	case ATC_Beacon::SpeedDown:
	case ATC_Beacon::SpeedUp:
		{
			R_ATC::RouteLimit temp;
			temp.Position = int(b.Data / 1000);	//下3桁切り捨て
			temp.Speed = int(b.Data % 1000);	//下3桁のみ
			R_ATC::patterns[static_cast<int>(R_ATC::pattern_name::Route)]->SetBeaconData(temp);
		}
	case ATC_Beacon::PlatformStart:
		R_ATC::PlatformStart.push_back(b.Data);
		std::sort(R_ATC::PlatformStart.begin(), R_ATC::PlatformStart.end());	//昇順ソート
		break;
	case ATC_Beacon::PlatformEnd:
		R_ATC::PlatformEnd.push_back(b.Data);
		std::sort(R_ATC::PlatformEnd.begin(), R_ATC::PlatformEnd.end());	//昇順ソート
		break;
	case ATC_Beacon::LocationCorrection:
		Stat.Zd = b.Data;
		break;
	case ATC_Beacon::PreTrainDistance:
		R_ATC::PreTrain_Distance.push_back(b.Data);
		break;
	case ATC_Beacon::PreTrainTime:
		R_ATC::PreTrain_Time.push_back(b.Data * 1000);
		break;
	case ATC_Beacon::Set2StepPattern:
		R_ATC::Stop2Step.push_back(b.Data / 10);
		break;
	case ATC_Beacon::Crossing:
		R_ATC::Crossings.push_back(b.Data);
		std::sort(R_ATC::Crossings.begin(), R_ATC::Crossings.end());	//昇順ソート
		break;
	case ATC_Beacon::ATC10_notice_f:
		ATC10::Notice(b);
		break;
	default:
		break;
	}
}



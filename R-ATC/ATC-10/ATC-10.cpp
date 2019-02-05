#include "ATC-10.h"
#include "../header/define.h"

extern int signal;	//信号インデックス
extern bool door;	//ドア状態
extern int ATCstatus;	//ATC状態
extern int LimitSpeed;	//制限速度
extern struct Spec specific;	//車両性能
extern struct Hand manual;	//手動ハンドル位置
extern struct Hand handle;	//ハンドル制御値


int ATC10::ORP;	//
int ATC10::notice;	//前方予告
bool ATC10::status;	//ATC状態
bool ATC10::Emergency;	//非常運転
bool ATC10::Bell;	//単打ベル
void ATC10::GetSignal(int);
void ATC10::Check(State, int *, int *);
void ATC10::EmergencyDrive(State, int *, int *);


void ATC10::GetSignal(int sig) {
	Emergency = false;
	notice = false;
	if (sig == 35)LimitSpeed = ORP;
	if (LimitSpeed != SpeedLimit[sig])Bell = true;
	LimitSpeed = SpeedLimit[sig];
}

void ATC10::Check(State status, int *panel, int *sound) {
	if (ATCstatus == ATC_status::ATC__10) {

		panel[ATC_Panel::Limit_1] = int(LimitSpeed);	//速度矢印1k
		panel[ATC_Panel::Limit_5] = int(LimitSpeed) % 10 > 5.0 ? (int(LimitSpeed / 10) + 1) * 10 : int(LimitSpeed / 10) * 10;	//速度矢印5k

		if (notice)panel[ATC_Panel::fw_notice] = true;

		//ATC-01/針隠し
		if (LimitSpeed == 0) panel[ATC_Panel::ATC01] = 1;
		else panel[ATC_Panel::ATC01] = 0;

		//ブレーキ動作判定
		if (status.V >= LimitSpeed) {	//現示超過で6割ブレーキ
			panel[ATC_Panel::ATCbrake] = true;
			handle.P = 0;
			handle.B = specific.B * 0.6;
			if (status.V >= LimitSpeed + 5) {	//現示超過+5km/hで常用最大
				handle.B = specific.B;
			}
		}
		else {
			panel[ATC_Panel::pattern] = false;
			panel[ATC_Panel::ATCbrake] = false;
		}

		//単打ベル
		sound[ATC_bell] = SoundInfo::PlayContinue;
		if (Bell == true) {
			sound[ATC_bell] = SoundInfo::PlayOnce;
		}
		Bell = false;
	}
	else {
		panel[ATC_Panel::fw_notice] = false;
		panel[ATC_Panel::ATC_G] = false;
		panel[ATC_Panel::pattern] = false;
		panel[ATC_Panel::ATCbrake] = false;
	}
}

void ATC10::EmergencyDrive(State status, int *panel, int *sound) {
	if (abs(int(status.V)) == 0 && manual.B == specific.B && manual.P == 0) {
		panel[ATC_Panel::ATCemdrive] = true;
		LimitSpeed = ATC10_EMERGENCY_SPEED;
		Bell = true;
	}
}

void ATC10::Notice(Beacon data) {	//前方予告判定
	if (LimitSpeed >= SpeedLimit[data.Sig])	{
		notice = SpeedLimit[data.Sig];
	}
}
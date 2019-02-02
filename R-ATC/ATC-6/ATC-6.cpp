#include "ATC-6.h"
#include "../header/define.h"

extern int signal;	//信号インデックス
extern bool door;	//ドア状態
extern int ATCstatus;	//ATC状態
extern int LimitSpeed;	//制限速度
extern struct Spec specific;	//車両性能
extern struct Hand manual;	//手動ハンドル位置
extern struct Hand handle;	//ハンドル制御値

bool ATC6::status;	//ATC状態
bool ATC6::Emergency;	//非常運転
bool ATC6::Bell;	//単打ベル
void ATC6::GetSignal(int);
void ATC6::Check(State, int *, int *);
void ATC6::EmergencyDrive(State, int *, int *);


void ATC6::GetSignal(int sig) {
	Emergency = false;
	if (sig == 35)LimitSpeed = ATC6_ORP_SPEED;
	if (LimitSpeed != SpeedLimit[sig])Bell = true;
	LimitSpeed = SpeedLimit[sig];
}

void ATC6::Check(State status, int *panel, int *sound) {
	if (ATCstatus == ATC_status::ATC__6 || ATCstatus == ATC_status::ATC__6_emergency) {

		panel[ATC_Panel::Limit_1] = int(LimitSpeed);	//速度矢印1k
		panel[ATC_Panel::Limit_5] = int(LimitSpeed) % 10 > 5.0 ? (int(LimitSpeed / 10) + 1) * 10 : int(LimitSpeed / 10) * 10;	//速度矢印5k

		//ATC-01/針隠し
		if (LimitSpeed == 0) panel[ATC_Panel::ATC01] = 1;
		else panel[ATC_Panel::ATC01] = 0;

		//ブレーキ動作判定
		if (status.V >= LimitSpeed - 5) {
			//panel[ATC_Panel::pattern] = true;	//現示-5km/hでP接近点灯
			if (status.V >= LimitSpeed) {	//現示超過で常用最大
				panel[ATC_Panel::ATCbrake] = true;
				handle.P = 0;
				handle.B = specific.B;
			}
		}
		else {
			panel[ATC_Panel::pattern] = false;
			panel[ATC_Panel::ATCbrake] = false;
		}

		//単打ベル
		sound[ATC_6_bell] = SoundInfo::PlayContinue;
		if (Bell == true) {
			sound[ATC_6_bell] = SoundInfo::PlayOnce;
		}
		Bell = false;
	}
	else {
		panel[ATC_Panel::pattern] = false;
		panel[ATC_Panel::ATCbrake] = false;
	}
}

void ATC6::EmergencyDrive(State status, int *panel, int *sound) {
	if (abs(int(status.V)) == 0 && manual.B == specific.B && manual.P == 0) {
		panel[ATC_Panel::ATCemdrive] = true;
		LimitSpeed = ATC6_EMERGENCY_SPEED;
		Bell = true;
	}
}
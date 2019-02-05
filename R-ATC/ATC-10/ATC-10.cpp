#include "ATC-10.h"
#include "../header/define.h"

extern int signal;	//�M���C���f�b�N�X
extern bool door;	//�h�A���
extern int ATCstatus;	//ATC���
extern int LimitSpeed;	//�������x
extern struct Spec specific;	//�ԗ����\
extern struct Hand manual;	//�蓮�n���h���ʒu
extern struct Hand handle;	//�n���h������l


int ATC10::ORP;	//
int ATC10::notice;	//�O���\��
bool ATC10::status;	//ATC���
bool ATC10::Emergency;	//���^�]
bool ATC10::Bell;	//�P�Ńx��
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

		panel[ATC_Panel::Limit_1] = int(LimitSpeed);	//���x���1k
		panel[ATC_Panel::Limit_5] = int(LimitSpeed) % 10 > 5.0 ? (int(LimitSpeed / 10) + 1) * 10 : int(LimitSpeed / 10) * 10;	//���x���5k

		if (notice)panel[ATC_Panel::fw_notice] = true;

		//ATC-01/�j�B��
		if (LimitSpeed == 0) panel[ATC_Panel::ATC01] = 1;
		else panel[ATC_Panel::ATC01] = 0;

		//�u���[�L���씻��
		if (status.V >= LimitSpeed) {	//�������߂�6���u���[�L
			panel[ATC_Panel::ATCbrake] = true;
			handle.P = 0;
			handle.B = specific.B * 0.6;
			if (status.V >= LimitSpeed + 5) {	//��������+5km/h�ŏ�p�ő�
				handle.B = specific.B;
			}
		}
		else {
			panel[ATC_Panel::pattern] = false;
			panel[ATC_Panel::ATCbrake] = false;
		}

		//�P�Ńx��
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

void ATC10::Notice(Beacon data) {	//�O���\������
	if (LimitSpeed >= SpeedLimit[data.Sig])	{
		notice = SpeedLimit[data.Sig];
	}
}
#pragma once
#ifndef _ATC10_
#define _ATC10_

#include"../header/ats.h"

#define ATC10_EMERGENCY_SPEED 15



namespace ATC10 {
	extern int ORP;	//
	extern int notice;	//�O���\��
	extern bool status;	//ATC���
	extern bool Emergency;	//���^�]
	extern bool Bell;	//�P�Ńx��
	extern void GetSignal(int);
	extern void Check(State, int *, int *);
	extern void EmergencyDrive(State, int *, int *);
	extern void Notice(Beacon);	//�O���\������
};

#endif // _ATC10_
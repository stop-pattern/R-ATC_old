#pragma once
#ifndef _ATC6_
#define _ATC6_

#include"../header/ats.h"

#define ATC6_EMERGENCY_SPEED 15
#define ATC6_ORP_SPEED 15


namespace ATC6 {
	extern bool status;	//ATC状態
	extern bool Emergency;	//非常運転
	extern bool Bell;	//単打ベル
	extern void GetSignal(int);
	extern void Check(State, int *, int *);
	extern void EmergencyDrive(State, int *, int *);
};


#endif // !_ATC6_
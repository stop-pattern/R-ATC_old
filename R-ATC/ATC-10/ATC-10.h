#pragma once
#ifndef _ATC10_
#define _ATC10_

#include"../header/ats.h"

#define ATC10_EMERGENCY_SPEED 15



namespace ATC10 {
	extern int ORP;	//
	extern int notice;	//Oϋ\
	extern bool status;	//ATCσΤ
	extern bool Emergency;	//ρν^]
	extern bool Bell;	//PΕx
	extern void GetSignal(int);
	extern void Check(State, int *, int *);
	extern void EmergencyDrive(State, int *, int *);
	extern void Notice(Beacon);	//Oϋ\»θ
};

#endif // _ATC10_
#pragma once
#ifndef _ATC10_
#define _ATC10_

#include"../header/ats.h"

#define ATC10_EMERGENCY_SPEED 15



namespace ATC10 {
	extern int ORP;	//
	extern int notice;	//‘O•û—\
	extern bool status;	//ATCó‘Ô
	extern bool Emergency;	//”ñí‰^“]
	extern bool Bell;	//’P‘Åƒxƒ‹
	extern void GetSignal(int);
	extern void Check(State, int *, int *);
	extern void EmergencyDrive(State, int *, int *);
	extern void Notice(Beacon);	//‘O•û—\”»’è
};

#endif // _ATC10_
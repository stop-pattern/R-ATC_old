#pragma once
#ifndef _RATC_
#define _RATC_


#include "../header/ats.h"
#include "../header/define.h"


class R_ATC {
public:
	int stat;	//ATCstatus

	double Limit[3] = { 0,ATC_MAX,0 };	//�������x[km/h]
	double Location[3] = { 0,0,0 };	//�ڕW������[m]

	int distance[5];	//��s������[m]
	int time[5];	//��s����[ms]
	double a, b, c[3];	//���x����p���p�����[�^�[
	bool reload;	//�ύX���f�w��


	double target;	//��~�_�c����
	double pattern_speed[2];	//���e�ō����x
	//double pattern_brake;	//���e�ō����x
	double notice_dist;	//P�ڋߔ��苗��
	double brake_speed[11];	//�����J�n���x�L�^

	void Status(State, int *, int *);	//ATC��ԊǗ�
	void Calc(void);	//��s�v�Z
	void Pattern(State, int *, int *);	//ATC�p�^�[�����x�ƍ�
	//void Sort(int, int array[]);	//��s�ύX�n��q�ʉߎ����בւ�����
private:
	enum param {
		P_none = 0,	//�Ȑ�������
		P_pretrain = 1,	//��s����
		P_2step = 2,	//2�iP
	};
	enum stat {
		off = 0,
		on = 1,
		inside = 2,
		outside = 3,
	};
};



#endif // !_RATC_
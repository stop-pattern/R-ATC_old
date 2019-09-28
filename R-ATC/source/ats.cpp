#include "../header/ats.h"


void SpecPlus::setData(Spec s) {
	this->A = s.A;
	this->B = s.B;
	this->C = s.C;
	this->E = s.B + 1;
	this->J = s.J;
	this->P = s.P;
}
void StatePlus::setData(StatePlus b, State a) {
	this->BC = a.BC;
	this->BP = a.BP;
	this->ER = a.ER;
	this->I = a.I;
	this->MR = a.MR;
	this->SAP = a.SAP;
	this->T = a.T;
	this->V = a.V;
	this->Z = a.Z;
	this->dV = (double)a.V - (double)b.V;
	this->dT = (double)a.T - (double)b.T;
	this->Zd = b.Z + (this->dV * this->dT);
	this->A = this->dT != 0 ? this->dV / this->dT : 0;
}
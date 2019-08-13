#pragma once

#include <cmath>
#include "EffectiveDigit.h"

EffectiveDigit::EffectiveDigit() {
	this->value = 0;
	this->digit = 0;
}
EffectiveDigit::EffectiveDigit(int arg0) {
	this->value = arg0;
	this->digit =2;

}
EffectiveDigit::EffectiveDigit(int arg0, int arg1) {
	this->value = arg0;
	this->digit = arg1;
}


void EffectiveDigit::setValue(int arg) {
	this->value = arg;
}
void EffectiveDigit::setValue(float arg) {
	this->setValue(static_cast<double>(arg));
}
void EffectiveDigit::setValue(double arg) {
	this->value = static_cast<double>(arg) / std::pow(10, this->digit);
}


float EffectiveDigit::toFloat() {
	return this->value / std::pow(10, this->digit);
}
double EffectiveDigit::toDouble() {
	return this->value / std::pow(10, this->digit);
}
#pragma once
class EffectiveDigit {
private:
	int value;
	int digit;

public:
	//ctor
	EffectiveDigit();
	EffectiveDigit(int);
	EffectiveDigit(int, int);

	//input
	void setValue(int);
	void setValue(float);
	void setValue(double);

	//output
	float toFloat();
	double toDouble();
};


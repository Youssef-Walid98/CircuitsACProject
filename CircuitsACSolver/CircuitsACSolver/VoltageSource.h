#pragma once
#include "Component.h"
class VoltageSource :
	public Component
{
private:
	double phase;
	double magnitude;
	double pi = 3.14159265359;
	std::complex<double> val;
public:
	VoltageSource(int pNode, int nNode, double magnitude, double phase);
	double getPhase();
	double getMagnitude();
	std::complex<double> getVal();
	VoltageSource();
	~VoltageSource();
};


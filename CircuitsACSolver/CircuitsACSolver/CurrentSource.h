#pragma once
#include "Component.h"
#include <complex>
class CurrentSource :
	public Component
{
private:
	double phase;
	double magnitude;
	double pi = 3.14159265359;
	std::complex<double> val;
public:
	CurrentSource(int pNode, int nNode, double magnitude, double phase);
	double getPhase();
	double getMagnitude();
	std::complex<double> getVal();
	CurrentSource();
	~CurrentSource();
};


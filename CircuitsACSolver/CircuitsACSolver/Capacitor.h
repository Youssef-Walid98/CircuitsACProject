#pragma once
#include "Component.h"
#include <complex>
class Capacitor :
	public Component
{
private:
	double capacitance;
	std::complex<double> XC;
public:
	Capacitor();
	Capacitor(int node1, int node2, double capacitance, double w);
	std::complex<double> getR();
	std::complex<double> getInvR();
	~Capacitor();
};


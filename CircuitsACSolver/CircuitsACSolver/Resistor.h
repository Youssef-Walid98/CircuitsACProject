#pragma once
#include "Component.h"
#include <complex>
class Resistor :
	public Component
{
private:
	std::complex<double> resistance;
public:
	Resistor();
	Resistor(int node1, int node2, double resistance);
	std::complex<double> getR();
	std::complex<double> getInvR();
	~Resistor();
};


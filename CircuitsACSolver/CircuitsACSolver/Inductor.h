#pragma once
#include "Component.h"
class Inductor :
	public Component
{
private:
	double inductance;
	std::complex < double> XL;
public:
	Inductor();
	Inductor(int node1, int node2, double inductance,double w);
	std::complex<double> getR();
	std::complex<double> getInvR();
	~Inductor();
};


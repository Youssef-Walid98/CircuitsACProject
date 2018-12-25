#include "Inductor.h"



Inductor::Inductor()
{
}

Inductor::Inductor(int node1, int node2, double inductance, double w) :Component(node1, node2)
{
	this->inductance = inductance;
	this->XL = std::complex<double>(0, (w * inductance));
}

std::complex<double> Inductor::getR()
{
	return this->XL;
}

std::complex<double> Inductor::getInvR()
{
	return (std::complex<double>(1, 0) / XL);
}

Inductor::~Inductor()
{
}

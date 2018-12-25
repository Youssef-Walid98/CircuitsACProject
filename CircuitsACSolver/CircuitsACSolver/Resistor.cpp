#include "Resistor.h"



Resistor::Resistor()
{
}

Resistor::Resistor(int node1, int node2, double resistance):Component(node1,node2)
{
	this->resistance = std::complex<double>(resistance,0);
}

std::complex<double> Resistor::getR()
{
	return this->resistance;
}

std::complex<double> Resistor::getInvR()
{
	return (std::complex<double>(1, 0) / resistance);
}


Resistor::~Resistor()
{
}
